#include "pch.h"
#include "PlayerManager.h"

PlayerManager GPlayerManager;

void PlayerManager::Init(int size)
{
	for (int i = 0; i < size; i++) {
		shared_ptr<Player> _player = make_shared<Player>();
		playerPool.push(_player);
	}
	NumOfPlayers = 0;
}

void PlayerManager::AddPlayer(shared_ptr<Player> _player)
{
	AllPlayerInfo.insert({ PlayerIdx, _player });
}

void PlayerManager::DeletePlayer(shared_ptr<Player> _player)
{
	lock_guard<recursive_mutex> guard(playerLock);
	UINT32 _playerId = _player->playerId;
	//auto it = AllPlayerInfo.find(_playerId);
	AllPlayerInfo.erase(_playerId);
	ReturnPlayer(_player);
}

shared_ptr<Player> PlayerManager::FindPlayerByName(string _name)
{
	string _playerId;

	for (auto& i : AllPlayerInfo) {
		if (i.second->name == _name) {
			_playerId = i.first;
			return i.second;
		}
	}
	return nullptr;
}

shared_ptr<Player> PlayerManager::NewPlayer()
{
	lock_guard<recursive_mutex> guard(playerLock);
	shared_ptr<Player> _player = playerPool.front();
	playerPool.pop();
	AddPlayer(_player);

	return _player;
}

void PlayerManager::ReturnPlayer(shared_ptr<Player> _player)
{
	playerPool.push(_player);
}

