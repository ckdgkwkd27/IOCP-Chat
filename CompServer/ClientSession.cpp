#include "pch.h"
#include "ClientSession.h"
#include "SessionManager.h"
#include "Player.h"

void ClientSession::OnConnected()
{
    GSessionManager.AddSession(static_pointer_cast<ClientSession>(shared_from_this()));
    cout << "[동접자수]: " << GSessionManager.GetUserCnt() << endl;
}

INT32 ClientSession::OnRecv(char* buffer, INT32 len)
{
    INT32 processLen = 0;

    while (true) {
        INT32 dataSize = len - processLen;
        if (dataSize < sizeof(PacketHeader))
            break;

        PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
        if (dataSize < header.size)
            break;

        ClientPacketHandler::HandlePacket(dynamic_pointer_cast<ClientSession>(shared_from_this()), buffer, len);

        processLen += header.size;
    }
    return processLen;
}

void ClientSession::OnSend(INT32 len)
{
    //cout << "OnSend Len = " << len << "\n";
}

void ClientSession::OnDisconnected()
{
    cout << "Disconnected...!" << endl;
    GSessionManager.RemoveSession(static_pointer_cast<ClientSession>(shared_from_this()));
}

