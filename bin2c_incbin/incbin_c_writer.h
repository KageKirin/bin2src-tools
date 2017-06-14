#ifndef INCBIN_C_WRITER_H_INC
#define INCBIN_C_WRITER_H_INC

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

const char* bin2c_srcText = R"(/*
 * generated by bin2c
 * DO NOT MODIFY
 */

#define INCBIN_PREFIX g_
#define INCBIN_STYLE INCBIN_STYLE_SNAKE

#include <incbin.h>

// Reference from outside using:
// INCBIN_EXTERN({name});
// This translation unit has now 3 external symbols:
// extern const unsigned char *g_{name}_data;
// extern const unsigned char *g_{name}_end;
// extern const unsigned int g_{name}_size;

INCBIN({name}, "{file}");
// This translation unit has now 3 symbols:
// const unsigned char *g_{name}_data;
// const unsigned char *g_{name}_end;
// const unsigned int g_{name}_size;

)";

class Bin2C_IncBin_Writer : public bx::WriterI
{
public:
	Bin2C_IncBin_Writer(bx::WriterI* _writer, const char* _name, const char* _filePath)
		: m_writer(_writer), m_name(_name), m_filePath(_filePath)
	{
	}

	virtual ~Bin2C_IncBin_Writer()
	{
	}

	virtual int32_t write(const void* _data, int32_t _size, bx::Error* /*_err*/ = NULL) BX_OVERRIDE
	{
		return 0;
	}

	void finish()
	{
		bx::writePrintf(m_writer,
						fmt::format(bin2c_srcText,
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

#endif	//! INCBIN_C_WRITER_H_INC