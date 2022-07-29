#pragma once

/*----------------
	BufferWriter
-----------------*/

class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(char* buffer, UINT32 size, UINT32 pos = 0);
	~BufferWriter();

	char* Buffer() { return _buffer; }
	UINT32			Size() { return _size; }
	UINT32			WriteSize() { return _pos; }
	UINT32			FreeSize() { return _size - _pos; }

	template<typename T>
	bool			Write(T* src) { return Write(src, sizeof(T)); }
	bool			Write(void* src, UINT32 len);

	template<typename T>
	T* Reserve();

	template<typename T>
	BufferWriter& operator<<(T&& src);


private:
	char* _buffer = nullptr;
	UINT32			_size = 0;
	UINT32			_pos = 0;
};

template<typename T>
T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
		return nullptr;

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return ret;
}

template<typename T>
BufferWriter& BufferWriter::operator<<(T&& src)
{
	using DataType = std::remove_reference_t<T>;
	*reinterpret_cast<DataType*>(&_buffer[_pos]) = std::forward<DataType>(src);
	_pos += sizeof(T);
	return *this;
}

