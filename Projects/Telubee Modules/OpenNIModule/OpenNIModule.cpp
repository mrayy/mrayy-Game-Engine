// AudioVideoModule.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "OpenNIModule.h"
#include "OpenNIService.h"


namespace mray
{
	namespace TBee
	{
		class IServiceModule;

		class  CServiceModule
		{
		protected:
			OpenNIService* m_impl;
		public:

			static CServiceModule* instance;

			CServiceModule(void);
			// TODO: add your methods here.

			virtual~CServiceModule();

			OpenNIService* GetService()
			{
				return m_impl;
			}

		};

		CServiceModule* CServiceModule::instance = 0;

		// This is the constructor of a class that has been exported.
		// see TelubeeRobotDLL.h for the class definition
		CServiceModule::CServiceModule()
		{
			m_impl = new OpenNIService();

		}


		CServiceModule::~CServiceModule(void)
		{
			delete m_impl;
		}

	}

	PLCMODULE_API std::string DLL_GetServiceName()
	{
		return TBee::OpenNIService::ModuleName;
	}
	PLCMODULE_API void  DLL_ServiceInit()
	{
		printf("[%s] Init\n", DLL_GetServiceName().c_str());
		TBee::CServiceModule::instance = new TBee::CServiceModule();
	}
	PLCMODULE_API void*  DLL_GetServiceModule()
	{
		return TBee::CServiceModule::instance->GetService();
	}
	PLCMODULE_API void  DLL_SreviceDestroy()
	{
		if (TBee::CServiceModule::instance)
		{
			printf("[%s] Destroy\n", DLL_GetServiceName().c_str());
			delete TBee::CServiceModule::instance;
			TBee::CServiceModule::instance = 0;
		}
	}
}


