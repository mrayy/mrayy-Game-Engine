
#include "stdafx.h"
#include "Application.h"

using namespace mray;

APPLICATION_ENTRY_POINT
{
	mray::Application*app=new mray::Application();

	core::string resFileName=mT("plugins.stg");

	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),math::vector2di(800,600),false,resFileName,0,true,false))
		app->run();
	delete app;
}
