#pragma once

class Session;
class IocpObject;
class SendBuffer;

enum class EventType : UINT8 {
	ACCEPT,
	CONNECT,
	DISCONNECT,
	SEND,
	RECV
};

class IocpEvent : public OVERLAPPED {
public:
	IocpEvent(EventType _type) : eventType(_type)
	{
	}

	void Init() {
		OVERLAPPED::hEvent = 0;
		OVERLAPPED::Internal = 0;
		OVERLAPPED::InternalHigh = 0;
		OVERLAPPED::Offset = 0;
		OVERLAPPED::OffsetHigh = 0;
	}

	EventType GetType() {
		return eventType;
	}

public:
	std::shared_ptr<IocpObject> owner;

private:
	EventType eventType;

};

class AcceptEvent : public IocpEvent {
public:
	AcceptEvent() : IocpEvent(EventType::ACCEPT) {}

public:
	std::shared_ptr<Session> sessionRef;

};

class ConnectEvent : public IocpEvent {
public:
	ConnectEvent() : IocpEvent(EventType::CONNECT) {}
};

class DisconnectEvent : public IocpEvent {
public:
	DisconnectEvent() : IocpEvent(EventType::DISCONNECT) {}
};

class SendEvent : public IocpEvent {
public:
	SendEvent() : IocpEvent(EventType::SEND) 
	{
	}

public:
	std::shared_ptr<SendBuffer> sendBuffer;
};

class RecvEvent : public IocpEvent {
public:
	RecvEvent() : IocpEvent(EventType::RECV) {}
};