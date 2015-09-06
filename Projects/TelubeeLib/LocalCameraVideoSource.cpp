
#include "stdafx.h"
#include "LocalCameraVideoSource.h"
#include "ICameraVideoGrabber.h"

#include "DirectShowVideoGrabber.h"
#include "FlyCameraVideoGrabber.h"

#include "StreamReader.h"

namespace mray
{
namespace TBee
{

// #if 1
// 	typedef video::DirectShowVideoGrabber VCameraType;
// #else
// 	typedef video::FlyCameraVideoGrabber VCameraType;
	// #endif
	LocalCameraVideoSource::LocalCameraVideoSource(int c1, int c2, EUSBCameraType camType)
{
		m_camConnection = camType;
	m_cameraResolution.set(640, 480);
	m_cameraFPS = 25;
	
	m_cameraSource[0].id = c1;
	m_cameraSource[1].id = c2;

	m_exposureValue = -1;
	m_gainValue = 0.1;
	m_WBValue = -1;
	m_gammaValue = 0.1;

	m_cameraSource[0].videoGrabber = new video::VideoGrabberTexture();
	
	if (c1 != c2)
		m_cameraSource[1].videoGrabber = new video::VideoGrabberTexture();
	else
		m_cameraSource[1].videoGrabber = 0;

	m_started = false;
}
LocalCameraVideoSource::~LocalCameraVideoSource()
{
	delete m_cameraSource[0].videoGrabber;
	delete m_cameraSource[1].videoGrabber;
}
float LocalCameraVideoSource::GetCaptureFrameRate(int i)
{
	if (m_cameraSource[i].camera)
		return m_cameraSource[i].camera->GetCaptureFrameRate();
	return 0;
}
void LocalCameraVideoSource::SetCameraResolution(const math::vector2d& res, int fps)
{
	m_cameraResolution = res;
	m_cameraFPS = fps;

	if (m_started)
	{
		for (int i = 0; i < 2;++i)
		{
			if (m_cameraSource[i].camera)
			{
				m_cameraSource[i].camera->Stop();
				m_cameraSource[i].camera->InitDevice(m_cameraSource[i].id, m_cameraResolution.x, m_cameraResolution.y, m_cameraFPS);
				m_cameraSource[i].camera->Start();

			}
		}
	}
}
void LocalCameraVideoSource::Init()
{
	{
		//load camera cropping offsets

		OS::IStreamPtr s = gFileSystem.openFile(gFileSystem.getAppPath() + "CameraOffsets.txt", OS::TXT_READ);
		if (!s.isNull())
		{
			OS::StreamReader rdr(s);
			core::StringConverter::parse(rdr.readLine(), m_cameraSource[0].offsets);
			core::StringConverter::parse(rdr.readLine(), m_cameraSource[1].offsets);
			s->close();
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		if (!m_cameraSource[i].videoGrabber)
			continue;
		//m_eyes[i].flip90 = true;
		if (m_camConnection==ECam_DirectShow)
			m_cameraSource[i].camera = new video::DirectShowVideoGrabber();
		else
		{
			video::FlyCameraVideoGrabber* c = new video::FlyCameraVideoGrabber();;
			m_cameraSource[i].camera = c;
			//c->SetImageFormat(video::EPixel_YUYV);

			c->SetCroppingOffset(m_cameraSource[i].offsets.x, m_cameraSource[i].offsets.y);
		}

		video::ITexturePtr tex = Engine::getInstance().getDevice()->createEmptyTexture2D(true);

		m_cameraSource[i].videoGrabber->Set(m_cameraSource[i].camera, tex);
	}
}
void LocalCameraVideoSource::Open()
{
	m_started = true;

	for (int i = 0; i < 2; ++i)
	{
		if (m_cameraSource[i].camera)
		{
			//m_cameraSource[i].camera->SetImageFormat(video::EPixel_YUYV);
			m_cameraSource[i].camera->InitDevice(m_cameraSource[i].id, m_cameraResolution.x, m_cameraResolution.y, m_cameraFPS);
			m_cameraSource[i].camera->Start();
		}
	}

	SetCameraParameterValue(video::ICameraVideoGrabber::Param_Exposure, (m_exposureValue>0 ? core::StringConverter::toString(m_exposureValue) : "auto"));
	SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gain, (m_gainValue > 0 ? core::StringConverter::toString(m_gainValue) : "auto"));
	SetCameraParameterValue(video::ICameraVideoGrabber::Param_WhiteBalance, (m_WBValue > 0 ? core::StringConverter::toString(m_WBValue) : "auto"));
	SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gamma, (m_gammaValue > 0 ? core::StringConverter::toString(m_gammaValue) : "auto"));
}
void LocalCameraVideoSource::Close()
{
	m_started = false;
	m_cameraSource[0].camera->Stop();
	if (m_cameraSource[1].camera)
		m_cameraSource[1].camera->Stop();
}
void LocalCameraVideoSource::SetCameraParameterValue(const core::string& name, const core::string& value)
{


	for (int i = 0; i < 2; ++i)
	{
		if (m_cameraSource[i].camera)
		{
			m_cameraSource[i].camera->SetParameter(name, value);

		//	printf("Camera [%d] %s value is: %s\n", i, name.c_str(), m_cameraSource[i].camera->GetParameter(name).c_str());
		}
	}
}
core::string LocalCameraVideoSource::GetCameraParameterValue(const core::string& namne)
{
	return m_cameraSource[0].camera->GetParameter(namne);
}

void LocalCameraVideoSource::SetCameraID(int i, int cam)
{
	m_cameraSource[i].id = cam;
}
math::vector2d LocalCameraVideoSource::GetEyeResolution(int i)
{
	if (!m_cameraSource[i].camera)
		return m_cameraSource[0].camera->GetFrameSize();
	return m_cameraSource[i].camera->GetFrameSize();
}
video::ITexturePtr LocalCameraVideoSource::GetEyeTexture(int i)
{
	if (!m_cameraSource[i].videoGrabber)
		return m_cameraSource[0].videoGrabber->GetTexture();
	return m_cameraSource[i].videoGrabber->GetTexture();
}

bool LocalCameraVideoSource::Blit(int eye)
{
	if (eye < 0)
	{
		bool a = m_cameraSource[0].videoGrabber->Blit();
		bool b = 0;
		if (m_cameraSource[1].videoGrabber)
			m_cameraSource[1].videoGrabber->Blit();
		return a || b;
	}
	else if (eye < 2)
	{
		if (!m_cameraSource[eye].videoGrabber)
			return false;
		return m_cameraSource[eye].videoGrabber->Blit();
	}
	return false;
}
void LocalCameraVideoSource::LoadFromXML(xml::XMLElement* e)
{

	xml::XMLAttribute* attr;

	attr = e->getAttribute("CameraLeft");
	if (attr)
		m_cameraSource[0].id = core::StringConverter::toInt(attr->value);
	attr = e->getAttribute("CameraRight");
	if (attr)
		m_cameraSource[1].id = core::StringConverter::toInt(attr->value);


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

	attr = e->getAttribute("Exposure");
	if (attr)
	{
		m_exposureValue = core::StringConverter::toFloat(attr->value);
	}
	attr = e->getAttribute("Gain");
	if (attr)
	{
		m_gainValue = core::StringConverter::toFloat(attr->value);
	}
	attr = e->getAttribute("WB");
	if (attr)
	{
		m_WBValue = core::StringConverter::toFloat(attr->value);
	}
	attr = e->getAttribute("Gamma");
	if (attr)
	{
		m_gammaValue = core::StringConverter::toFloat(attr->value);
	}
}

}
}
