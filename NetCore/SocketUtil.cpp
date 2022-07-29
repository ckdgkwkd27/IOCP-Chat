#include "pch.h"
#include "SocketUtil.h"

LPFN_CONNECTEX		SocketUtil::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtil::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtil::AcceptEx = nullptr;

void SocketUtil::Init()
{
	WSADATA wsaData;

	assert(::WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);

	SOCKET dummySocket = CreateSocket();
	DWORD bytes = 0;
	assert(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	assert(dummySocket != SOCKET_ERROR);

	CloseSocket(dummySocket);
}

bool SocketUtil::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

void SocketUtil::Clear() {
	WSACleanup();
}

SOCKET SocketUtil::CreateSocket() {
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtil::SyncConnect(SOCKET _socket, NetAddress _netAddress) {
	SOCKADDR_IN sockin = _netAddress.GetSockAddr();
	sockaddr* _sockAddr = reinterpret_cast<sockaddr*>(&sockin);
	return 0 == ::connect(_socket, _sockAddr, sizeof(sockin));
}

void SocketUtil::CloseSocket(SOCKET& s) {
	if (s != INVALID_SOCKET)
		closesocket(s);
	s = INVALID_SOCKET;
}

bool SocketUtil::Bind(SOCKET _socket, NetAddress netAddr)
{
	return SOCKET_ERROR !=::bind(_socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN));
}

bool SocketUtil::BindAnyAddress(SOCKET _socket, UINT16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(_socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));
}

bool SocketUtil::Listen(SOCKET _socket, INT32 backlog)
{
	return SOCKET_ERROR != ::listen(_socket, backlog);
}

bool SocketUtil::SetLinger(SOCKET _socket) {
	LINGER option;
	option.l_linger = 0;
	option.l_onoff = 0;
	return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&option), 
		sizeof(option));
}

bool SocketUtil::SetReuseAddr(SOCKET _socket, bool flag)
{
	return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&flag),
		sizeof(flag));
}

bool SocketUtil::SetTcpNoDelay(SOCKET _socket, bool flag)
{
	return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, TCP_NODELAY, reinterpret_cast<char*>(&flag),
		sizeof(flag));
}


bool SocketUtil::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&listenSocket),
		sizeof(listenSocket));
}

