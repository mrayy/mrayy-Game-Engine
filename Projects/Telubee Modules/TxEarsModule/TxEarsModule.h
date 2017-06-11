// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TxEarsVIDEOMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TxEarsMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#ifdef TxEarsMODULE_EXPORTS
#define TxEarsMODULE_API extern "C" __declspec(dllexport)
#else
#define TxEarsMODULE_API __declspec(dllimport)
#endif

#include <string>

namespace mray
{
	namespace TBee
	{
		class IServiceModule;
	}

	TxEarsMODULE_API std::string DLL_GetServiceName();
	TxEarsMODULE_API void  DLL_ServiceInit();
	TxEarsMODULE_API void*  DLL_GetServiceModule();
	TxEarsMODULE_API void  DLL_ServiceDestroy();
}


