#pragma once
#include "SocketUtil.h"
#include "IocpCore.h"
#include "IocpObject.h"
#include "SessionManager.h"

class Connector
{
public:
	Connector(int _maxSessionCnt, SessionFactory _factory, NetAddress _netAddr);
	~Connector();

	bool StartConnect();

public:
	shared_ptr<IocpCore> GetIocpCore() { return iocpCore; }

private:
	SOCKET connSocket = INVALID_SOCKET;
	atomic<bool> isConnected = false;
	shared_ptr<IocpCore> iocpCore;
	NetAddress netAddr;
	int maxSessionCnt;
	SessionFactory factory;
};

