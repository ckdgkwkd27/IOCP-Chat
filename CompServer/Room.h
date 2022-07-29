#pragma once
#include "Player.h"

class SendBuffer;
class ClientSession;

class Room
{
public:
	void Init(INT32 _maxUserCnt, INT32 _roomNum);
	void EnterPlayer(shared_ptr<Player> player);
	void LeavePlayer(shared_ptr<Player> player);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer);

private:
	recursive_mutex roomLock;
	map<UINT64, shared_ptr<Player>> _players;
	INT32 maxUserCnt;
	INT32 roomNum;
};

extern Room GRoom;