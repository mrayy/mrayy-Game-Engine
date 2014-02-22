

// VTelesar5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TRApplication.h"
#include "GCCollector.h"
#include "DirectShowVideoGrabber.h"
#include <windows.h>
// #include <vld.h>
// #include <vldapi.h>


using namespace mray;
using namespace core;
using namespace math;

#define EntryPoint int main()
EntryPoint
{

	GCPtr<TRApplication> app = new TRApplication();

	core::string resFileName = mT("plugins.stg");

#ifdef UNICODE
	resFileName = mT("pluginsU.stg");
#endif

	gLogManager.setVerbosLevel(EVL_Heavy);

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.valueSet.clear();
	for (int j = 0; j < 2;++j)
	{
		op.name = "Camera"+core::StringConverter::toString(j);
		video::DirectShowVideoGrabber ds;
		int camsCount = ds.ListDevices();
		for (int i = 0; i<camsCount; ++i)
		{
			op.valueSet.insert(core::StringConverter::toString(i)+" - " +ds.GetDeviceName(i));
		}
		if (op.valueSet.size()>0)
		{
			op.value = *op.valueSet.begin();
		}
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "Debugging";
		op.value = "No";
		op.valueSet.insert("Yes");
		op.valueSet.insert("No");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "DepthStream";
		op.value = "No";
		op.valueSet.insert("Yes");
		op.valueSet.insert("No");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "Controller";
		op.value = "Logicool";
		op.valueSet.insert("Logicool");
		op.valueSet.insert("XBox");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "Audio";
		op.value = "Yes";
		op.valueSet.insert("Yes");
		op.valueSet.insert("No");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "Network";
		op.value = "Local";
		op.valueSet.insert("Local");
		op.valueSet.insert("Global");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "Quality";
		op.value = "Medium";
		op.valueSet.insert("Ultra Low");
		op.valueSet.insert("Low");
		op.valueSet.insert("Medium");
		op.valueSet.insert("High");
		op.valueSet.insert("Ultra High");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	//VLDEnable();
	app->loadResourceFile(mT("tbdataPath.stg"));
	if (app->startup(mT("TELUBee Robot Agent 1.00"), vector2di(800, 600), false, extraOptions, resFileName, 0, true, true, true))
	{
		app->run();
	}

	//	VLDDisable();
	app = 0;

	GCCollector::shutdown();
	return 0;
}

