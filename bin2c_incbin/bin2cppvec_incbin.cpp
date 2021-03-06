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


const char* bin2cppVec_srcText = R"(/*
 * generated by bin2cppVec
 * DO NOT MODIFY
 */

#define INCBIN_PREFIX g_
#define INCBIN_STYLE INCBIN_STYLE_SNAKE

#include <incbin.h>

#include <cstdint>
#include <vector>


INCBIN({name}, "{file}");
// This translation unit has now 3 symbols:
// const unsigned char *g_{name}_data;
// const unsigned char *g_{name}_end;
// const unsigned int g_{name}_size;


#ifdef __cplusplus
{extC_open}
#endif // __cplusplus

extern const std::vector<uint8_t> {name}; // for linker happiness
const std::vector<uint8_t> {name}(
	reinterpret_cast<const uint8_t*>(g_{name}_data),
	reinterpret_cast<const uint8_t*>(g_{name}_data) + static_cast<size_t>(g_{name}_size)
);

#ifdef __cplusplus
{extC_close}
#endif // __cplusplus

)";


class Bin2CppVector_IncBin_Writer : public bx::WriterI
{
public:
	Bin2CppVector_IncBin_Writer(bx::WriterI* _writer, const char* _name, const char* _filePath)
		: m_writer(_writer), m_name(_name), m_filePath(_filePath)
	{
	}

	virtual ~Bin2CppVector_IncBin_Writer()
	{
	}

	virtual int32_t write(const void* _data, int32_t _size, bx::Error* /*_err*/ = NULL) BX_OVERRIDE
	{
		return 0;
	}

	void finish()
	{
		bx::writePrintf(m_writer,
						fmt::format(bin2cppVec_srcText,
									fmt::arg("name", m_name),
									fmt::arg("file", m_filePath),
									fmt::arg("extC_open", R"(extern "C" {)"),
									fmt::arg("extC_close", R"(} // extern "C")"))
						  .c_str());
	}

	bx::WriterI* m_writer;
	std::string  m_name;
	std::string  m_filePath;
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

	bx::CrtFileWriter fw;
	if (bx::open(&fw, outFilePath))
	{
		Bin2CppVector_IncBin_Writer writer(&fw, name, filePath);
		writer.finish();
		bx::close(&fw);
	}

	return 0;
}
