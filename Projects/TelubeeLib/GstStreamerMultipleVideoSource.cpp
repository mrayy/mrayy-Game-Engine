

#include "stdafx.h"
#include "GstStreamerMultipleVideoSource.h"
#include "VideoGrabberTexture.h"

#include "GstNetworkMultipleVideoPlayer.h"
#include "GstNetworkAudioPlayer.h"



namespace mray
{
namespace TBee
{

GstStreamerMultipleVideoSource::GstStreamerMultipleVideoSource(const core::string& ip, uint videoport, uint count, uint audioport, uint clockPort, bool rtcp, bool useAudio)
{
	m_useAudio = useAudio;
	m_player = new video::GstPlayerBin();

	m_playerImpl = new video::GstNetworkMultipleVideoPlayer();
	m_player->AddPlayer(m_playerImpl, "Video");

	if (m_useAudio)
	{
		video::GstNetworkAudioPlayer*ap = new video::GstNetworkAudioPlayer();
		m_player->AddPlayer(ap, "Audio");
	}

	SetIP(ip, videoport,count, audioport, clockPort, rtcp);
	m_isStereo = true;
}

GstStreamerMultipleVideoSource::~GstStreamerMultipleVideoSource()
{
	m_playerGrabber.clear();
	m_player->ClearPlayers(true);
}

void GstStreamerMultipleVideoSource::Init()
{
	m_playerGrabber.resize(m_count);
	for (int i = 0; i < m_count; ++i)
	{
		m_playerGrabber[i] = new video::VideoGrabberTexture();
		m_playerGrabber[i]->Set(new video::GstNetworkMultipleVideoPlayerGrabber((video::GstNetworkMultipleVideoPlayer*)m_player->GetPlayer("Video")), 0, i);
	}
}

video::GstNetworkMultipleVideoPlayer* GstStreamerMultipleVideoSource::GetVideoPlayer()
{
	return ((video::GstNetworkMultipleVideoPlayer*)m_player->GetPlayer("Video"));
}

void GstStreamerMultipleVideoSource::Open()
{
	m_playerImpl->SetIPAddress(m_ip, m_vport, m_count, 0, m_rtcp);
	m_playerImpl->CreateStream();
	printf("Open video streams\n"
		"\tPort base number:%d , Ports Count:%d\n", m_vport, m_count);
	if (m_useAudio)
	{
		((video::GstNetworkAudioPlayer*)m_player->GetPlayer("Audio"))->SetIPAddress(m_ip, m_aport, 0, m_rtcp);
		((video::GstNetworkAudioPlayer*)m_player->GetPlayer("Audio"))->CreateStream();
	}

	m_player->Play();
}
void GstStreamerMultipleVideoSource::Close()
{
	m_player->CloseAll();

}
bool GstStreamerMultipleVideoSource::Blit(int eye)
{
	return m_playerGrabber[eye]->Blit();
}
float GstStreamerMultipleVideoSource::GetCaptureFrameRate(int i)
{
	return m_playerImpl->GetCaptureFrameRate(i);
}
math::vector2d GstStreamerMultipleVideoSource::GetEyeScalingFactor(int i)
{
	return math::vector2d(m_isStereo ? 1 : 1, 1);// 2.0f / 3.0f);
}
math::vector2d GstStreamerMultipleVideoSource::GetEyeResolution(int i)
{
	math::vector3di sz = m_playerGrabber[i]->GetTexture()->getSize();
	return math::vector2d(sz.x , sz.y);
}
video::ITexturePtr GstStreamerMultipleVideoSource::GetEyeTexture(int i)
{
	return m_playerGrabber[i]->GetTexture();
}
math::rectf GstStreamerMultipleVideoSource::GetEyeTexCoords(int i)
{
	return math::rectf(0, 0, 1, 1);
}

void GstStreamerMultipleVideoSource::SetIsStereo(bool stereo)
{
	m_isStereo = stereo;
}


}
}


