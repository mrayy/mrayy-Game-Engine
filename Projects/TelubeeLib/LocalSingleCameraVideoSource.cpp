


#include "stdafx.h"
#include "LocalSingleCameraVideoSource.h"
#include "ICameraVideoGrabber.h"

#include "DirectShowVideoGrabber.h"


namespace mray
{
namespace TBee
{

typedef video::DirectShowVideoGrabber VCameraType;
LocalSingleCameraVideoSource::LocalSingleCameraVideoSource(int c1)
{
	m_cameraResolution.set(640, 480);
	m_cameraFPS = 25;

	m_cameraSource.id = c1;

	m_cameraSource.videoGrabber = new video::VideoGrabberTexture();

}
LocalSingleCameraVideoSource::~LocalSingleCameraVideoSource()
{

	delete m_cameraSource.videoGrabber;
}

void LocalSingleCameraVideoSource::Init()
{
		//m_eyes[i].flip90 = true;
	m_cameraSource.camera = new VCameraType();
	video::ITexturePtr tex = Engine::getInstance().getDevice()->createEmptyTexture2D(true);

	m_cameraSource.videoGrabber->Set(m_cameraSource.camera, tex);
}
void LocalSingleCameraVideoSource::Open()
{
	m_cameraSource.camera->InitDevice(m_cameraSource.id, m_cameraResolution.x, m_cameraResolution.y, m_cameraFPS);

	for (int i = 0; i < 2; ++i)
	{
		//	if (m_cameraSource[i].camera)
		//		m_cameraSource[i].camera->SetParameter(video::ICameraVideoGrabber::Param_Focus, "0.4");
	}

}
float LocalSingleCameraVideoSource::GetCaptureFrameRate(int i)
{
	return m_cameraSource.camera->GetCaptureFrameRate();
}
void LocalSingleCameraVideoSource::Close()
{
	m_cameraSource.camera->Stop();
}
math::vector2d LocalSingleCameraVideoSource::GetEyeResolution(int i)
{
	return m_cameraSource.camera->GetFrameSize();
}
video::ITexturePtr LocalSingleCameraVideoSource::GetEyeTexture(int i)
{
	return m_cameraSource.videoGrabber->GetTexture();
}

bool LocalSingleCameraVideoSource::Blit(int eye)
{
	bool a = m_cameraSource.videoGrabber->Blit();
	return a ;
}
void LocalSingleCameraVideoSource::LoadFromXML(xml::XMLElement* e)
{

	xml::XMLAttribute* attr;

	attr = e->getAttribute("CameraLeft");
	if (attr)
		m_cameraSource.id = core::StringConverter::toInt(attr->value);


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

