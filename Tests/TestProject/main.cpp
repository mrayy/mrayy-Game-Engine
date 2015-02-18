

#include "stdafx.h"
#include "Application.h"

using namespace mray;
using namespace core;
using namespace math;



APPLICATION_ENTRY_POINT{

#if defined(_DEBUG) && defined(WIN32)
	// check for memory leaks at program exit (after the 'return 0')
	// through a call to _CrtDumpMemoryLeaks 
	// note that in debug mode, objects allocated with the new operator 
	// may be destroyed *after* the end of the main function. 
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	long temp=-1;
	_crtBreakAlloc=temp;
#endif
	GCPtr<Application> app=new Application();

	core::string plgFileName;
	plgFileName=mT("plugins.stg");

	app->loadResourceFile(mT("hdataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,plgFileName,0,true,false))
		app->run();
	app=0;
}


