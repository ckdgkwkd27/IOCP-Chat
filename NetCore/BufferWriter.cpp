#include "pch.h"
#include "BufferWriter.h"

/*----------------
	BufferWriter
-----------------*/

BufferWriter::BufferWriter()
{

}

BufferWriter::BufferWriter(char* buffer, UINT32 size, UINT32 pos)
	: _buffer(buffer), _size(size), _pos(pos)
{

}

BufferWriter::~BufferWriter()
{

}

bool BufferWriter::Write(void* src, UINT32 len)
{
	if (FreeSize() < len)
		return false;

	::memcpy(&_buffer[_pos], src, len);
	_pos += len;
	return true;
}