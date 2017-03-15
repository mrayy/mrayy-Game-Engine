
#include "stdafx.h"
#include "GNSSServiceModule.h"
#include "TBeeServiceContext.h"
#include "CommunicationMessages.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "ModuleSharedMemory.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "MutexLocks.h"
#include "GNSSController.h"

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(GNSSServiceModule, IServiceModule)
	const std::string GNSSServiceModule::ModuleName("GNSSServiceModule");


class GNSSServiceModuleImpl :public IServiceContextListener
{
public:


	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	bool m_connected;

	GNSSController* m_controller;

	std::string m_port;
//	core::string m_plcIP;
//	int m_plcPort;
public:

public:

	GNSSServiceModuleImpl()
	{
		m_status = EServiceStatus::Idle;
		m_connected = false;
		m_controller = new GNSSController();
//		m_plcPort = PLC_PORT_TCP_UNUSED;
	//	m_plcIP = MELSEC_PLC;
	}

	~GNSSServiceModuleImpl()
	{
		delete m_controller;
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;

		m_status = EServiceStatus::Inited;
		m_controller->Init(m_port/*,m_plcIP,m_plcPort*/);
		
		context->AddListener(this);
//  		m_thread = OS::IThreadManager::getInstance().createThread(new RemoteRobotControlThread(this), OS::ETP_Normal);
//  		m_thread->start(0);
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_status = EServiceStatus::Idle;
		m_context->RemoveListener(this);
		m_controller->Shutdown();

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		m_status = EServiceStatus::Running;
		m_controller->Start();
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;
		m_controller->Stop();
		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;

	}
	void DebugRender(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;


	}
	void Render(ServiceRenderContext* context)
	{
		m_controller->Render(context);
	}


	//////////////////////////////////////////////////////////////////////////
	/// Listeners
	virtual void OnUserConnected(const UserConnectionData& data)
	{
		m_connected = true;
	}
	virtual void OnUserDisconnected()
	{
		m_connected = false;
	}
	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{
	}
};


GNSSServiceModule::GNSSServiceModule()
{
	m_impl = new GNSSServiceModuleImpl();
}

GNSSServiceModule::~GNSSServiceModule()
{
	delete m_impl;
}


std::string GNSSServiceModule::GetServiceName()
{
	return GNSSServiceModule::ModuleName;
}

EServiceStatus GNSSServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void GNSSServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus GNSSServiceModule::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool GNSSServiceModule::StopService()
{
	return m_impl->Stop();
}

void GNSSServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void GNSSServiceModule::Update(float dt)
{
	m_impl->Update(dt);
}

void GNSSServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void GNSSServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool GNSSServiceModule::LoadServiceSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e = elem->getSubElement("RobotParameters");
	if (e)
	{
		e = e->getSubElement("Value");
	}
	xml::XMLAttribute*a = elem->getAttribute("GNSSPort");
	if (a)
	{
		m_impl->m_port = a->value;
	}
	/*
	a = elem->getAttribute("PLCIP");
	if (a)
	{
		m_impl->m_plcIP = a->value;
	}
	a = elem->getAttribute("PLCPort");
	if (a)
	{
		m_impl->m_plcPort = core::StringConverter::toInt( a->value);
	}
	*/
	return true;
}

void GNSSServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


