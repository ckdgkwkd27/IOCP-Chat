#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

recursive_mutex ServerPacketHandler::tempLock;

bool Handle_Invalid(ServerSessionRef& session, char* buffer, INT32 len)
{
	return false;
}


void ServerPacketHandler::HandlePacket(ServerSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);
	
	PacketHeader header;
	br >> header;

	//cout << "header id: " << header.id << endl;
	//cout << "header size: " << header.size << endl;

	switch (header.id)
	{
	case SC_RES_NORMAL_CHAT:
		Handle_SC_RES_NORMAL_CHAT(session, buffer, len);
		break;

	case SC_RES_WHISPER_CHAT:
		Handle_SC_RES_WHISPER_CHAT(session, buffer, len);
		break;

	default:
		break;
	}
}

shared_ptr<SendBuffer> ServerPacketHandler::Make_CS_Res_Hello(char* buf, INT32 len)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);

	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());

	PacketHeader* header = bw.Reserve<PacketHeader>();

	int a[5] = { 1,2,3,4,5 };
	int sz = 5;
	bw << sz;

	for (int i = 0; i < sz; i++)
		bw << a[i];

	bw.Write(buf, len);

	header->size = bw.WriteSize();
	header->id = CS_HELLO;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}

shared_ptr<SendBuffer> ServerPacketHandler::Make_CS_REQ_LOGIN(string Id)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << Id;
	ChatId = Id;

	header->size = bw.WriteSize();
	header->id = CS_REQ_LOGIN;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}

shared_ptr<SendBuffer> ServerPacketHandler::Make_CS_REQ_NORMAL_CHAT(string msg)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	int sz = msg.size();
	bw << sz;

	for (int i = 0; i < sz; i++) {
		bw << msg[i];
	}

	header->size = bw.WriteSize();
	header->id = CS_REQ_NORMAL_CHAT;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}

shared_ptr<SendBuffer> ServerPacketHandler::Make_CS_REQ_TEST_ROOMCHAT(string msg)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	int sz = msg.size();
	bw << sz;

	for (int i = 0; i < sz; i++) {
		bw << msg[i];
	}

	header->size = bw.WriteSize();
	header->id = CS_REQ_TEST_ROOMCHAT;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}

shared_ptr<SendBuffer> ServerPacketHandler::Make_CS_REQ_ROOM_CHANGE(UINT32 roomIdx)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << roomIdx;

	header->size = bw.WriteSize();
	header->id = CS_REQ_ROOM_CHANGE;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}

shared_ptr<SendBuffer> ServerPacketHandler::Make_CS_REQ_WHISPER_CHAT(string _name, string msg)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	UINT32 nameSz = _name.size();
	bw << nameSz;

	for (UINT32 i = 0; i < nameSz; i++) {
		bw << _name[i];
	}

	UINT32 msgSz = msg.size();
	bw << msgSz;

	for (UINT32 i = 0; i < msgSz; i++) {
		bw << msg[i];
	}

	header->size = bw.WriteSize();
	header->id = CS_REQ_WHISPER_CHAT;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}

void ServerPacketHandler::Handle_SC_RES_NORMAL_CHAT(ServerSessionRef session, char* buffer, INT32 len) {
	//cout << "Handle_SC_RES_NORMAL_CHAT" << endl;
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	string msg;
	int sz;
	br >> sz;
	msg.resize(sz);

	for (int i = 0; i < sz; i++) {
		char c;
		br >> c;
		msg[i] = c;
	}

	tempLock.lock();
	string base = "[" + TimeUtil::CurrentDateTime() + "] ";

	cout << base + ChatId + ": " << msg << endl;

	tempLock.unlock();
}

void ServerPacketHandler::Handle_SC_RES_WHISPER_CHAT(ServerSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	string msg;
	int sz;
	br >> sz;
	msg.resize(sz);

	for (int i = 0; i < sz; i++) {
		char c;
		br >> c;
		msg[i] = c;
	}

	tempLock.lock();
	string base = "[" + TimeUtil::CurrentDateTime() + "] ";

	cout << base + ChatId + ": " << msg << endl;

	tempLock.unlock();
}
