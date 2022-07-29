#include "pch.h"
#include "StartUtil.h"

void NormalStart() {
	shared_ptr<Connector> connector = std::make_shared<Connector>(1, std::make_shared<ServerSession>, NetAddress(L"127.0.0.1", 7777));
	assert(connector->StartConnect());

	//Connecter의 Session이 DIspatch하게 한다.
	ThreadManager manager;
	for (int i = 0; i < 2; i++) {
		manager.Launch([=]() {
			while (true)
			{
				connector->GetIocpCore()->Dispatch();
			}
			});
	}

	string Id;
	while (true) {
		if (GConnected) {
			cout << "Enter ID: ";
			cin >> Id;
			shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::Make_CS_REQ_LOGIN(Id);
			GSessionManager.BroadCast(sendBuffer);
			this_thread::sleep_for(100ms);

			break;
		}
	}

	this_thread::sleep_for(100ms);

	std::cin.ignore();
	while (GConnected) {
		string msg;
		cout << "Enter Chat: ";
		getline(cin, msg);

		if (msg == "")
			continue;

		if (msg[0] == '/')
		{
			string _substr = msg.substr(1);
			string delim = " ";
			string operatr;

			{
				auto pos = _substr.find(delim);
				operatr = _substr.substr(0, pos);
				_substr.erase(0, pos + delim.length());
			}

			if (operatr == "roomchange") {
				string roomNumber;

				auto pos = _substr.find(delim);
				roomNumber = _substr.substr(0, pos);
				_substr.erase(0, pos + delim.length());

				shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::Make_CS_REQ_ROOM_CHANGE(stoi(roomNumber));
				GSessionManager.BroadCast(sendBuffer);
			}

			else if (operatr == "whisper") {
				string name, whisperMsg;

				{
					auto pos = _substr.find(delim);
					name = _substr.substr(0, pos);
					_substr.erase(0, pos + delim.length());
				}

				{
					auto pos = _substr.find(delim);
					whisperMsg = _substr.substr(0, pos);
					_substr.erase(0, pos + delim.length());
				}

				shared_ptr<SendBuffer> sendBuffer =
					ServerPacketHandler::Make_CS_REQ_WHISPER_CHAT(name, whisperMsg);
				GSessionManager.BroadCast(sendBuffer);
			}

			else {
				cout << "Cannot Find Operator " << operatr << endl;
			}
		}
		else {
			shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::Make_CS_REQ_NORMAL_CHAT(msg);
			GSessionManager.BroadCast(sendBuffer);
		}
	}

	manager.Join();
}

void StressStart()
{
	cout << "Stress Test..." << endl;

	shared_ptr<Connector> connector = std::make_shared<Connector>(10000, std::make_shared<ServerSession>, NetAddress(L"127.0.0.1", 7777));
	assert(connector->StartConnect());

	ThreadManager manager;
	for (int i = 0; i < 5; i++) {
		manager.Launch([=]() {
			while (true)
			{
				connector->GetIocpCore()->Dispatch();
			}
			});
	}

	string Id;
	while (true) {
		if (GConnected) {
			Id = "stress_test";
			shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::Make_CS_REQ_LOGIN(Id);
			GSessionManager.BroadCast(sendBuffer);
			this_thread::sleep_for(100ms);

			break;
		}
	}

	while (GConnected) {
		string msg;
		msg = "Stress Message";

		if (msg == "")
			continue;

		else {
			shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::Make_CS_REQ_TEST_ROOMCHAT(msg);
			GSessionManager.BroadCast(sendBuffer);
			this_thread::sleep_for(100ms);
		}
	}

	manager.Join();
}
