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
		// �ּ��� ����� �Ľ��� �� �־�� �Ѵ�
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));

		// ����� ��ϵ� ��Ŷ ũ�⸦ �Ľ��� �� �־�� �Ѵ�
		if (dataSize < header.size)
			break;

		// ��Ŷ ���� ����
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
