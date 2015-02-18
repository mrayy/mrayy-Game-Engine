// PIDController.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GCCollector.h"
#include "PIDApplication.h"

using namespace mray;
using namespace core;
using namespace math;

APPLICATION_ENTRY_POINT
{
	GCPtr<PIDApplication> app=new PIDApplication();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	gLogManager.setVerbosLevel(EVL_Heavy);


	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.name="Stereoscopic";
	op.value="None";
	op.valueSet.insert("None");
	op.valueSet.insert("OverUnder");
	op.valueSet.insert("SideBySide");
	op.valueSet.insert("DVI");
	extraOptions.push_back(op);
	op.valueSet.clear();
	//VLDEnable();
	app->loadResourceFile(mT("vtdataPath.stg"));
	if(app->startup(mT("PID Controller"),vector2di(800,600),false,extraOptions,resFileName,0,true,false,true))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
	return (int) 0;
}

