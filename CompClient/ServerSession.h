#pragma once
#include "Session.h"
#include "ServerPacketHandler.h"

class SessionManager;

//Å¬¶ó
class ServerSession : public Session
{
public:
	~ServerSession();

public:
	virtual void OnConnected();
	virtual INT32 OnRecv(char* buffer, INT32 len);
	virtual void OnSend(INT32 len);
	virtual void OnDisconnected();
};

extern bool GConnected;
extern string ChatId;
