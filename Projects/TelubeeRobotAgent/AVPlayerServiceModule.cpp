

#include "stdafx.h"
#include "AVPlayerServiceModule.h"

#include "VideoGrabberTexture.h"
#include "GstPlayerBin.h"
#include "TBeeServiceContext.h"
#include "GstNetworkAudioPlayer.h"
#include "GstNetworkVideoPlayer.h"
#include "ViewPort.h"


namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(AVPlayerServiceModule, IServiceModule)
	const std::string AVPlayerServiceModule::ModuleName("AVPlayerServiceModule");

class AVPlayerServiceModuleImpl :public IServiceContextListener
{
public:
	EServiceStatus m_status;
	TBeeServiceContext* m_context;

	video::VideoGrabberTexture* m_playerGrabber;
	GCPtr<video::GstPlayerBin> m_players;
public:

	AVPlayerServiceModuleImpl()
	{
		m_players = new video::GstPlayerBin();
		m_status = EServiceStatus::Idle;
		m_context = 0;
	}

	~AVPlayerServiceModuleImpl()
	{
	}

	void Init(TBeeServiceContext* context)
	{
		m_context = context;

		m_playerGrabber = new video::VideoGrabberTexture();
		{
			video::GstNetworkAudioPlayer* player;
			player = new video::GstNetworkAudioPlayer();

			m_players->AddPlayer(player, "Audio");
		}
		{
			video::GstNetworkVideoPlayer* player;
			player = new video::GstNetworkVideoPlayer();
			m_players->AddPlayer(player, "Video");

			m_playerGrabber->Set(new video::GstNetworkVideoPlayerGrabber(player), 0);
		}

		context->AddListener(this);
		m_status = EServiceStatus::Inited;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;
		m_context->RemoveListener(this);

		m_players->ClearPlayers(true);

		m_status = EServiceStatus::Idle;

	}

	void Start()
	{
		if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
			return;

		// User got connected, Begin the video stream
		((video::GstNetworkAudioPlayer*)m_players->GetPlayer("Audio"))->CreateStream();
		((video::GstNetworkVideoPlayer*)m_players->GetPlayer("Video"))->CreateStream();
		m_players->Play();

		m_status = EServiceStatus::Running;
	}
	bool Stop()
	{
		if (m_status != EServiceStatus::Running)
			return false;

		m_players->CloseAll();
		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Update(float dt)
	{
		if (m_status != EServiceStatus::Running)
			return;
	}
	void DebugRender(TbeeServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;
	}
	void Render(TbeeServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;

		video::TextureUnit tex;
		math::vector2d txsz;
		scene::ViewPort*vp = context->viewPort;

		m_playerGrabber->Blit();
		tex.SetTexture(m_playerGrabber->GetTexture());
		txsz.x = m_playerGrabber->GetTexture()->getSize().x;
		txsz.y = m_playerGrabber->GetTexture()->getSize().y;
		float r = (float)vp->GetSize().y / (float)vp->GetSize().x;
		float w = txsz.x*r;
		float c = txsz.x - w;
		gEngine.getDevice()->useTexture(0, &tex);
		math::rectf texCoords(1, 0, 0, 1);
		gEngine.getDevice()->draw2DImage(math::rectf(c / 2, 0, w, vp->GetSize().y), 1, 0, &texCoords);
	}


	//////////////////////////////////////////////////////////////////////////
	/// Listeners
	virtual void OnUserConnected(const UserConnectionData& data)
	{
		((video::GstNetworkVideoPlayer*)m_players->GetPlayer("Video"))->SetIPAddress(data.address.toString(), data.videoPort, data.clockPort, data.rtcp);
		((video::GstNetworkAudioPlayer*)m_players->GetPlayer("Audio"))->SetIPAddress(data.address.toString(), data.audioPort, data.clockPort + 1, data.rtcp);
	}
};


AVPlayerServiceModule::AVPlayerServiceModule()
{
	m_impl = new AVPlayerServiceModuleImpl();
}

AVPlayerServiceModule::~AVPlayerServiceModule()
{
	delete m_impl;
}


std::string AVPlayerServiceModule::GetServiceName()
{
	return AVPlayerServiceModule::ModuleName;
}

EServiceStatus AVPlayerServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void AVPlayerServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus AVPlayerServiceModule::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool AVPlayerServiceModule::StopService()
{
	return m_impl->Stop();
}

void AVPlayerServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void AVPlayerServiceModule::Update(float dt)
{
	m_impl->Update(dt);
}

void AVPlayerServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render((TbeeServiceRenderContext*)contex);
}

void AVPlayerServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender((TbeeServiceRenderContext*)contex);
}

bool AVPlayerServiceModule::LoadServiceSettings(xml::XMLElement* e)
{
	return true;
}

void AVPlayerServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}



