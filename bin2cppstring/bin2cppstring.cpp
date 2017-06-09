/*
 * Copyright 2017 Christian Helmich. All rights reserved.
 * Based on work by Branimir Karadzic
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#include <string>
#include <vector>

#include <bx/commandline.h>
#include <bx/crtimpl.h>
#include <bx/string.h>

class Bin2CppStringWriter : public bx::WriterI
{
public:
	Bin2CppStringWriter(bx::WriterI* _writer, const char* _name) : m_writer(_writer), m_name(_name)
	{
	}

	virtual ~Bin2CppStringWriter()
	{
	}

	virtual int32_t write(const void* _data, int32_t _size, bx::Error* /*_err*/ = NULL) BX_OVERRIDE
	{
		const char* data = (const char*)_data;
		m_buffer.insert(m_buffer.end(), data, data + _size);
		return _size;
	}


	void finish()
	{
#define HEX_DUMP_WIDTH 16
#define HEX_DUMP_SPACE_WIDTH 96
#define HEX_DUMP_FORMAT "%-" BX_STRINGIZE(HEX_DUMP_SPACE_WIDTH) "." BX_STRINGIZE(HEX_DUMP_SPACE_WIDTH) "s"
		const uint8_t* data = &m_buffer[0];
		uint32_t	   size = (uint32_t)m_buffer.size();


		bx::writePrintf(m_writer, "#include <string>\n\n");
		bx::writePrintf(m_writer, "extern const std::string %s; // for linker happiness\n", m_name.c_str());

		bx::writePrintf(m_writer, "const std::string %s =\n", m_name.c_str());

		constexpr size_t maxStringSize = 2040;	// 2048, but leaving a of space


		auto it = m_buffer.begin();
		while (it != m_buffer.end())
		{
			bx::writePrintf(m_writer, "std::string(R\"(");

			while (it != m_buffer.end())
			{
				bx::writePrintf(m_writer, "%c", *it++);

				if ((std::distance(m_buffer.begin(), it) % maxStringSize) == 0)
				{
					break;
				}
			}

			bx::writePrintf(m_writer, ")\") +\n");
		}

		bx::writePrintf(m_writer, "\"\";\n");


#undef HEX_DUMP_WIDTH
#undef HEX_DUMP_SPACE_WIDTH
#undef HEX_DUMP_FORMAT

		m_buffer.clear();
	}

	bx::WriterI*				 m_writer;
	std::string					 m_filePath;
	std::string					 m_name;
	typedef std::vector<uint8_t> Buffer;
	Buffer						 m_buffer;
};

void help(const char* _error = NULL)
{
	if (NULL != _error)
	{
		fprintf(stderr, "Error:\n%s\n\n", _error);
	}

	fprintf(stderr,
			"bin2cppstring, binary to C++ std::string\n"
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
			Bin2CppStringWriter writer(&fw, name);
			bx::write(&writer, data, size);
			writer.finish();
			bx::close(&fw);
		}

		free(data);
	}

	return 0;
}
