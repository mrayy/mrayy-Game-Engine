// VTelesar5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "GCCollector.h"

#include "compileconfig.h"

#include <windows.h>

// #include <vld.h>
// #include <vldapi.h>
#include "DirectShowVideoGrabber.h"

using namespace mray;
using namespace core;
using namespace math;

APPLICATION_ENTRY_POINT
{

	GCPtr<VT::Application> app=new VT::Application();

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
	op.name="Debugging";
	op.value="No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();
	op.name="HorizontalFlip";
	op.value="No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();
	op.name="GhostServer";
	op.value="192.168.10.102";
	extraOptions.push_back(op);
	op.valueSet.clear();
	op.name="DVIPort";
	op.value="COM3";
	extraOptions.push_back(op);
	op.valueSet.clear();

	for (int j = 0; j < 2; ++j)
	{
		op.name = "Camera" + core::StringConverter::toString(j);
		video::DirectShowVideoGrabber ds;
		int camsCount = ds.ListDevices();
		for (int i = 0; i<camsCount; ++i)
		{
			op.valueSet.insert(core::StringConverter::toString(i) + " - " + ds.GetDeviceName(i));
		}
		if (op.valueSet.size()>0)
		{
			op.value = *op.valueSet.begin();
		}
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
// 
// 	op.name="Camera";
// 	op.value="USB";
// 	op.valueSet.insert("USB");
// 	op.valueSet.insert("FlyCapture");
// 	extraOptions.push_back(op);
	op.valueSet.clear();
	op.name="Physics";
	op.value="No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();
	op.name="FlipCameras";
	op.value="No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();
	op.name="Preview";
	op.value="No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	//VLDEnable();
	app->loadResourceFile(mT("vtdataPath.stg"));
	if(app->startup(mT("V.Telesar 2.00"),vector2di(800,600),false,extraOptions,resFileName,0,true,false,false))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
	return 0;
}

