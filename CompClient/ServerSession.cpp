#include "pch.h"
#include "ServerSession.h"
#include <SessionManager.h>

bool GConnected = false;
string ChatId = "";

ServerSession::~ServerSession() {
    cout << "~ServerSession()" << endl;
}

void ServerSession::OnConnected()
{
    cout << "OnConnected..!" << endl;
	GSessionManager.AddSession(static_pointer_cast<ServerSession>(shared_from_this()));

	GConnected = true;
}

INT32 ServerSession::OnRecv(char* buffer, INT32 len)
{
	INT32 processLen = 0;
	
	while (true)
	{
		INT32 dataSize = len - processLen;
		// 최소한 헤더는 파싱할 수 있어야 한다
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));

		// 헤더에 기록된 패킷 크기를 파싱할 수 있어야 한다
		if (dataSize < header.size)
			break;

		// 패킷 조립 성공
		ServerPacketHandler::HandlePacket(dynamic_pointer_cast<ServerSession>(shared_from_this()), &buffer[processLen], header.size);

		processLen += header.size;
	}

	return processLen;
}

void ServerSession::OnSend(INT32 len)
{
	//cout << "OnSend Len = " << len << "\n";
}

void ServerSession::OnDisconnected()
{
	GSessionManager.RemoveSession(static_pointer_cast<ServerSession>(shared_from_this()));
	GConnected = false;
}
