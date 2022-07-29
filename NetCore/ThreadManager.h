#pragma once
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void Launch(function<void(void)> callback);
	void Join();

private:
	vector<thread> _threads;
	recursive_mutex _lock;
};

