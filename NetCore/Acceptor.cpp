#include "pch.h"
#include "Acceptor.h"

Acceptor::Acceptor(int _maxSessionCnt, SessionFactory _factory, NetAddress _netAddr)
{
	SocketUtil::Init();
	acceptSocket = SocketUtil::CreateSocket();
	assert(acceptSocket != INVALID_SOCKET);

	iocpCore = make_shared<IocpCore>();
	netAddr = _netAddr;
	maxSessionCnt = _maxSessionCnt;
	factory = _factory;
}

Acceptor::~Acceptor()
{
	SocketUtil::CloseSocket(acceptSocket);
}

bool Acceptor::Init()
{
	//家南 可记
	return SocketUtil::Listen(acceptSocket);

}

bool Acceptor::StartAccept()
{
	assert(iocpCore->Register(shared_from_this()));

	assert(SocketUtil::SetLinger(acceptSocket));
	assert(SocketUtil::SetReuseAddr(acceptSocket, true));
	assert(SocketUtil::SetTcpNoDelay(acceptSocket, true));

	bool ret = SocketUtil::Bind(acceptSocket, netAddr);
	if (!ret) {
		cout << "Bind Error " << WSAGetLastError() << endl;
		return false;
	}

	ret = SocketUtil::Listen(acceptSocket);
	if (!ret) {
		cout << "Listen Error " << WSAGetLastError() << endl;
		return false;
	}

	for (INT32 i = 0; i < maxSessionCnt; i++) {
		AcceptEvent* _acceptEvent = new AcceptEvent();
		_acceptEvent->owner = shared_from_this();

		PostAccept(_acceptEvent);
	}

	return true;
}

void Acceptor::PostAccept(AcceptEvent* _event)
{
	auto ss = GSessionManager.CreateSession(iocpCore, factory);
	SOCKET skt = ss->GetSocket();

	_event->sessionRef = ss;
	_event->Init();

	DWORD bytesReceived = 0;
	
	if (false == SocketUtil::AcceptEx(acceptSocket, ss->GetSocket(),
		ss->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&bytesReceived, static_cast<LPOVERLAPPED>(_event)))
	{
		const INT32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			cout << "PostAccept_ERRCODE: " << errCode << endl;
			PostAccept(_event);
		}
	}
}

void Acceptor::Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes)
{
	assert(iocpEvent->GetType() == EventType::ACCEPT);
	AcceptEvent* accptEvent = static_cast<AcceptEvent*>(iocpEvent);
	
	shared_ptr<Session> ss = accptEvent->sessionRef;

	if (false == SocketUtil::SetUpdateAcceptSocket(ss->GetSocket(), acceptSocket))
	{
		PostAccept(accptEvent);
		cout << "UpdateSocket Option Error" << endl;
		return;
	}

	SOCKADDR_IN sockAddress;
	INT32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(ss->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		PostAccept(accptEvent);
		return;
	}

	ss->SetNetAddress(NetAddress(sockAddress));
	ss->ProcessConnect();
	PostAccept(accptEvent);
}
