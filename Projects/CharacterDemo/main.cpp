// CharacterDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"

using namespace mray;

APPLICATION_ENTRY_POINT
{
	GCPtr<Application> app=new Application();

	core::string plgFileName;
	plgFileName=mT("plugins.stg");

	app->loadResourceFile(mT("CD_dataPath.stg"));
	if(app->startup(mT("MRaY App"),math::vector2di(800,600),false,plgFileName,0,true,false))
		app->run();
	app=0;
	return 0;
}

