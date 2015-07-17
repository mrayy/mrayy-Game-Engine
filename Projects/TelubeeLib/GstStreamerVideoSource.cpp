

#include "stdafx.h"
#include "GstStreamerVideoSource.h"
#include "VideoGrabberTexture.h"

#include "GstNetworkVideoPlayer.h"
#include "GstNetworkAudioPlayer.h"



namespace mray
{
namespace TBee
{

GstStreamerVideoSource::GstStreamerVideoSource(const core::string& ip, uint videoport, uint audioport, uint clockPort,bool rtcp, bool useAudio)
{
	m_useAudio = useAudio;
	m_player = new video::GstPlayerBin();

	video::GstNetworkVideoPlayer*vp = new video::GstNetworkVideoPlayer();
	m_player->AddPlayer(vp, "Video");

	if (m_useAudio)
	{
		video::GstNetworkAudioPlayer*ap = new video::GstNetworkAudioPlayer();
		m_player->AddPlayer(ap, "Audio");
	}



	m_playerGrabber = new video::VideoGrabberTexture();
	SetIP(ip, videoport, audioport, clockPort, rtcp);
	m_isStereo = true;
}

GstStreamerVideoSource::~GstStreamerVideoSource()
{
	delete m_playerGrabber;
	m_player->ClearPlayers(true);
}

void GstStreamerVideoSource::Init()
{
	m_playerGrabber->Set(new video::GstNetworkVideoPlayerGrabber((video::GstNetworkVideoPlayer*)m_player->GetPlayer("Video")), 0);
}

video::GstNetworkVideoPlayer* GstStreamerVideoSource::GetVideoPlayer()
{
	return ((video::GstNetworkVideoPlayer*)m_player->GetPlayer("Video"));
}

void GstStreamerVideoSource::Open()
{
	((video::GstNetworkVideoPlayer*)m_player->GetPlayer("Video"))->SetIPAddress(m_ip, m_vport, m_clockPort, m_rtcp);
	((video::GstNetworkVideoPlayer*)m_player->GetPlayer("Video"))->CreateStream();

	if (m_useAudio)
	{
		((video::GstNetworkAudioPlayer*)m_player->GetPlayer("Audio"))->SetIPAddress(m_ip, m_aport, m_clockPort+1, m_rtcp);
		((video::GstNetworkAudioPlayer*)m_player->GetPlayer("Audio"))->CreateStream();
	}

	m_player->Play();
}
void GstStreamerVideoSource::Close()
{
	m_player->CloseAll();

}
bool GstStreamerVideoSource::Blit(int eye)
{
	return m_playerGrabber->Blit();
}
float GstStreamerVideoSource::GetCaptureFrameRate(int i)
{
	return m_playerGrabber->GetGrabber()->GetCaptureFrameRate();
}
math::vector2d GstStreamerVideoSource::GetEyeScalingFactor(int i)
{
	return math::vector2d(m_isStereo ? 1 : 1, 2.0f/3.0f);
}
math::vector2d GstStreamerVideoSource::GetEyeResolution(int i)
{
	math::vector3di sz = m_playerGrabber->GetTexture()->getSize();
	return math::vector2d(sz.x / (m_isStereo ? 2 : 1), sz.y);
}
video::ITexturePtr GstStreamerVideoSource::GetEyeTexture(int i)
{
	return m_playerGrabber->GetTexture();
}
math::rectf GstStreamerVideoSource::GetEyeTexCoords(int i)
{
	if (m_isStereo)
		return math::rectf(i*0.5, 0, 0.5 + i*0.5, 1);
	else
		return math::rectf(0, 0, 1, 1);
}

void GstStreamerVideoSource::SetIsStereo(bool stereo)
{
	m_isStereo = stereo;
}


}
}


