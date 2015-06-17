

#include "stdafx.h"
#include "GstStereoNetVideoSource.h"
#include "ICameraVideoGrabber.h"
#include "GStreamVideoProvider.h"



namespace mray
{
namespace TBee
{

GstStereoNetVideoSource::GstStereoNetVideoSource(const core::string& ip )
{
	m_providers = new GStreamVideoProvider();
	SetIP(ip);
	m_isStereo = true;
}

GstStereoNetVideoSource::~GstStereoNetVideoSource()
{
	m_providers->Disconnect();
	delete m_providers;
}

void GstStereoNetVideoSource::Init()
{
	m_remoteTex = Engine::getInstance().getDevice()->createTexture2D(4, video::EPixel_R8G8B8, true);
	m_remoteTex->setMipmapsFilter(false);
}
void GstStereoNetVideoSource::Open()
{
	m_providers->ConnectToCameras(m_ip, 5000, 5002, 5001);
	m_frameCount = 0;
	m_timeAcc = 0;
	m_lastT = 0;
	m_captureFPS = 0;
}
void GstStereoNetVideoSource::Close()
{
	m_providers->Disconnect();

}
bool GstStereoNetVideoSource::Blit(int eye)
{

	bool dirty = false;
	m_providers->Update(gEngine.getFPS()->dt());
	if (m_providers->HasNewImage(0))
	{
		const video::ImageInfo* image = m_providers->GetImage(0);
		if (m_remoteTex->getSize() != image->Size)
			dirty = true;
		m_remoteTex->createTexture(math::vector3d(image->Size.x, image->Size.y, 1), image->format);
		video::LockedPixelBox box(math::box3d(0, image->Size), image->format, image->imageData);
		m_remoteTex->getSurface(0)->blitFromMemory(box);
		float t = gEngine.getTimer()->getSeconds();
		m_timeAcc += (t - m_lastT)*0.001f;

		++m_frameCount;
		if (m_timeAcc > 1)
		{
			m_captureFPS = m_frameCount;
			m_timeAcc = m_timeAcc - (int)m_timeAcc;
			m_frameCount = 0;
			//	printf("Capture FPS: %d\n", m_captureFPS);
		}
		m_lastT = t;
	}
	if (dirty)
		return true;
	return false;
}

math::vector2d GstStereoNetVideoSource::GetEyeScalingFactor(int i)
{
	return math::vector2d(m_isStereo?2:1, 1); 
}
math::vector2d GstStereoNetVideoSource::GetEyeResolution(int i)
{
	math::vector3di sz = m_remoteTex->getSize();
	return math::vector2d(sz.x / (m_isStereo?2:1), sz.y);
}
video::ITexturePtr GstStereoNetVideoSource::GetEyeTexture(int i)
{
	return m_remoteTex;
}
math::rectf GstStereoNetVideoSource::GetEyeTexCoords(int i)
{ 
	if (m_isStereo)
		return math::rectf(i*0.5, 0, 0.5 + i*0.5, 1);
	else 
		return math::rectf(0, 0, 1, 1);
}

void GstStereoNetVideoSource::SetIsStereo(bool stereo)
{
	m_isStereo = stereo;
}


}
}

