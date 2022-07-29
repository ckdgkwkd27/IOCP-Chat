#include "pch.h"
#include "RoomManager.h"

RoomManager GRoomManager;

void RoomManager::Init(const UINT32 _roomCnt, const UINT32 _maxUserCnt)
{
	maxRoomCnt = _roomCnt;

	for (UINT32 i = 0; i < maxRoomCnt; i++) {
		shared_ptr<Room> _room = make_shared<Room>();
		_room->Init(_maxUserCnt, i);

		roomPool.emplace_back(_room);
	}
}

void RoomManager::Enter(shared_ptr<Player> _player, INT32 _roomId)
{
	auto _room = GetRoomByID(_roomId);
	if (_room != nullptr)
		_room->EnterPlayer(_player);

	playerRoomMap.insert({ _player->playerId, _roomId });
}

void RoomManager::Leave(shared_ptr<Player> _player, INT32 _roomId)
{
	auto _room = GetRoomByID(_roomId);
	if (_room != nullptr)
		_room->LeavePlayer(_player);

	playerRoomMap.erase(_player->playerId);
}

void RoomManager::RoomChat(shared_ptr<Player> _player, shared_ptr<SendBuffer> sendBuffer)
{
	INT32 _roomId = playerRoomMap[_player->playerId];
	auto _room = GetRoomByID(_roomId);

	_room->Broadcast(sendBuffer);
}

void RoomManager::AllPlayerAnnounce(shared_ptr<SendBuffer> sendBuffer)
{
	for (int i = 0; i < roomPool.size(); i++) {
		roomPool[i]->Broadcast(sendBuffer);
	}
}

void RoomManager::TestRoomChat(shared_ptr<Player> _player, shared_ptr<SendBuffer> sendBuffer)
{
	auto _room = GetRoomByID(0);
	_room->Broadcast(sendBuffer);
}