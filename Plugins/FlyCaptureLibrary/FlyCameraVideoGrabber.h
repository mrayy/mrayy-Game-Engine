

/********************************************************************
	created:	2012/10/15
	created:	15:10:2012   17:56
	filename: 	d:\Development\mrayEngine\Plugins\FlyCaptureLibrary\FlyCameraVideoGrabber.h
	file path:	d:\Development\mrayEngine\Plugins\FlyCaptureLibrary
	file base:	FlyCameraVideoGrabber
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___FlyCameraVideoGrabber___
#define ___FlyCameraVideoGrabber___


#include "ICameraVideoGrabber.h"

namespace mray
{
namespace video
{

	class FlyCameraData;
class FlyCameraVideoGrabber:public ICameraVideoGrabber
{
protected:
	FlyCameraData* m_data;

	math::vector2di m_size;
	video::ImageInfo  m_textureImage;
	video::ImageInfo  m_tempImage;
	bool m_hasNewFrame;
	video::EPixelFormat m_format;

	int m_fps;
	int m_device;
	bool m_inited;

	OS::IMutex* m_imageMutex;

	void ShutDown();

	float m_lastGrabbed;

	ulong m_bufferId;

public:
	FlyCameraVideoGrabber();
	virtual~FlyCameraVideoGrabber();

	int ListDevices();
	core::string GetDeviceName(int id);
	void SetDevice(int id);

	void SetFrameRate(int fps);
	int GetFrameRate();

	bool InitDevice(int device,int w,int h,int fps);

	void SetFrameSize(int w,int h);
	const math::vector2di& GetFrameSize();

	void SetImageFormat(video::EPixelFormat fmt);
	video::EPixelFormat GetImageFormat();

	bool GrabFrame();
	bool HasNewFrame();	
	ulong GetBufferID(){ return m_bufferId; }

	void Stop();
	void Start();

	const video::ImageInfo*  GetLastFrame();

	void BlitImage(const uchar* buf,int rows,int cols,EPixelFormat fmt);
	
};

}
}

#endif
