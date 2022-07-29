#pragma once
#include "NetAddress.h"

class SocketUtil
{
public:
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();
	static void Clear();
	static SOCKET CreateSocket();
	static bool SyncConnect(SOCKET _socket, NetAddress _netAddress);
	static void CloseSocket(SOCKET& s);

	static bool Bind(SOCKET _socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET _socket, UINT16 port);
	static bool Listen(SOCKET _socket, INT32 backlog = SOMAXCONN);

	static bool SetLinger(SOCKET _socket);
	static bool SetReuseAddr(SOCKET _socket, bool flag);
	static bool SetTcpNoDelay(SOCKET _socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

private:
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
};

