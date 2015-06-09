
#include "stdafx.h"
#include "GstLocalCameraVideoSource.h"
#include "GstLocalVideoPlayer.h"


namespace mray
{
namespace TBee
{

GstLocalCameraVideoSource::GstLocalCameraVideoSource(int c0,int c1 )
{
	m_player = new video::GstPlayerBin();

	video::GstLocalVideoPlayer*vp = new video::GstLocalVideoPlayer();
	m_player->AddPlayer(vp, "Video");



	m_playerGrabber = new video::VideoGrabberTexture();
	m_cameraSource[0] = c0;
	m_cameraSource[1] = c1;
	m_cameraFPS = 30;
	m_cameraResolution.set(1280, 720);
}

GstLocalCameraVideoSource::~GstLocalCameraVideoSource()
{
	delete m_playerGrabber;
	m_player->ClearPlayers(true);
}

void GstLocalCameraVideoSource::Init()
{
	m_playerGrabber->Set(new video::GstLocalVideoPlayerGrabber((video::GstLocalVideoPlayer*)m_player->GetPlayer("Video")), 0);
}
void GstLocalCameraVideoSource::Open()
{
	((video::GstLocalVideoPlayer*)m_player->GetPlayer("Video"))->SetCameras(m_cameraSource[0], m_cameraSource[1]);
	((video::GstLocalVideoPlayer*)m_player->GetPlayer("Video"))->SetResolution(m_cameraResolution.x, m_cameraResolution.y, m_cameraFPS);
	((video::GstLocalVideoPlayer*)m_player->GetPlayer("Video"))->CreateStream();


	m_player->Play();
}
void GstLocalCameraVideoSource::Close()
{
	m_player->CloseAll();

}
bool GstLocalCameraVideoSource::Blit(int eye)
{
	return m_playerGrabber->Blit();
}
float GstLocalCameraVideoSource::GetCaptureFrameRate(int i)
{
	return m_playerGrabber->GetGrabber()->GetCaptureFrameRate();
}
math::vector2d GstLocalCameraVideoSource::GetEyeScalingFactor(int i)
{
	return math::vector2d(IsStereo() ? 1 : 1, 1);
}
math::vector2d GstLocalCameraVideoSource::GetEyeResolution(int i)
{
	math::vector3di sz = m_playerGrabber->GetTexture()->getSize();
	return math::vector2d(sz.x / (IsStereo() ? 2 : 1), sz.y);
}
video::ITexturePtr GstLocalCameraVideoSource::GetEyeTexture(int i)
{
	return m_playerGrabber->GetTexture();
}
math::rectf GstLocalCameraVideoSource::GetEyeTexCoords(int i)
{
	if (IsStereo())
		return math::rectf(i*0.5, 0, 0.5 + i*0.5, 1);
	else
		return math::rectf(0, 0, 1, 1);
}

void GstLocalCameraVideoSource::LoadFromXML(xml::XMLElement* e)
{

	xml::XMLAttribute* attr;

	attr = e->getAttribute("CameraLeft");
	if (attr)
		m_cameraSource[0] = core::StringConverter::toInt(attr->value);
	attr = e->getAttribute("CameraRight");
	if (attr)
		m_cameraSource[1] = core::StringConverter::toInt(attr->value);


	attr = e->getAttribute("Size");
	if (attr)
	{
		m_cameraResolution = core::StringConverter::toVector2d(attr->value);
	}
	attr = e->getAttribute("FPS");
	if (attr)
	{
		m_cameraFPS = core::StringConverter::toInt(attr->value);
	}

}



}
}


