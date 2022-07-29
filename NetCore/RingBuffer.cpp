#include "pch.h"
#include "RingBuffer.h"

RingBuffer::RingBuffer(INT32 bufferSize)
	: _bufferSize(bufferSize)
{
	_capacity = bufferSize * 10;
	_buffer.resize(_capacity);
}

RingBuffer::~RingBuffer()
{

}

void RingBuffer::Clean()
{
	INT32 dataSize = DataSize();
	if (dataSize == 0) {
		_readPos = _writePos = 0;
	}
	
	else {
		if (FreeSize() < _bufferSize) {
			::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}
}

bool RingBuffer::OnRead(INT32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RingBuffer::OnWrite(INT32 numOfBytes)
{
	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}

char* RingBuffer::ReadPos()
{
	return &_buffer[_readPos];
}

char* RingBuffer::WritePos()
{
	return &_buffer[_writePos];
}

INT32 RingBuffer::DataSize()
{
	return _writePos - _readPos;
}

INT32 RingBuffer::FreeSize()
{
	return _capacity - _writePos;
}
