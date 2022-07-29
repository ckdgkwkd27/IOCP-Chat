#pragma once
#include "Player.h"
#define MAX_PLAYER 65535

class PlayerManager {
public:
	void Init(int size = MAX_PLAYER);
	shared_ptr<Player> NewPlayer();
	void DeletePlayer(shared_ptr<Player> _player);
	shared_ptr<Player> FindPlayerByName(string _name);

private:
	void AddPlayer(shared_ptr<Player> _player);
	void ReturnPlayer(shared_ptr<Player> _player);
	
public:
	map<UINT32, shared_ptr<Player>> AllPlayerInfo;

private:
	recursive_mutex playerLock;
	queue<shared_ptr<Player>> playerPool;
	UINT32 NumOfPlayers = 0;
	UINT32 PlayerIdx = 0;

public:
	UINT32 GetPlayerIdx() { return PlayerIdx; }
	void IncreasePlayerIdx() { PlayerIdx++; }
	void DecreasePlayerIdx() { PlayerIdx--; }
};

extern PlayerManager GPlayerManager;