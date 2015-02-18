

#include "stdafx.h"
#include "GCCollector.h"
#include "TSApplication.h"

using namespace mray;
using namespace core;
using namespace math;

APPLICATION_ENTRY_POINT
{
	GCPtr<TSApplication> app=new TSApplication();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	gLogManager.setVerbosLevel(EVL_Heavy);

	std::vector<SOptionElement> extraOptions;

	//VLDEnable();
	app->loadResourceFile(mT("IVRCDataPath.stg"));
	if(app->startup(mT("!nteractTeam - Touch Shadow"),vector2di(800,600),false,extraOptions,resFileName,0,true,false,true))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
	return (int) 0;
}

