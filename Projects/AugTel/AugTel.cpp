// VTelesar5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "GCCollector.h"
#include "DirectShowVideoGrabber.h"
#include "FlyCameraManager.h"
#include <windows.h>

//#define ENABLE_VLD

#ifdef ENABLE_VLD
	#include <vld.h>
	#include <vldapi.h>
#endif

using namespace mray;
using namespace core;
using namespace math;

#ifdef _DEBUG 
#define EntryPoint int main()
#else
#define EntryPoint int main()
//#define EntryPoint int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

#endif
EntryPoint
{

	GCPtr<AugTel::Application> app = new AugTel::Application();

	core::string resFileName = mT("plugins.stg");

#ifdef UNICODE
	resFileName = mT("pluginsU.stg");
#endif

	printf("%d\n", sizeof(std::string));


	gLogManager.setVerbosLevel(EVL_Heavy);
	new video::FlyCameraManager();

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.valueSet.clear();
	op.name = "Debugging";
	op.value = "No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();
	{

		op.name = "Preview";
		op.value = "No";
		op.valueSet.insert("Yes");
		op.valueSet.insert("No");
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
		int camsCount = video::FlyCameraManager::getInstance().GetCamerasCount();
		for (int i = 0; i<camsCount; ++i)
		{
			uint sp;
			video::FlyCameraManager::getInstance().GetCameraSerialNumber(i, sp);
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
		op.name = "CameraType";
		op.value = "DirectShow";
		op.valueSet.insert("DirectShow");
		op.valueSet.insert("PointGray");
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
		op.name = "CameraSource";
		op.value = "Remote";
		op.valueSet.insert("Local");
		op.valueSet.insert("Remote");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "HeadController";
		op.value = "Oculus";
		op.valueSet.insert("IK");
		op.valueSet.insert("Oculus");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}
	{
		op.name = "Stereoscopic";
		op.value = "None";
		op.valueSet.insert("None");
		op.valueSet.insert("Side-by-side");
		op.valueSet.insert("Up-bottom");
		op.valueSet.insert("StereoTV");
		op.valueSet.insert("Oculus");
		extraOptions.push_back(op);
		op.valueSet.clear();

	}
	{
		op.name = "Robot";
		op.value = "Oculus";
		op.valueSet.insert("Keyboard");
		op.valueSet.insert("Joystick");
		op.valueSet.insert("Wiiboard");
		op.valueSet.insert("Oculus");
		extraOptions.push_back(op);
		op.valueSet.clear();
	}

#ifdef ENABLE_VLD
	VLDEnable();
#endif
	app->loadResourceFile(mT("atdataPath.stg"));
	if (app->startup(mT("Oculus VR + TORSO"), vector2di(800, 600), false, extraOptions, resFileName,"AugTelConfig.stg", 0, true, true, true))
	{
		app->run();
	}

#ifdef ENABLE_VLD
	VLDDisable();
#endif
	app = 0;

	GCCollector::shutdown();
	return 0;
}

