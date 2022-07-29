#pragma once
#include <string.h>

class RingBuffer
{
public:
	RingBuffer(INT32 bufferSize);
	~RingBuffer();

public:
	void Clean();
	bool OnRead(INT32 numOfBytes);
	bool OnWrite(INT32 numOfBytes);

	char* ReadPos();
	char* WritePos();
	INT32 DataSize();
	INT32 FreeSize();

	char* BufferData() {
		return _buffer.data();
	}

private:
	INT32			_capacity = 0;
	INT32			_bufferSize = 0;
	INT32			_readPos = 0;
	INT32			_writePos = 0;
	vector<char>	_buffer;
};

