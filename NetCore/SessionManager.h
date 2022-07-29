#pragma once
#include "Session.h"

class IocpCore;
class ServerSession;
class ClientSession;

class SessionManager
{
private:
	set<shared_ptr<Session>> _sessions;
	recursive_mutex sessionLock;
	UINT32 userCnt = 0;

public:
	shared_ptr<Session> CreateSession(shared_ptr<IocpCore> _iocpCore, SessionFactory _factory);
	void AddSession(shared_ptr<Session> session);
	void RemoveSession(shared_ptr<Session> session);
	void BroadCast(shared_ptr<SendBuffer> sendBuffer);
	UINT32 GetUserCnt();
};

extern SessionManager GSessionManager;

