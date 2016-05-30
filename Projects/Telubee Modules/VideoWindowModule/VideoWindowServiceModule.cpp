
#include "stdafx.h"
#include "VideoWindowServiceModule.h"
#include "TBeeServiceContext.h"
#include "StringUtil.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "VideoWindow.h"
#include "RenderWindow.h"

#include "Win32WindowUtils.h"


namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(VideoWindowServiceModule, IServiceModule)
const std::string VideoWindowServiceModule::ModuleName("VideoWindowServiceModule");




class VideoWindowServiceModuleImpl :public IServiceContextListener, public video::IRenderWindowListener
{
public:

	GCPtr<VideoWindow> m_VideoWindow;

	EServiceStatus m_status;
	TBeeServiceContext* m_context;
	uint m_videoPort;
	bool m_connected;
public:

	VideoWindowServiceModuleImpl()
	{
		m_VideoWindow = new VideoWindow();
		m_status = EServiceStatus::Idle;
		m_context = 0;
		m_connected = false;
		m_videoPort = 7011;
	}

	~VideoWindowServiceModuleImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;

		printf("Initializing Video Window\n");

		m_VideoWindow->Parse(context->appOptions);
		m_VideoWindow->OnInit(context);
		m_VideoWindow->GetVideoWindow()->AddListener(this);

		context->AddListener(this);
		m_status = EServiceStatus::Inited;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->RemoveListener(this);

		m_VideoWindow->OnClose();

		m_status = EServiceStatus::Idle;

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		m_VideoWindow->OnEnable();
		m_status = EServiceStatus::Running;
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;

		m_VideoWindow->OnDisable();
		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;

		m_VideoWindow->OnUpdate(dt);
	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

		if (!Win32WindowUtils::doMessagePump())
		{
			m_status = EServiceStatus::Shutdown;
		}
		m_VideoWindow->GetVideoWindow()->SetActiveWindow();
		m_VideoWindow->GetVideoWindow()->Render(true);
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
		m_VideoWindow->OnConnected(m_context->remoteAddr.toString(), m_videoPort, 0);
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
		if (msg == "VideoWindowPorts" && values.size() >= 1)
		{
			m_videoPort = core::StringConverter::toInt(values[0]);
			m_VideoWindow->OnConnected(m_context->remoteAddr.toString(), m_videoPort, 0);
		}
	}


	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd, scene::ViewPort* vp)
	{

	}
	virtual void WindowClosed(video::RenderWindow* window)
	{
		m_status = EServiceStatus::Shutdown;
	}

};


VideoWindowServiceModule::VideoWindowServiceModule()
{
	m_impl = new VideoWindowServiceModuleImpl();
}

VideoWindowServiceModule::~VideoWindowServiceModule()
{
	delete m_impl;
}


std::string VideoWindowServiceModule::GetServiceName()
{
	return VideoWindowServiceModule::ModuleName;
}

EServiceStatus VideoWindowServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void VideoWindowServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus VideoWindowServiceModule::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool VideoWindowServiceModule::StopService()
{
	return m_impl->Stop();
}

void VideoWindowServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void VideoWindowServiceModule::Update(float dt)
{
	m_impl->Update(dt);
}

void VideoWindowServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void VideoWindowServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool VideoWindowServiceModule::LoadServiceSettings(xml::XMLElement* e)
{
	return true;
}

void VideoWindowServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


