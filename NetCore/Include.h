#pragma once
#include <iostream>
#include <assert.h>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <utility>
#include <functional>
#include <istream>
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>
#include <time.h>

#include <thread>
#include <mutex>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#include "IocpEvent.h"
#include "FileLog.h"

using namespace std;

using SessionFactory = function<shared_ptr<Session>(void)>;