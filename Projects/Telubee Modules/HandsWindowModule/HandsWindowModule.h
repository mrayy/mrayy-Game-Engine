// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ROBOTCONTROLMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HandsWindowMODULE_EXPORTS functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HandsWindowMODULE_EXPORTS
#define HandsWindowMODULE_API extern "C" __declspec(dllexport)
#else
#define HandsWindowMODULE_API __declspec(dllimport)
#endif
#include <string>

namespace mray
{
	namespace TBee
	{
		class IServiceModule;
	}

	HandsWindowMODULE_API std::string DLL_GetServiceName();
	HandsWindowMODULE_API void  DLL_ServiceInit();
	HandsWindowMODULE_API void*  DLL_GetServiceModule();
	HandsWindowMODULE_API void  DLL_SreviceDestroy();
}


