

// VTelesar5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TRApplication.h"
#include "GCCollector.h"
#include "DirectShowVideoGrabber.h"
#include "FlyCameraManager.h"
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
	app->LoadCameraProfiles("CameraProfiles.xml");
	core::string resFileName = mT("plugins.stg");

#ifdef UNICODE
	resFileName = mT("pluginsU.stg");
#endif

	gLogManager.setVerbosLevel(EVL_Heavy);
	

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.valueSet.clear();

	{
		op.name = "CameraConnection";
		op.value = "DirectShow";
		op.valueSet.insert("DirectShow");
		op.valueSet.insert("PointGray");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}

	string CameraName[2] = { "Camera_Left", "Camera_Right" };
	for (int c = 0; c < 2; ++c)
	{
		op.name = "DS_" + CameraName[c]; // "Camera" + core::StringConverter::toString(c);
		video::DirectShowVideoGrabber ds;
		int camsCount = ds.ListDevices();
		op.valueSet.insert("0 - None");
		for (int i = 0; i<camsCount; ++i)
		{
			op.valueSet.insert(core::StringConverter::toString(i + 1) + "-" + ds.GetDeviceName(i));
		}
		if (op.valueSet.size()>0)
		{
			op.value = *op.valueSet.begin();
		}
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	for (int c = 0; c < 2; ++c)
	{
		op.name = "PT_" + CameraName[c]; // "Camera" + core::StringConverter::toString(c);

		op.valueSet.insert("0 - None");
		int camsCount = video::FlyCameraManager::instance.GetCamerasCount();
		for (int i = 0; i<camsCount; ++i)
		{
			uint sp;
			video::FlyCameraManager::instance.GetCameraSerialNumber(i, sp);
			op.valueSet.insert(core::StringConverter::toString(i + 1) + " - FC_" + core::StringConverter::toString(sp));
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
		op.name = "EnablePlayers";
		op.value = "No";
		op.valueSet.insert("Yes");
		op.valueSet.insert("No");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "EnableStreams";
		op.value = "Yes";
		op.valueSet.insert("Yes");
		op.valueSet.insert("No");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "HandsDisplay";
		op.value = "-1";
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "StreamResolution";
		op.value = "0-VGA";
		op.valueSet.insert("0-VGA");
		op.valueSet.insert("1-HD");
		op.valueSet.insert("2-FullHD");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "CameraProfile";

		CameraProfileManager* cprof= app->GetCameraProfileManager();
		const std::map<core::string, CameraProfile*>& profiles=cprof->GetProfiles();
		std::map<core::string, CameraProfile*>::const_iterator it = profiles.begin();
		for (; it != profiles.end();++it)
		{
			op.valueSet.insert(it->first);
		}
		op.value = "";
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
		op.name = "Quality";
		op.value = "Medium";
		op.valueSet.insert("0- Ultra Low");
		op.valueSet.insert("1- Low");
		op.valueSet.insert("2- Medium");
		op.valueSet.insert("3- High");
	//	op.valueSet.insert("Ultra High");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	//VLDEnable();
	app->loadResourceFile(mT("tbdataPath.stg"));
	if (app->startup(mT("TELUBee Robot Agent 1.00"), vector2di(800, 600), false, extraOptions, resFileName,"TBAgentConfigure.stg", 0, true, false, true))
	{
		app->run();
	}

	//	VLDDisable();
	app = 0;

	GCCollector::shutdown();
	return 0;
}

