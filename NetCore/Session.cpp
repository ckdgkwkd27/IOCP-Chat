#include "pch.h"
#include "Session.h"

Session::Session()
	: _recvBuffer(65536)
{
	//strcpy_s(msg, 100, "Hello World");
	_socket = SocketUtil::CreateSocket();

	ClearFileLog("Logger.txt");
}


Session::~Session()
{
	closesocket(_socket);
}

void Session::Send(shared_ptr<SendBuffer> sendBuffer)
{
	if (_isConnected == false)
		return;

	bool reserveSend = false;

	{
		lock_guard<std::recursive_mutex> guard(sendLock);
		if (_sendReserved.exchange(true) == false)
			reserveSend = true;
		_sendBuffer = sendBuffer;
	}

	if (reserveSend)
		PostSend();
}

void Session::PostSend()
{ 
	if (_isConnected == false)
		return;
	
	//this_thread::sleep_for(10ms);
	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();
	assert(_sendEvent.owner != nullptr);

	{
		std::lock_guard<std::recursive_mutex> guard(sendLock);
		_sendEvent.sendBuffer = _sendBuffer;
	}

	DWORD numOfBytes = 0;
	WSABUF wsabuf;
	wsabuf.buf = _sendEvent.sendBuffer->Buffer();
	wsabuf.len = static_cast<LONG>(_sendEvent.sendBuffer->WriteSize());

	if (SOCKET_ERROR == WSASend(_socket, &wsabuf, (DWORD)1, &numOfBytes, 0, &_sendEvent, nullptr))
	{
		INT32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			cout << "Send Error: " << errCode << endl;
			_sendEvent.owner = nullptr;
			_sendEvent.sendBuffer->Clear();
			_sendReserved.store(false);
		}
	}
}

void Session::PostRecv()
{
	if (!_isConnected)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = _recvBuffer.WritePos();
	wsaBuf.len = _recvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, &_recvEvent, nullptr))
	{
		INT32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			cout << "Recv Error: " << errCode << endl;
			_recvEvent.owner = nullptr;
		}
	}
}

bool Session::PostConnect()
{
	if (_isConnected)
		return false;

	//클라이언트 타입이 아니라면 종료

	if (SocketUtil::SetReuseAddr(_socket, true) == false)
		return false;

	if (SocketUtil::BindAnyAddress(_socket, 0) == false)
		return false;

	_connectEvent.Init();
	_connectEvent.owner = shared_from_this(); // ADD_REF

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = _netAddress.GetSockAddr();

	if (false == SocketUtil::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), 
		sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		INT32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			cout << "errorCode: " << errorCode << endl;
			_connectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

bool Session::PostDisconnect()
{
	_disconnectEvent.Init();
	_disconnectEvent.owner = shared_from_this();
	_isConnected.store(false);

	if (false == SocketUtil::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0)) {
		INT32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING) {
			_disconnectEvent.owner = nullptr;
			return false;
		}
	}

	return true;
}


//============================================================================
//Process
//============================================================================

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr;
	_isConnected.store(true);

	OnConnected();
	PostRecv();
}

void Session::ProcessDisconnect()
{
	_disconnectEvent.owner = nullptr;

	OnDisconnected();
}

void Session::ProcessRecv(INT32 numOfBytes)
{
	_recvEvent.owner = nullptr;

	if (numOfBytes == 0) {
		PostDisconnect();
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false) {
		PostDisconnect();
		return;
	}

	INT32 dataSize = _recvBuffer.DataSize();

	INT32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
	
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false) {
		PostDisconnect();
		return;
	}
	
	_recvBuffer.Clean();
	PostRecv();
}

void Session::ProcessSend(INT32 numOfBytes)
{
	//printf("_sendEvent Buffer: %s\n", _sendEvent.buffer);

	_sendEvent.owner = nullptr;
	_sendEvent.sendBuffer->Clear();
	//_sendBuffer->Clear();

	if (numOfBytes == 0) {
		PostDisconnect();
		return;
	}

	OnSend(numOfBytes);

	lock_guard<recursive_mutex> guard(sendLock);
	_sendReserved.store(false);

	//PostSend()
}

void Session::Dispatch(IocpEvent* iocpEvent, INT32 numOfBytes)
{
	assert(iocpEvent->owner != nullptr);

	switch (iocpEvent->GetType())
	{
	case EventType::CONNECT:
		ProcessConnect();
		break;
	case EventType::DISCONNECT:
		ProcessDisconnect();
		break;
	case EventType::RECV:
		ProcessRecv(numOfBytes);
		break;
	case EventType::SEND:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}
