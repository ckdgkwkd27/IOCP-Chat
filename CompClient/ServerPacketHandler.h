#pragma once
#include <BufferReader.h>
#include <BufferWriter.h>
#include <TimeUtil.h>
using namespace std;

//헤더 분리
class ServerSession;

using ServerSessionRef = shared_ptr<ServerSession>;
using ServerPacketHandlerFunc = function<bool(ServerSessionRef&, char*, INT32)>;

extern ServerPacketHandlerFunc GServerPacketHandler[65535];

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

bool Handle_Invalid(ServerSessionRef& session, char* buffer, INT32 len);

class ServerPacketHandler {
public:
	static void HandlePacket(ServerSessionRef session, char* buffer, INT32 len);

	static shared_ptr<SendBuffer> Make_CS_Res_Hello(char* buf, INT32 len);
	static shared_ptr<SendBuffer> Make_CS_REQ_LOGIN(string Id);
	static shared_ptr<SendBuffer> Make_CS_REQ_NORMAL_CHAT(string msg);
	static shared_ptr<SendBuffer> Make_CS_REQ_TEST_ROOMCHAT(string msg);
	static shared_ptr<SendBuffer> Make_CS_REQ_ROOM_CHANGE(UINT32 roomIdx);
	static shared_ptr<SendBuffer> Make_CS_REQ_WHISPER_CHAT(string _name, string msg);

	static void Handle_SC_RES_NORMAL_CHAT(ServerSessionRef session, char* buffer, INT32 len);
	static void Handle_SC_RES_WHISPER_CHAT(ServerSessionRef session, char* buffer, INT32 len);

public:
	//TEMP
	static recursive_mutex tempLock;
};