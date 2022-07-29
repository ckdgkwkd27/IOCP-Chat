#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{

}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{

	lock_guard<recursive_mutex> guard(_lock);
	_threads.push_back(thread([=]() {
		callback(); })
	);

}

void ThreadManager::Join()
{
	for (auto& t : _threads) {
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}
