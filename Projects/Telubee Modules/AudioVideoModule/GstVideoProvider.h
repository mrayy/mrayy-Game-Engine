

/********************************************************************
	created:	2013/12/01
	created:	1:12:2013   23:10
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotAgent\GstVideoProvider.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotAgent
	file base:	GstVideoProvider
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GstVideoProvider__
#define __GstVideoProvider__

#include "IVideoGrabber.h"
#include "NetAddress.h"

namespace mray
{

	class GstVideoProviderImpl;

	class GstVideoGrabber
	{
	public:
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
		virtual video::IVideoGrabber* GetGrabber() = 0;
	};

	enum class EStreamingQuality
	{
		UltraLow,
		Low,
		Medium,
		High,
		UltraHigh
	};

	struct CameraInfo
	{
		int index;
		core::string guidPath;
	};
class GstVideoProvider
{
protected:
	GstVideoProviderImpl* m_impl;
	bool m_connected;
public:
	GstVideoProvider();
	virtual~GstVideoProvider();

	void SetCameras(const CameraInfo& cam0, const CameraInfo& cam1);
	void SetTargetResolution(const math::vector2di& res);

	bool IsStereoCameras();

	void SetDataSource(GstVideoGrabber* src);
	void StreamDataTo(const network::NetAddress& target,int videoPort,int audioPort);

	void EnableAudio(bool a);
	void SetNetworkType(bool isLocal);

	void Start(EStreamingQuality quality);
	void Stop();
	bool IsConnected();
};

}


#endif
