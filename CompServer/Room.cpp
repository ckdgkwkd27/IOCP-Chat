#include "pch.h"
#include "Room.h"
#include <SendBuffer.h>
#include "ClientSession.h"

Room GRoom;

void Room::Init(INT32 _maxUserCnt, INT32 _roomNum)
{
	maxUserCnt = _maxUserCnt;
	roomNum = _roomNum;
}

void Room::EnterPlayer(shared_ptr<Player> player)
{
	lock_guard<recursive_mutex> guard(roomLock);
	_players[player->playerId] = player;
}

void Room::LeavePlayer(shared_ptr<Player> player)
{
	lock_guard<recursive_mutex> guard(roomLock);
	_players.erase(player->playerId);
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	lock_guard<recursive_mutex> guard(roomLock);
	for (auto& p : _players)
		p.second->ownerSession->Send(sendBuffer);
}
