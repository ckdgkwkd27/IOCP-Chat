#pragma once

class ClientSession;

class Player
{
public:
	UINT32 playerId = 0;
	string name;
	//enum player_type
	shared_ptr<ClientSession> ownerSession;
};

using PlayerRef = shared_ptr<Player>;
