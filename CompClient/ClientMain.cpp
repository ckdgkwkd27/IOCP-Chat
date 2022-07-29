#include "pch.h"
#include <Connector.h>
#include <ThreadManager.h>
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "StartUtil.h"
#define STRESS_TEST 1

int main() {
	if (STRESS_TEST == 0)
		NormalStart();
	else
		StressStart();
}