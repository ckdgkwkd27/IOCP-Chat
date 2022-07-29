#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <Acceptor.h>
#include <ThreadManager.h>
#include "ClientSession.h"
#include "ClientPacketHandler.h"
#include <BufferReader.h>
#include <BufferWriter.h>

int main() {
	GPlayerManager.Init();
	GRoomManager.Init(5, 10);

	shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(3000, std::make_shared<ClientSession>, NetAddress(L"127.0.0.1", 7777));
	assert(acceptor->StartAccept());

	ThreadManager manager;
	for (int i = 0; i < 7; i++) {
		manager.Launch([=]() {
			while (true) 
			{
				acceptor->GetIocpCore()->Dispatch();
			}
		});
	}

	manager.Join();
}
