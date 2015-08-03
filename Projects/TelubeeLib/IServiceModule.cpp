

#include "stdafx.h"
#include "IServiceModule.h"


namespace mray
{
namespace TBee
{

	IMPLEMENT_ROOT_RTTI(IServiceModule);


	static core::string ServiceStatusStr[]=
	{
		"Idle",
		"Inited",
		"Running",
		"Stopped",
		"Shutdown"
	};

	core::string IServiceModule::ServiceStatusToString(EServiceStatus s)
	{
		return ServiceStatusStr[(int)s];
	}
}
}