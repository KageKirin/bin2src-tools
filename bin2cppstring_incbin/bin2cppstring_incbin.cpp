/*
 * Copyright 2017 Christian Helmich. All rights reserved.
 * Based on work by Branimir Karadzic
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#include <cstdio>
#include <string>
#include <vector>

#include <bx/commandline.h>
#include <bx/crtimpl.h>
#include <bx/string.h>

#include <boost/filesystem.hpp>

class Bin2CppString_IncBin_Writer : public bx::WriterI
{
public:
	Bin2CppString_IncBin_Writer(bx::WriterI* _writer, const char* _name, const char* _filePath)
		: m_writer(_writer), m_name(_name), m_filePath(_filePath)
	{
	}

	virtual ~Bin2CppString_IncBin_Writer()
	{
	}

	virtual int32_t write(const void* _data, int32_t _size, bx::Error* /*_err*/ = NULL) BX_OVERRIDE
	{
		return 0;
	}

	void finish()
	{
		bx::writePrintf(m_writer, "#define INCBIN_PREFIX g_\n");
		bx::writePrintf(m_writer, "#define INCBIN_STYLE INCBIN_STYLE_SNAKE\n");

		bx::writePrintf(m_writer, "#include <incbin.h>\n\n");
		bx::writePrintf(m_writer, "#include <string>\n\n");

		bx::writePrintf(m_writer, "INCBIN(%s, \"%s\");\n", m_name.c_str(), m_filePath.c_str());
		bx::writePrintf(m_writer, "// This translation unit now has three symbols\n");
		bx::writePrintf(m_writer, "// const unsigned char* g_%s_data;\n", m_name.c_str());
		bx::writePrintf(m_writer, "// const unsigned char *g_%s_end;\n", m_name.c_str());
		bx::writePrintf(m_writer, "// const unsigned int g_%s_size;\n\n", m_name.c_str());

		// bx::writePrintf(m_writer, "// Reference in other translation units like this\n");
		// bx::writePrintf(m_writer, "INCBIN_EXTERN(%s);\n", m_name.c_str());
		// bx::writePrintf(m_writer, "// This translation unit now has three extern symbols\n");
		// bx::writePrintf(m_writer, "// extern const unsigned char g_%s_data[];\n", m_name.c_str());
		// bx::writePrintf(m_writer, "// extern const unsigned char *g_%s_end;\n", m_name.c_str());
		// bx::writePrintf(m_writer, "// extern const unsigned int g_%s_size;\n", m_name.c_str());

		bx::writePrintf(m_writer, "extern const std::string %s; // for linker happiness\n", m_name.c_str());
		bx::writePrintf(m_writer,
						"const std::string %s(reinterpret_cast<const char*>(g_%s_data), (size_t)g_%s_end);\n",
						m_name.c_str(),
						m_name.c_str(),
						m_name.c_str());
		bx::writePrintf(m_writer, "\n");
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

	namespace fs		   = boost::filesystem;
	const char* rebasePath = cmdLine.findOption('r');
	fs::path	relPath(filePath);

	if (NULL != rebasePath)
	{
		fs::path rebaseFilePath(rebasePath);
		relPath = fs::relative(relPath, rebaseFilePath);
	}
	filePath = relPath.string().c_str();

	bx::CrtFileWriter fw;
	if (bx::open(&fw, outFilePath))
	{
		Bin2CppString_IncBin_Writer writer(&fw, name, filePath);
		writer.finish();
		bx::close(&fw);
	}

	return 0;
}
