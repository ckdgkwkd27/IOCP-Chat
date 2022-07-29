#include "pch.h"
#include "SessionManager.h"
#include "IocpCore.h"

SessionManager GSessionManager;

shared_ptr<Session> SessionManager::CreateSession(shared_ptr<IocpCore> _iocpCore, SessionFactory _factory)
{
	auto _session = _factory();
	_iocpCore->Register(_session);
	return _session;
}

void SessionManager::AddSession(shared_ptr<Session> session)
{
	lock_guard<recursive_mutex> guard(sessionLock);
	_sessions.insert(session);
	userCnt++;
}

void SessionManager::RemoveSession(shared_ptr<Session> session)
{
	lock_guard<recursive_mutex> guard(sessionLock);
	_sessions.erase(session);
	userCnt--;
}

void SessionManager::BroadCast(shared_ptr<SendBuffer> sendBuffer)
{
	lock_guard<recursive_mutex> guard(sessionLock);
	for (shared_ptr<Session> session : _sessions) {
		session->Send(sendBuffer);
	}
}

UINT32 SessionManager::GetUserCnt()
{
	return userCnt;
}


