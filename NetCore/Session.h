#pragma once
#include "RingBuffer.h"
#include "SendBuffer.h"
#include "SocketUtil.h"
#include "NetAddress.h"
#include "IocpObject.h"

class Session : public IocpObject
{
public:
	Session();
	~Session();

	SOCKET GetSocket() { return _socket; }	
	void SetNetAddr(NetAddress netAddress) { _netAddress = netAddress; }
	
	void Send(shared_ptr<SendBuffer> sendBuffer); // <-- ÀÌÈÄ SendBuffer·Î

	void PostSend();
	void PostRecv();
	bool PostConnect();
	bool PostDisconnect();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(INT32 numOfBytes);
	void ProcessSend(INT32 numOfBytes);
	
public:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddress = {};
	RingBuffer _recvBuffer;

private:
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
	SendEvent			_sendEvent;

	atomic<bool> _isConnected = false;
	atomic<bool> _sendReserved = false;
	recursive_mutex sendLock;
	shared_ptr<SendBuffer> _sendBuffer; 

public:
	virtual HANDLE GetHandle() { return reinterpret_cast<HANDLE>(_socket); }
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	virtual void Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes = 0) override;

	char msg[100] = { 0, };

	virtual void		OnConnected() { }
	virtual INT32		OnRecv(char* buffer, INT32 len) { return len; }
	virtual void		OnSend(INT32 len) { }
	virtual void		OnDisconnected() { }
};


struct PacketHeader {
	UINT16 size;
	UINT16 id;
};

