#include "pch.h"
#include "Connector.h"

Connector::Connector(int _maxSessionCnt, SessionFactory _factory, NetAddress _netAddr)
{
	SocketUtil::Init();
	connSocket = SocketUtil::CreateSocket();
	assert(connSocket != INVALID_SOCKET);

	iocpCore = make_shared<IocpCore>();
	netAddr = _netAddr;
	maxSessionCnt = _maxSessionCnt;
	factory = _factory;
}

Connector::~Connector()
{
	SocketUtil::CloseSocket(connSocket);
}

bool Connector::StartConnect()
{
	for (INT32 i = 0; i < maxSessionCnt; i++) {
		shared_ptr<Session> session = GSessionManager.CreateSession(iocpCore, factory);
		session->SetNetAddr(netAddr);
		if (session->PostConnect() == false)
			return false;
	}

	return true;
}
