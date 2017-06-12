// RobotControlModule.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TxBodyModule.h"
#include "TxBodyService.h"


namespace mray
{
namespace TBee
{
class IServiceModule;

class  CServiceModule
{
protected:
	TxBodyService* m_impl;
public:

	static CServiceModule* instance;

	CServiceModule(void);
	// TODO: add your methods here.

	virtual~CServiceModule();

	TxBodyService* GetService()
	{
		return m_impl;
	}

};

CServiceModule* CServiceModule::instance = 0;

// This is the constructor of a class that has been exported.
// see TelubeeRobotDLL.h for the class definition
CServiceModule::CServiceModule()
{
	m_impl = new TxBodyService();

}


CServiceModule::~CServiceModule(void)
{
	delete m_impl;
}

}

ROBOTCONTROLMODULE_API std::string DLL_GetServiceName()
{
	return TBee::TxBodyService::ModuleName;
}
ROBOTCONTROLMODULE_API void  DLL_ServiceInit()
{
	printf("[%s] Init\n", DLL_GetServiceName().c_str());
	TBee::CServiceModule::instance = new TBee::CServiceModule();
}
ROBOTCONTROLMODULE_API void*  DLL_GetServiceModule()
{
	return TBee::CServiceModule::instance->GetService();
}
ROBOTCONTROLMODULE_API void  DLL_ServiceDestroy()
{
	if (TBee::CServiceModule::instance)
	{
		printf("[%s] Destroy\n", DLL_GetServiceName().c_str());
		delete TBee::CServiceModule::instance;
		TBee::CServiceModule::instance = 0;
	}
}
}


