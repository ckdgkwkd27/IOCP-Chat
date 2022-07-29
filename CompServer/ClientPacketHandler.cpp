#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include <BufferWriter.h>
#include <BufferReader.h>

bool Handle_Invalid(ClientSessionRef& session, char* buffer, INT32 len)
{
	return false;
}

void ClientPacketHandler::HandlePacket(ClientSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	//cout << "header id: " << header.id << endl;
	//cout << "header size: " << header.size << endl;

	switch (header.id)
	{
	case CS_HELLO:
		Handle_CS_Hello(session, buffer, len);
		break;

	case CS_REQ_LOGIN:
		Handle_CS_REQ_LOGIN(session, buffer, len);
		break;

	case CS_REQ_NORMAL_CHAT:
		Handle_CS_REQ_NORMAL_CHAT(session, buffer, len);
		break;

	case CS_REQ_ROOM_CHANGE:
		Handle_CS_REQ_ROOM_CHANGE(session, buffer, len);
		break;

	case CS_REQ_WHISPER_CHAT:
		Handle_CS_REQ_WHISPER_CHAT(session, buffer, len);
		break;

	case CS_REQ_TEST_ROOMCHAT:
		Handle_CS_REQ_TEST_ROOMCHAT(session, buffer, len);
		break;

	default:
		break;
	}
}

void ClientPacketHandler::Handle_CS_Hello(ClientSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	int sz;
	int a[5];

	br >> sz;
	for (int i = 0; i < sz; i++) {
		int temp;
		br >> temp;
		a[i] = temp;
	}

	char msg[512];
	br.Read(msg, sizeof(msg));

	for (int i = 0; i < sz; i++)
		printf("a[%d]: %d\n", i, a[i]);

	cout << msg << endl;
	printf("\n");
}

void ClientPacketHandler::Handle_CS_REQ_LOGIN(ClientSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;
	
	string Id;
	br >> Id;

	cout << "ID: " << Id << endl;

	shared_ptr<Player> player = GPlayerManager.NewPlayer();
	player->name = Id;
	player->playerId = GPlayerManager.GetPlayerIdx();
	GPlayerManager.IncreasePlayerIdx();
	player->ownerSession = session;

	session->currentPlayer = player;

	//GRoom.Enter(player);
	GRoomManager.Enter(player, 0);
}

void ClientPacketHandler::Handle_CS_REQ_NORMAL_CHAT(ClientSessionRef session, char* buffer, INT32 len)
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

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* writeHeader = bw.Reserve<PacketHeader>();

	INT32 writeSize = msg.size();
	bw << writeSize;

	for (int i = 0; i < writeSize; i++) {
		bw << msg[i];
	}

	writeHeader->size = bw.WriteSize();
	writeHeader->id = SC_RES_NORMAL_CHAT;

	sendBuffer->Close(bw.WriteSize());

	GRoomManager.RoomChat(session->currentPlayer, sendBuffer);
}

void ClientPacketHandler::Handle_CS_REQ_TEST_ROOMCHAT(ClientSessionRef session, char* buffer, INT32 len)
{
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

		shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
		BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
		PacketHeader* writeHeader = bw.Reserve<PacketHeader>();

		INT32 writeSize = msg.size();
		bw << writeSize;

		for (int i = 0; i < writeSize; i++) {
			bw << msg[i];
		}

		writeHeader->size = bw.WriteSize();
		writeHeader->id = SC_RES_NORMAL_CHAT;

		sendBuffer->Close(bw.WriteSize());

		GRoomManager.TestRoomChat(session->currentPlayer, sendBuffer);
	}
}

void ClientPacketHandler::Handle_CS_REQ_ROOM_CHANGE(ClientSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	UINT32 roomIdx;
	br >> roomIdx;

	shared_ptr<Player> _player = session->currentPlayer;
	GRoomManager.Leave(_player, GRoomManager.GetPlayerRoom(_player->playerId));
	GRoomManager.Enter(_player, roomIdx);
	cout << "Enter to Room: " << roomIdx << endl;
}

void ClientPacketHandler::Handle_CS_REQ_WHISPER_CHAT(ClientSessionRef session, char* buffer, INT32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	string playerName;
	int nameSz;
	br >> nameSz;
	playerName.resize(nameSz);

	for (int i = 0; i < nameSz; i++) {
		char c;
		br >> c;
		playerName[i] = c;
	}

	string msg;
	int sz;
	br >> sz;
	msg.resize(sz);

	for (int i = 0; i < sz; i++) {
		char c;
		br >> c;
		msg[i] = c;
	}

	cout << "Whisper (TO)" << playerName << ": " << msg << endl;

	PlayerRef _player = GPlayerManager.FindPlayerByName(playerName);
	if (_player == nullptr) 
		return;

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* writeHeader = bw.Reserve<PacketHeader>();

	INT32 writeSize = msg.size();
	bw << writeSize;

	for (int i = 0; i < writeSize; i++) {
		bw << msg[i];
	}

	writeHeader->size = bw.WriteSize();
	writeHeader->id = SC_RES_WHISPER_CHAT;

	sendBuffer->Close(bw.WriteSize());

	_player->ownerSession->Send(sendBuffer);
}


////////////////////////////////////////////////////////////////////////////
//Make
////////////////////////////////////////////////////////////////////////////
shared_ptr<SendBuffer> Make_SC_RES_NORMAL_CHAT(string msg) {
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* writeHeader = bw.Reserve<PacketHeader>();

	int sz = msg.size();
	bw << sz;

	for (int i = 0; i < sz; i++) {
		bw << msg[i];
	}

	writeHeader->size = bw.WriteSize();
	writeHeader->id = SC_RES_NORMAL_CHAT;

	sendBuffer->Close(bw.WriteSize());
	return sendBuffer;
}