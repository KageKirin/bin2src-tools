#ifndef U64_C_WRITER_H_INC
#define U64_C_WRITER_H_INC

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


class Bin2C_U64_Writer : public bx::WriterI
{
public:
	Bin2C_U64_Writer(bx::WriterI* _writer, const char* _name) : m_writer(_writer), m_name(_name)
	{
	}

	virtual ~Bin2C_U64_Writer()
	{
	}

	virtual int32_t write(const void* _data, int32_t _size, bx::Error* /*_err*/ = NULL) BX_OVERRIDE
	{
		const char* data = (const char*)_data;
		m_buffer.insert(m_buffer.end(), data, data + _size);
		// add padding
		m_buffer.resize(m_buffer.size() + m_buffer.size() % sizeof(uint64_t), 0);
		return _size;
	}

	virtual void finish()
	{
		// printf("starting finish\n");
		bx::writePrintf(m_writer, "#include <cstdint>\n\n");
		bx::writePrintf(m_writer, "static uint64_t g_%s_data[] = {\n", m_name.c_str());

		const uint64_t* data_begin = reinterpret_cast<uint64_t*>(&*m_buffer.begin());
		const size_t	size	   = m_buffer.size() / sizeof(uint64_t);
		const uint64_t* data_end   = data_begin + size;

		constexpr size_t elemPerLine = 4;
		size_t			 elemOnLine  = 0;

		for (auto it = data_begin; it != data_end; ++it)
		{
			// overhead: 0.5 byte / byte_in_data
			// bx::writePrintf(m_writer, "\t0x%.16llX,", *it);

			// less or equal overhead
			bx::writePrintf(m_writer, "\t0x%llX,", *it);

			if (elemOnLine++ >= elemPerLine)
			{
				// printf("changeline\n");
				bx::writePrintf(m_writer, "\n");
				elemOnLine = 0;
			}
		}

		bx::writePrintf(m_writer, "};\n\n");
		bx::writePrintf(m_writer, "static size_t g_%s_size_u64 = %d;\n", m_name.c_str(), m_buffer.size() / sizeof(uint64_t));
		bx::writePrintf(m_writer, "static size_t g_%s_size = %d;\n\n", m_name.c_str(), m_buffer.size());

		m_buffer.clear();
		// printf("ending finish\n");
	}

	bx::WriterI*				 m_writer;
	std::string					 m_name;
	typedef std::vector<uint8_t> Buffer;
	Buffer						 m_buffer;
};

#endif	//! U64_C_WRITER_H_INC
