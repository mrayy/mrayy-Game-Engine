// TestCharacter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Application.h"


APPLICATION_ENTRY_POINT
{
	GCPtr<Application> app=new Application();

	std::vector<SOptionElement> extraOptions;

	core::string resFileName;
	resFileName=mT("plugins.stg");
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),math::vector2di(800,600),false,extraOptions,resFileName,0,true,false))
		app->run();
	app=0;

	return 0;
}

