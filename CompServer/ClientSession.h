#pragma once
#include "Session.h"
#include "ClientPacketHandler.h"

class SessionManager;
class Player;

//¼­¹ö
class ClientSession : public Session
{
public:
	virtual void OnConnected();
	virtual INT32 OnRecv(char* buffer, INT32 len);
	virtual void OnSend(INT32 len);
	virtual void OnDisconnected();

public:
	shared_ptr<Player> currentPlayer;
};

