#pragma once

/*----------------
	BufferReader
-----------------*/

class BufferReader
{
public:
	BufferReader();
	BufferReader(char* buffer, UINT32 size, UINT32 pos = 0);
	~BufferReader();

	char* Buffer() { return _buffer; }
	UINT32			Size() { return _size; }
	UINT32			ReadSize() { return _pos; }
	UINT32			FreeSize() { return _size - _pos; }

	template<typename T>
	bool			Peek(T* dest) { return Peek(dest, sizeof(T)); }
	bool			Peek(void* dest, UINT32 len);

	template<typename T>
	bool			Read(T* dest) { return Read(dest, sizeof(T)); }
	bool			Read(void* dest, UINT32 len);

	template<typename T>
	BufferReader& operator>>(OUT T& dest);

private:
	char* _buffer = nullptr;
	UINT32			_size = 0;
	UINT32			_pos = 0;
};

template<typename T>
inline BufferReader& BufferReader::operator>>(OUT T& dest)
{
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return *this;
}
