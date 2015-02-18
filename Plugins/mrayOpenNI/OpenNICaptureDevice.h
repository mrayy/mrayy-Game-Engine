

/********************************************************************
	created:	2013/08/16
	created:	16:8:2013   11:16
	filename: 	C:\Development\mrayEngine\Plugins\mrayOpenNI\OpenNICaptureDevice.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOpenNI
	file base:	OpenNICaptureDevice
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OpenNICaptureDevice__
#define __OpenNICaptureDevice__

#include <NITE.h>


namespace mray
{

class OpenNICaptureDevice
{
protected:
	openni::Device		m_device;
	nite::UserTracker*	m_pUserTracker;

	nite::UserTrackerFrameRef m_userTrackerFrame;

	const char* m_uri;
	bool m_isOpen;
	openni::VideoStream m_depthStream;
	openni::VideoStream m_colorStream;


public:
	OpenNICaptureDevice(const char* deviceURI);
	virtual~OpenNICaptureDevice();


	openni::Status Open(int w,int h,bool depth=true,bool color=true);
	bool IsOpen();
	void Close();

	bool Update();

	bool CreateUserTracker();


	openni::VideoStream& GetDepthStream(){return m_depthStream;}
	openni::VideoStream& GetColorStream(){return m_colorStream;}

	nite::UserTrackerFrameRef* GetFrame(){return &m_userTrackerFrame;}


	nite::UserTracker* GetTracker(){return m_pUserTracker;}
};

}


#endif
