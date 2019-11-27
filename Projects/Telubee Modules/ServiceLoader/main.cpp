// ServiceLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServiceLoader.h"
#include "CrashHandler.h"
#include <signal.h>
#include "stackwalker.h"


#ifdef _WIN32
int const sigClosed = SIGBREAK;
#else
int const sigClosed = SIGHUP;
#endif

using namespace mray;

ServiceLoader* s_loader = 0;

void OnExit()
{
	if (s_loader)
	{
		try
		{
			//gLogManager.log("Exiting Service", ELL_INFO);
			delete s_loader;
		}
		catch (...)
		{
		}
		s_loader = 0;
	}
}

void OnExitSig(int)
{
	OnExit();
}

class StackWalkerToLog: public StackWalker
{
protected:
	virtual void OnOutput(LPCSTR szText) { 

		gLogManager.log(szText, ELL_INFO);
	}
};


void handler(int sig) {

	gLogManager.log("Exception!", ELL_INFO);
	StackWalkerToLog sw;
	sw.ShowCallstack();
	//exit(1);
}
int _tmain(int argc, _TCHAR* argv[])
{


	signal(SIGSEGV, handler);
	signal(SIGTERM, handler);
	signal(SIGINT, handler);
	signal(SIGILL, handler);
	signal(sigClosed, OnExitSig);

	atexit(OnExit);
	if (argc < 2)
	{
		printf("Provide module name!\n");
		return 0;
	}
	core::string crashReportName = argv[1];
	//CrashHandler s_crashHandler(crashReportName+".err");

	s_loader=new ServiceLoader();
	if (s_loader->Init(argc, argv))
	{

		s_loader->Run();
	}
	OnExit();

	return 0;
}

