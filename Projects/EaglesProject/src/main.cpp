
#include <mrayEngine.h>
#include "eagleApplication.h"

using namespace mray;
using namespace math;



APPLICATION_ENTRY_POINT
{
#if defined(_DEBUG_) && defined(WIN32)
	// check for memory leaks at program exit (after the 'return 0')
	// through a call to _CrtDumpMemoryLeaks 
	// note that in debug mode, objects allocated with the new operator 
	// may be destroyed *after* the end of the main function. 
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	long temp=-1;
	_crtBreakAlloc=temp;
#endif


	//Main Application
	{
		GCPtr<eagles::EagleApplication> app=new eagles::EagleApplication();

		if(app->startApplication())
			app->run();
	}
	GCCollector::shutdown();
	return 0;
}

