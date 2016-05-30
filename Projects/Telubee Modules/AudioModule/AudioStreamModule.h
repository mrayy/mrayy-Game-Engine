// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AUDIOVIDEOMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AUDIOMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifdef AUDIOMODULE_EXPORTS
#define AUDIOMODULE_API extern "C" __declspec(dllexport)
#else
#define AUDIOMODULE_API __declspec(dllimport)
#endif

#include <string>

namespace mray
{
	namespace TBee
	{
		class IServiceModule;
	}

	AUDIOMODULE_API std::string DLL_GetServiceName();
	AUDIOMODULE_API void  DLL_ServiceInit();
	AUDIOMODULE_API void*  DLL_GetServiceModule();
	AUDIOMODULE_API void  DLL_SreviceDestroy();
}


