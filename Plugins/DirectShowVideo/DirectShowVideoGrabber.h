

/********************************************************************
	created:	2012/07/27
	created:	27:7:2012   14:29
	filename: 	d:\Development\mrayEngine\Plugins\DirectShowVideo\DirectShowVideoGrabber.h
	file path:	d:\Development\mrayEngine\Plugins\DirectShowVideo
	file base:	DirectShowVideoGrabber
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___DirectShowVideoGrabber___
#define ___DirectShowVideoGrabber___

#include "ICameraVideoGrabber.h"

class videoInput;

namespace mray
{
namespace video
{

class DirectShowVideoGrabber:public ICameraVideoGrabber
{
protected:
	static videoInput* s_videoInput;
	static int s_refCount;
protected:
	math::vector2di m_size;
	video::ImageInfo  m_textureImage;
	bool m_hasNewFrame;
	video::EPixelFormat m_format;

	int m_fps;
	int m_device;

	int m_captureFPS;
	int m_frameCount;
	float m_timeAcc;
	float m_lastT;

	bool m_inited;
	ulong m_bufferId;

public:
	DirectShowVideoGrabber();
	virtual~DirectShowVideoGrabber();

	int ListDevices();
	core::string GetDeviceName(int id);
	core::string GetDevicePath(int id);
	void SetDevice(int id);

	void SetFrameRate(int fps);
	int GetFrameRate();

	bool InitDevice(int device,int w,int h,int fps);

	void SetFrameSize(int w,int h);
	const math::vector2di& GetFrameSize();

	void SetImageFormat(video::EPixelFormat fmt);
	video::EPixelFormat GetImageFormat();

	int GetCaptureFPS(){ return m_captureFPS; }

	ulong GetBufferID(){ return m_bufferId; }

	bool GrabFrame();
	bool HasNewFrame();

	void Stop();
	void Start();

	const video::ImageInfo*  GetLastFrame();

	void SetParameter(const core::string& name,const core::string& value);
	core::string GetParameter(const core::string& name);

};

}
}

#endif
