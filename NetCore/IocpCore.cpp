#include "pch.h"
#include "IocpCore.h"

IocpCore::IocpCore()
{
    _iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    assert(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
    CloseHandle(_iocpHandle);
}

HANDLE IocpCore::GetHandle()
{
    return _iocpHandle;
}

bool IocpCore::Register(shared_ptr<IocpObject> iocpObject)
{
    return CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, 0, 0);
}

bool IocpCore::Dispatch(UINT32 timeout)
{
    DWORD numOfBytes = 0;
    ULONG_PTR key;
    IocpEvent* iocpEvent = nullptr;

    if (GetQueuedCompletionStatus(_iocpHandle, &numOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeout)) 
    {
        shared_ptr<IocpObject> iocpObject = iocpEvent->owner;
        iocpObject->Dispatch(iocpEvent, numOfBytes);
    }

    else {
        INT32 errCode = WSAGetLastError();
        switch (errCode)
        {
        case WAIT_TIMEOUT:
            return false;
        default:
            shared_ptr<IocpObject> iocpObject = iocpEvent->owner;
            iocpObject->Dispatch(iocpEvent, numOfBytes);
            break; 
        }
    }

    return true;
}
