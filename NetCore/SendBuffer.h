#pragma once

class SendBuffer : enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer();
	SendBuffer(INT32 bufferSize);
	~SendBuffer();

	char* Buffer() { return _buffer.data(); }
	INT32 WriteSize() { return _writeSize; }
	INT32 Capacity() { return static_cast<INT32>(_buffer.size()); }
	INT32 AllocSize() { return _allocSize; }

	void Clear();
	void CopyData(void* data, INT32 len);
	void Close(INT32 dataSize);

private:
	vector<char>	_buffer;
	INT32			_writeSize = 0;
	INT32			_allocSize = 0;
};

