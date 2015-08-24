// ServiceHostManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ServiceHostManager.h"

using namespace mray;

ServiceHostManager *manager=0;

void OnExit()
{
	if (manager)
	{
		delete manager;
		manager = 0;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	atexit(OnExit);
	manager = new ServiceHostManager();
	manager->Init(argc, argv);

	manager->Run();
	OnExit();
	return 0;
}

