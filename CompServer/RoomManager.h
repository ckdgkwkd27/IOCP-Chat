#pragma once
#include "Room.h"
#include "Player.h"

class RoomManager
{
public:
	void Init(const UINT32 _roomCnt, const UINT32 _maxUserCnt);
	void Enter(shared_ptr<Player> _player, INT32 _roomId);
	void Leave(shared_ptr<Player> _player, INT32 _roomId);
	void RoomChat(shared_ptr<Player> _player, shared_ptr<SendBuffer> sendBuffer);
	void TestRoomChat(shared_ptr<Player> _player, shared_ptr<SendBuffer> sendBuffer);
	void AllPlayerAnnounce(shared_ptr<SendBuffer> sendBuffer);

public:
	shared_ptr<Room> GetRoomByID(UINT32 roomNum) { 
		if (roomNum >= maxRoomCnt)
			return nullptr;
		return roomPool[roomNum]; 
	}
	INT32 GetPlayerRoom(INT32 _playerId) { return playerRoomMap[_playerId]; }

private:
	map<INT32, INT32> playerRoomMap;
	vector<shared_ptr<Room>> roomPool;
	UINT32 maxRoomCnt;
};

extern RoomManager GRoomManager;