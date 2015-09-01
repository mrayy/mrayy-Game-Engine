// ServiceLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServiceLoader.h"


using namespace mray;

ServiceLoader* s_loader = 0;

void OnExit()
{
	if (s_loader)
	{
		delete s_loader;
		s_loader = 0;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	atexit(OnExit);
	s_loader=new ServiceLoader();
	if (s_loader->Init(argc, argv))
	{
		s_loader->Run();
	}
	OnExit();

	return 0;
}
