// PhysicsTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "GCCollector.h"
#include "OptiTrackClient.h"

using namespace mray;
using namespace core;
using namespace math;

int _tmain(int argc, _TCHAR* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_crtBreakAlloc
	long temp=-1;
	_crtBreakAlloc=temp;
#endif

	GCPtr<Application> app=new Application();
	GCPtr<Engine> e=Engine::getInstancePtr();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.name="RemoteIP";
	op.value="192.168.10.215";
	extraOptions.push_back(op);
	op.name="LocalIP";
	op.value="192.168.10.119";
	extraOptions.push_back(op);
	//VLDEnable();
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;
	e=0;

	GCCollector::shutdown();
	return 0;
}

