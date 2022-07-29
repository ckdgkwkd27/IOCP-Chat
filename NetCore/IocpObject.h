#pragma once
#include "Include.h"

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes = 0) abstract;
};

