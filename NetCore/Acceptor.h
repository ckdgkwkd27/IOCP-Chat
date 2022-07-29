#pragma once
#include "SocketUtil.h"
#include "IocpCore.h"
#include "IocpObject.h"
#include "SessionManager.h"
#include "NetAddress.h"

class Acceptor : public IocpObject
{
public:
	Acceptor(int _maxSessionCnt, SessionFactory _factory, NetAddress _netAddr);
	~Acceptor();

	bool Init();
	bool StartAccept();
	void PostAccept(AcceptEvent* _event);

	virtual void Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes = 0);

public:
	virtual HANDLE GetHandle() {return reinterpret_cast<HANDLE>(acceptSocket);}

	shared_ptr<IocpCore> GetIocpCore() { return iocpCore; }

private:
	SOCKET acceptSocket = INVALID_SOCKET;
	vector<AcceptEvent*> acceptEvents;
	shared_ptr<IocpCore> iocpCore;
	NetAddress netAddr;
	SessionFactory factory;
	int maxSessionCnt;
};

