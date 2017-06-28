
#include "stdafx.h"
#include "TxProjHandsService.h"
#include "TBeeServiceContext.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "HandsWindow.h"
#include "RenderWindow.h"

#include "Win32WindowUtils.h"


namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(TxProjHandsService, IServiceModule)
const std::string TxProjHandsService::ModuleName("TxProjHandsServiceModule");




class TxProjHandsServiceImpl :public IServiceContextListener, public video::IRenderWindowListener
{
public:

	GCPtr<HandsWindow> m_handsWindow;

	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	uint m_handPort;
	bool m_connected;
public:

	TxProjHandsServiceImpl()
	{
		m_handsWindow = new HandsWindow();
		m_status = EServiceStatus::Idle;
		m_context = 0;
		m_connected = false;
	}

	~TxProjHandsServiceImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;

		printf("Initializing Hands Window\n");

		m_handsWindow->Parse(context->appOptions);
		m_handsWindow->OnInit(context);
		m_handsWindow->GetHandsWindow()->AddListener(this);

		context->AddListener(this);
		m_status = EServiceStatus::Inited;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->RemoveListener(this);

		m_handsWindow->OnClose();

		m_status = EServiceStatus::Idle;

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		m_handsWindow->OnEnable();
		m_status = EServiceStatus::Running;
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;

		m_handsWindow->OnDisable();
		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;

		m_handsWindow->OnUpdate(dt);
	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

		if (!Win32WindowUtils::doMessagePump())
		{
			m_status = EServiceStatus::Shutdown;
		}
		m_handsWindow->GetHandsWindow()->SetActiveWindow();
		m_handsWindow->GetHandsWindow()->Render(true);
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
		m_handsWindow->OnConnected(m_context->remoteAddr.toString(), 7010, 0);
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
			//m_handsWindow->OnConnected(m_context->remoteAddr.toString(), m_handPort, 0);
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


TxProjHandsService::TxProjHandsService()
{
	m_impl = new TxProjHandsServiceImpl();
}

TxProjHandsService::~TxProjHandsService()
{
	delete m_impl;
}


std::string TxProjHandsService::GetServiceName()
{
	return TxProjHandsService::ModuleName;
}

EServiceStatus TxProjHandsService::GetServiceStatus()
{
	return m_impl->m_status;
}

void TxProjHandsService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus TxProjHandsService::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool TxProjHandsService::StopService()
{
	return m_impl->Stop();
}

void TxProjHandsService::DestroyService()
{
	m_impl->Destroy();
}


void TxProjHandsService::Update(float dt)
{
	m_impl->Update(dt);
}

void TxProjHandsService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void TxProjHandsService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool TxProjHandsService::LoadServiceSettings(xml::XMLElement* elem)
{
	return m_impl->LoadServiceSettings(elem);
}

void TxProjHandsService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


