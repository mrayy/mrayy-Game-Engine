
#include "stdafx.h"
#include "TxEmptyService.h"
#include "TBeeServiceContext.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"


namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(TxEmptyService, IServiceModule)
const std::string TxEmptyService::ModuleName("TxEmptyServiceModule");




class TxEmptyServiceImpl :public IServiceContextListener
{
public:


	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	uint m_handPort;
	bool m_connected;
public:

	TxEmptyServiceImpl()
	{
		m_status = EServiceStatus::Idle;
		m_context = 0;
		m_connected = false;
	}

	~TxEmptyServiceImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;


		context->AddListener(this);
		m_status = EServiceStatus::Inited;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->RemoveListener(this);


		m_status = EServiceStatus::Idle;

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		m_status = EServiceStatus::Running;
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;

		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;

	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

	}
	void DebugRender(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

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
		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);
		std::vector<core::string> values = core::StringUtil::Split(value, ",");
		if (msg == "HandPorts" && values.size() >= 1)
		{
			m_handPort = core::StringConverter::toInt(values[0]);
		}
	}


	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd, scene::ViewPort* vp)
	{

	}
	virtual void WindowClosed(video::RenderWindow* window)
	{
		m_status = EServiceStatus::Shutdown;
	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{

		return true;
	}
};


TxEmptyService::TxEmptyService()
{
	m_impl = new TxEmptyServiceImpl();
}

TxEmptyService::~TxEmptyService()
{
	delete m_impl;
}


std::string TxEmptyService::GetServiceName()
{
	return TxEmptyService::ModuleName;
}

EServiceStatus TxEmptyService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxEmptyService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxEmptyService::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool TxEmptyService::StopService()
{
	return m_impl->Stop();
}

void TxEmptyService::DestroyService()
{
	m_impl->Destroy();
}


void TxEmptyService::Update(float dt)
{
	m_impl->Update(dt);
}

void TxEmptyService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxEmptyService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxEmptyService::LoadServiceSettings(xml::XMLElement* elem)
{
	return m_impl->LoadServiceSettings(elem);
}

void TxEmptyService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


