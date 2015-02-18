// VTelesar5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "GCCollector.h"
#include <windows.h>
// #include <vld.h>
// #include <vldapi.h>


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

	GCPtr<Application> app = new Application();

	core::string resFileName = mT("plugins.stg");

#ifdef UNICODE
	resFileName = mT("pluginsU.stg");
#endif

	printf("%d\n", sizeof(std::string));


	gLogManager.setVerbosLevel(EVL_Heavy);

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.valueSet.clear();
	op.name = "Debugging";
	op.value = "No";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();

	op.name = "OptiServer";
	op.value = "127.0.0.1";
	extraOptions.push_back(op);

	//VLDEnable();
	app->loadResourceFile(mT("atdataPath.stg"));
	if (app->startup(mT("SIGGRAPH14 - WhiteBoard 1.00"), vector2di(800, 600), false, extraOptions, resFileName, 0, true, true, true))
	{
		app->run();
	}

	//	VLDDisable();
	app = 0;

	GCCollector::shutdown();
	return 0;
}

