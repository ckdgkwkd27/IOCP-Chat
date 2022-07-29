#pragma once
#include "IocpObject.h"

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle();

	bool Register(shared_ptr<IocpObject> iocpObject);
	bool Dispatch(UINT32 timeout = INFINITE);

private:
	HANDLE _iocpHandle = INVALID_HANDLE_VALUE;
};

