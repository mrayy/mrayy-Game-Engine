


#include "stdafx.h"
#include "Application.h"
#include "GCCollector.h"

using namespace mray;
using namespace core;
using namespace math;

APPLICATION_ENTRY_POINT
{
#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_crtBreakAlloc
	long temp=-1;
	_crtBreakAlloc=temp;
#endif

	GCPtr<Application> app=new Application();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	//VLDEnable();
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("Jet Figher X"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
}

