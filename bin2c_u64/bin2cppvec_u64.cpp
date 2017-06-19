/*
 * Copyright 2017 Christian Helmich. All rights reserved.
 * Based on work by Branimir Karadzic
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#include <fmt/format.h>

#include <cstdio>
#include <string>
#include <vector>

#include <bx/commandline.h>
#include <bx/crtimpl.h>
#include <bx/string.h>

#include "u64_c_writer.h"

const char* bin2cppVec_srcText = R"(/*
 * generated by bin2cppVec
 * DO NOT MODIFY
 */

#include <vector>

#ifdef __cplusplus
{extC_open}
#endif // __cplusplus

extern const std::vector<uint8_t> {name}; // for linker happiness
const std::vector<uint8_t> {name}(
	reinterpret_cast<const uint8_t*>(g_{name}_data),
	reinterpret_cast<const uint8_t*>(g_{name}_data) + g_{name}_size
);

#ifdef __cplusplus
{extC_close}
#endif // __cplusplus

)";


class Bin2CppVec_U64_Writer : public Bin2C_U64_Writer
{
public:
	Bin2CppVec_U64_Writer(bx::WriterI* _writer, const char* _name) : Bin2C_U64_Writer(_writer, _name)
	{
	}

	virtual ~Bin2CppVec_U64_Writer()
	{
	}

	virtual void finish()
	{
		Bin2C_U64_Writer::finish();

		bx::writePrintf(m_writer,
						fmt::format(bin2cppVec_srcText,
									fmt::arg("name", m_name),
									fmt::arg("extC_open", R"(extern "C" {)"),
									fmt::arg("extC_close", R"(} // extern "C")"))
						  .c_str());
	}
};

void help(const char* _error = NULL)
{
	if (NULL != _error)
	{
		fprintf(stderr, "Error:\n%s\n\n", _error);
	}

	fprintf(stderr,
			"bin2cppvec, binary to C++ std::vector\n"
			"Copyright 2017 Christian Helmich. All rights reserved.\n"
			"Based on work by Branimir Karadzic.\n"
			"License: https://github.com/bkaradzic/bx#license-bsd-2-clause\n\n");

	fprintf(stderr,
			"Usage: bin2cppvec -f <in> -o <out> -n <name>\n"

			"\n"
			"Options:\n"
			"  -f <file path>    Input file path.\n"
			"  -o <file path>    Output file path.\n"
			"  -n <name>         Array name.\n"

			"\n"
			"For additional information, see https://github.com/bkaradzic/bx\n");
}


int main(int _argc, const char* _argv[])
{
	bx::CommandLine cmdLine(_argc, _argv);

	if (cmdLine.hasArg('h', "help"))
	{
		help();
		return EXIT_FAILURE;
	}

	const char* filePath = cmdLine.findOption('f');
	if (NULL == filePath)
	{
		help("Input file name must be specified.");
		return EXIT_FAILURE;
	}

	const char* outFilePath = cmdLine.findOption('o');
	if (NULL == outFilePath)
	{
		help("Output file name must be specified.");
		return EXIT_FAILURE;
	}

	const char* name = cmdLine.findOption('n');
	if (NULL == name)
	{
		name = "data";
	}

	void*  data = NULL;
	size_t size = 0;

	bx::CrtFileReader fr;
	if (bx::open(&fr, filePath))
	{
		size = (size_t)bx::getSize(&fr);
		data = malloc(size);
		bx::read(&fr, data, size);

		bx::CrtFileWriter fw;
		if (bx::open(&fw, outFilePath))
		{
			Bin2CppVec_U64_Writer writer(&fw, name);
			bx::write(&writer, data, size);
			writer.finish();
			bx::close(&fw);
		}

		free(data);
	}

	return 0;
}