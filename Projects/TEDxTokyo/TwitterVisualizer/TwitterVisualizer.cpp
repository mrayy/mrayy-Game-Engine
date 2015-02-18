// TwitterVisualizer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "GCCollector.h"


using namespace mray;
using namespace core;
using namespace math;
int _tmain(int argc, _TCHAR* argv[])
{
	GCPtr<Application> app = new Application();

	core::string resFileName = mT("plugins.stg");

#ifdef UNICODE
	resFileName = mT("pluginsU.stg");
#endif


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
	//VLDEnable();
	app->loadResourceFile(mT("tedDataPath.stg"));
	if (app->startup(mT("TEDxTokyo 2014"), vector2di(800, 600), false, extraOptions, resFileName, 0, true, true, true))
	{
		app->run();
	}

	//	VLDDisable();
	app = 0;

	GCCollector::shutdown();
}

