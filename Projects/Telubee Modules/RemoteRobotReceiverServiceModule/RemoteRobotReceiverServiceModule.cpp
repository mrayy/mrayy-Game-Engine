
#include "stdafx.h"
#include "RemoteRobotReceiverServiceModule.h"
#include "TBeeServiceContext.h"
#include "CommunicationMessages.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "ModuleSharedMemory.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "MutexLocks.h"
#include "RemoteControllerReceiver.h"

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(RemoteRobotReceiverServiceModule, IServiceModule)
	const std::string RemoteRobotReceiverServiceModule::ModuleName("RemoteRobotReceiverServiceModule");


class RemoteRobotReceiverServiceModuleImpl :public IServiceContextListener
{
public:

	EServiceStatus m_status;
	TBeeServiceContext* m_context;

	RemoteControllerReceiver* m_controller;
	core::string m_robotDll;
public:

public:

	RemoteRobotReceiverServiceModuleImpl()
	{
		m_controller = new RemoteControllerReceiver();
		m_status = EServiceStatus::Idle;
		m_robotDll = "Robot.dll";
	}

	~RemoteRobotReceiverServiceModuleImpl()
	{
		delete m_controller;
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;

		m_controller->Init(m_robotDll);
		m_status = EServiceStatus::Running;

		context->AddListener(this);
//  		m_thread = OS::IThreadManager::getInstance().createThread(new RemoteRobotControlThread(this), OS::ETP_Normal);
//  		m_thread->start(0);
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
	//	m_status = EServiceStatus::Idle;
		m_context->RemoveListener(this);

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

// 		m_RobotHandler->Initialize();
// 		if (m_RobotHandler->GetRobotController())
// 			m_RobotHandler->GetRobotController()->ConnectRobot();

		m_status = EServiceStatus::Running;
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;
		
		//m_status = EServiceStatus::Stopped;
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
	}
	virtual void OnUserDisconnected()
	{
	}
	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{
	}
};


RemoteRobotReceiverServiceModule::RemoteRobotReceiverServiceModule()
{
	m_impl = new RemoteRobotReceiverServiceModuleImpl();
}

RemoteRobotReceiverServiceModule::~RemoteRobotReceiverServiceModule()
{
	delete m_impl;
}


std::string RemoteRobotReceiverServiceModule::GetServiceName()
{
	return RemoteRobotReceiverServiceModule::ModuleName;
}

EServiceStatus RemoteRobotReceiverServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void RemoteRobotReceiverServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus RemoteRobotReceiverServiceModule::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool RemoteRobotReceiverServiceModule::StopService()
{
	return m_impl->Stop();
}

void RemoteRobotReceiverServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void RemoteRobotReceiverServiceModule::Update(float dt)
{
	m_impl->Update(dt);
}

void RemoteRobotReceiverServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void RemoteRobotReceiverServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool RemoteRobotReceiverServiceModule::LoadServiceSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e = elem->getSubElement("RobotParameters");
	if (e)
	{
		e = e->getSubElement("Value");
	}
	xml::XMLAttribute*a = elem->getAttribute("RobotDll");
	if (a)
	{
		m_impl->m_robotDll = a->value;
	}

	return true;
}

void RemoteRobotReceiverServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


