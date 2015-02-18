// PhysicsTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VApplication.h"
#include "GCCollector.h"

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

	GCPtr<VApplication> app=new VApplication();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

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
	op.name="Shaders";
	op.value="Yes";
	op.valueSet.insert("Yes");
	op.valueSet.insert("No");
	extraOptions.push_back(op);
	op.valueSet.clear();
	//VLDEnable();
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("V.Telesar 1.10"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
	return 0;
}

