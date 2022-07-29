#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer()
{
	_buffer.resize(4096);
	_allocSize = 4096;
}

SendBuffer::SendBuffer(INT32 bufferSize)
{
	_buffer.resize(bufferSize);
	_allocSize = bufferSize;
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Clear()
{
	_buffer.clear();
}

void SendBuffer::CopyData(void* data, INT32 len)
{
	assert(Capacity() >= len);
	::memcpy(_buffer.data(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(INT32 dataSize)
{
	assert(_allocSize >= dataSize);
	_writeSize = dataSize;
}
