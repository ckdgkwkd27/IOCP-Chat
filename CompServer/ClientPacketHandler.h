#pragma once
#include <memory>
#include <functional>
#include <basetsd.h>
#include <SendBuffer.h>
#include "Player.h"
#include "Room.h"
#include "RoomManager.h"
#include "PlayerManager.h"
using namespace std;

//헤더 분리
class ClientSession;

using ClientSessionRef = shared_ptr<ClientSession>;
using ClientPacketHandlerFunc = function<bool(ClientSessionRef&, char*, INT32)>;

extern ClientPacketHandlerFunc GClientPacketHandler[65535];

enum PacketEnum : UINT16 {
	SC_HELLO = 1,
	CS_HELLO,

	SC_RES_LOGIN,
	CS_REQ_LOGIN,

	SC_RES_ROOM_ENTER,
	CS_REQ_ROOM_ENTER,

	SC_RES_ROOM_CHANGE,
	CS_REQ_ROOM_CHANGE,

	SC_RES_NORMAL_CHAT,
	CS_REQ_NORMAL_CHAT,

	SC_RES_WHISPER_CHAT,
	CS_REQ_WHISPER_CHAT,

	SC_RES_TEST_ROOMCHAT,
	CS_REQ_TEST_ROOMCHAT,

	END
};

bool Handle_Invalid(ClientSessionRef& session, char* buffer, INT32 len);


class ClientPacketHandler {
public:
	static void HandlePacket(ClientSessionRef session, char* buffer, INT32 len);

	static shared_ptr<SendBuffer> Make_SC_RES_NORMAL_CHAT(string msg);

	static void Handle_CS_Hello(ClientSessionRef session, char* buffer, INT32 len);
	static void Handle_CS_REQ_LOGIN(ClientSessionRef session, char* buffer, INT32 len);
	static void Handle_CS_REQ_NORMAL_CHAT(ClientSessionRef session, char* buffer, INT32 len);
	static void Handle_CS_REQ_TEST_ROOMCHAT(ClientSessionRef session, char* buffer, INT32 len);
	static void Handle_CS_REQ_ROOM_CHANGE(ClientSessionRef session, char* buffer, INT32 len);
	static void Handle_CS_REQ_WHISPER_CHAT(ClientSessionRef session, char* buffer, INT32 len);

};