// LUTGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Application.h"

using namespace mray;
using namespace math;

APPLICATION_ENTRY_POINT
{	
	GCPtr<Application> app=new Application();

	std::vector<SOptionElement> extraOptions;
	core::string resFileName=mT("plugins.stg");

	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,false,false))
		app->run();
	app=0;

	return 0;
}

