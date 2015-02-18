

#include "stdafx.h"
#include "OpenNICaptureDevice.h"
#include "ILogManager.h"


namespace mray
{

OpenNICaptureDevice::OpenNICaptureDevice(const char* deviceURI):m_uri(deviceURI)
{
	m_isOpen=false;
	m_pUserTracker = 0;
	//Open();
}
OpenNICaptureDevice::~OpenNICaptureDevice()
{
	Close();
}


openni::Status OpenNICaptureDevice::Open(int w, int h, bool depth, bool color )
{
	Close();
	openni::Status rc = m_device.open(m_uri);
	if (rc != openni::STATUS_OK)
	{
		gLogManager.log("Failed to open device:" + core::string(openni::OpenNI::getExtendedError()), ELL_WARNING);
		return rc;
	}
	if (depth)
	{
		rc = m_depthStream.create(m_device, openni::SENSOR_DEPTH);
		if (rc == openni::STATUS_OK)
		{
			openni::VideoMode mode = m_depthStream.getVideoMode();
			mode.setResolution(w, h);
			m_depthStream.setVideoMode(mode);
			rc = m_depthStream.start();
			if (rc != openni::STATUS_OK)
			{
				gLogManager.log("Failed to start depth sensor: " + core::string(m_device.getDeviceInfo().getName()), ELL_WARNING);
				Close();
				return rc;
			}
		}
		else
		{
			gLogManager.log("Failed to find depth sensor: " + core::string(m_device.getDeviceInfo().getName()), ELL_WARNING);
			m_device.close();
			return rc;
		}
	}
	if (color)
	{
		rc = m_colorStream.create(m_device, openni::SENSOR_COLOR);
		if (rc == openni::STATUS_OK)
		{
			openni::VideoMode mode = m_colorStream.getVideoMode();
			mode.setResolution(w, h);
			m_colorStream.setVideoMode(mode);
			rc = m_colorStream.start();
			if (rc != openni::STATUS_OK)
			{
				gLogManager.log("Failed to start color sensor: " + core::string(m_device.getDeviceInfo().getName()), ELL_WARNING);
				Close();
				return rc;
			}
		}
		else
		{
			gLogManager.log("Failed to find color sensor: " + core::string(m_device.getDeviceInfo().getName()), ELL_WARNING);
			m_depthStream.destroy();
			m_device.close();
			return rc;
		}
	}
	m_isOpen = true;
	return rc;
}
bool OpenNICaptureDevice::CreateUserTracker()
{
	if (!m_isOpen)
		return false;
	if (m_pUserTracker)
	{
		m_pUserTracker->destroy();
		delete m_pUserTracker;
		m_pUserTracker = 0;
	}
	m_pUserTracker = new nite::UserTracker();
	if (m_pUserTracker->create(&m_device) != nite::STATUS_OK)
	{
		delete m_pUserTracker;
		return false;
	}
	return true;
}
bool OpenNICaptureDevice::IsOpen()
{
	return m_isOpen;
}
void OpenNICaptureDevice::Close()
{
	if(!m_isOpen)
		return;
	m_isOpen=false;
	m_depthStream.destroy();
	m_colorStream.destroy();
	m_userTrackerFrame.release();
	delete m_pUserTracker;
	m_pUserTracker=0;
	m_device.close();
}

bool OpenNICaptureDevice::Update()
{
	if(!m_isOpen)
		return false;
	nite::Status rc = m_pUserTracker->readFrame(&m_userTrackerFrame);
	if (rc != nite::STATUS_OK)
	{
		gLogManager.log("GetNextData failed",ELL_WARNING,EVL_Heavy);
		return false;
	}
	return true;
}



}



