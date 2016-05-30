// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ROBOTCONTROLMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ROBOTCONTROLMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GNSSModuleDLL_EXPORTS
#define GNSSMODULE_API extern "C" __declspec(dllexport)
#else
#define GNSSMODULE_API __declspec(dllimport)
#endif
#include <string>

namespace mray
{
	namespace TBee
	{
		class IServiceModule;
	}

	GNSSMODULE_API std::string DLL_GetServiceName();
	GNSSMODULE_API void  DLL_ServiceInit();
	GNSSMODULE_API void*  DLL_GetServiceModule();
	GNSSMODULE_API void  DLL_SreviceDestroy();
}


