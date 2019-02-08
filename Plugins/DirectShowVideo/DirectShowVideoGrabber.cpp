

#include "stdafx.h"
#include "DirectShowVideoGrabber.h"
#include "videoInput.h"
#include "Engine.h"
#include "ITimer.h"
#include "IVideoDevice.h"
#include "IThreadManager.h"
#include "ILogManager.h"

#include <strmif.h>

namespace mray
{
namespace video
{


	videoInput* DirectShowVideoGrabber::s_videoInput=0;
	int DirectShowVideoGrabber::s_refCount=0;
DirectShowVideoGrabber::DirectShowVideoGrabber()
{
	m_format=video::EPixel_B8G8R8;
	m_inited=false;
	m_fps=30;
	m_device=0;
	++s_refCount;
	if(s_refCount==1)
	{
		s_videoInput=new videoInput();
		s_videoInput->setVerbose(false);
		//s_videoInput->setComMultiThreaded(true);
		s_videoInput->setUseCallback(true);
	}
	m_textureImage.autoDel=true;
	m_hasNewFrame = false;
	m_bufferId = 0;
	m_timeAcc = 0;

	m_lock = OS::IThreadManager::getInstance().createMutex();
}
DirectShowVideoGrabber::~DirectShowVideoGrabber()
{
	Stop();
	--s_refCount;
	if(s_refCount==0)
		delete s_videoInput;
}
int DirectShowVideoGrabber::ListDevices()
{
	return s_videoInput->listDevices(true);
}
core::string DirectShowVideoGrabber::GetDeviceName(int id)
{
	core::string str;
	const char* d=s_videoInput->getDeviceName(id);
	core::char_to_string(d,str);
	return str;
}
core::string DirectShowVideoGrabber::GetDevicePath(int id)
{
	core::string str;
//	char* d = s_videoInput->getDevicePath(id);
	//core::char_to_string(d, str);
	str = s_videoInput->getDeviceName(id);
	return str;
}
void DirectShowVideoGrabber::SetDevice(int id)
{
	InitDevice(id,m_size.x,m_size.y,m_fps);
}
void DirectShowVideoGrabber::SetFrameRate(int fps)
{
	m_fps=fps;
}
int DirectShowVideoGrabber::GetFrameRate()
{
	return m_fps;
}
bool DirectShowVideoGrabber::InitDevice(int device,int w,int h,int fps)
{
	if (m_inited)
	{
		s_videoInput->stopDevice(m_device);
	}

	m_device=device;
	if (m_device < 0)
		return false
		;
	gLogManager.log(core::string("DirectShowVideoGrabber::InitDevice() - Initing Device: ") + core::StringConverter::toString(m_device), ELL_INFO);

	//int format = VI_NTSC_M;
	//s_videoInput->setFormat(device, format);
		
//	s_videoInput->setRequestedMediaSubType(VI_MEDIASUBTYPE_YUYV);
//	s_videoInput->setIdealFramerate(device, fps);
	if(!s_videoInput->setupDevice(device,w,h))
	{
		m_inited=false;
		return false;
	}
	m_inited=true;

	m_frameCount = 0;
	m_timeAcc = 0;
	m_lastT = 0;
	m_captureFPS = 0;

// 	s_videoInput->setVideoSettingFilterPct(device,s_videoInput->propExposure,0.1,2);
// 	s_videoInput->setVideoSettingFilterPct(device,s_videoInput->propWhiteBalance,0.1,2);
//	s_videoInput->showSettingsWindow(device);

	w=s_videoInput->getWidth(device);
	h=s_videoInput->getHeight(device);
	int ppCnt=s_videoInput->getSize(device);


	printf("Camera Index:%d Width=%d, Height=%d, FPS=%d\n", m_device, w, h,fps);
	if(m_size.x!=w || m_size.y!=h)
	{
		m_size.x=w;
		m_size.y=h;

	}
	return true;
}

void DirectShowVideoGrabber::Stop()
{
	if (m_inited)
	{
		m_inited = false;
		Sleep(10);
		s_videoInput->stopDevice(m_device);
	}
}
void DirectShowVideoGrabber::Start()
{
//	if(m_inited)
	{
		if (!InitDevice(m_device, m_size.x, m_size.y, m_fps))
		{
			//bool r=s_videoInput->restartDevice(m_device);
			//if(!r)
			gLogManager.log(core::string("DirectShowVideoGrabber::Start() - Failed to start directshow camera device ") + core::StringConverter::toString(m_device), ELL_WARNING);
		}else 
			s_videoInput->setAutoReconnectOnFreeze(m_device, false, 500);
	}
}
bool DirectShowVideoGrabber::IsConnected()
{
	return m_inited && s_videoInput->isDeviceSetup(m_device);
}
void DirectShowVideoGrabber::SetFrameSize(int w,int h)
{
	InitDevice(m_device,w,h,m_fps);
}
const math::vector2di& DirectShowVideoGrabber::GetFrameSize()
{
	return m_size;
}

void DirectShowVideoGrabber::SetImageFormat(video::EPixelFormat fmt)
{
}
video::EPixelFormat DirectShowVideoGrabber::GetImageFormat()
{
	return m_format;
}

bool DirectShowVideoGrabber::GrabFrame(int i )
{
	m_hasNewFrame=false;
	if(!m_inited)
		return false;
	if(s_videoInput->isFrameNew(m_device))
	{
		float t = gEngine.getTimer()->getSeconds();
		m_timeAcc += (t - m_lastT);

		m_frameCount++;
		if (m_timeAcc > 1000)
		{
			m_captureFPS = m_frameCount;
			m_frameCount = 0;
			m_timeAcc = 0;

		//	printf("Capture FPS: %d\n", m_captureFPS);
		}

		m_lastT = t;

		m_bufferId++;
		m_hasNewFrame=true;
		unsigned char * viPixels = s_videoInput->getPixels(m_device, false,true);

		Lock();
		m_textureImage.setData(viPixels,math::vector3d(m_size.x,m_size.y,1),m_format);
		Unlock();

		return true;
	}
	return false;
	
}

void DirectShowVideoGrabber::Lock()
{
	m_lock->lock();
}

void DirectShowVideoGrabber::Unlock()
{
	m_lock->unlock();
}
bool DirectShowVideoGrabber::HasNewFrame(int i)
{
	return m_hasNewFrame;
}


const video::ImageInfo* DirectShowVideoGrabber::GetLastFrame(int i)
{
	return &m_textureImage;
}

void DirectShowVideoGrabber::SetParameter(const core::string& name,const core::string& value)
{
	if (m_device == -1 || !m_inited)
		return;
	if (GetParameter(name) == value)
		return;
	bool isauto = (value == "auto");
	float v = 0;
	if (!isauto)
		v=core::StringConverter::toFloat(value);

	long flags = 0;
	if (isauto)
		flags = VideoProcAmp_Flags_Auto;
	else flags = VideoProcAmp_Flags_Manual;

#define setValue(param)\
	if (isauto)\
		s_videoInput->setVideoSettingFilter(m_device, param, v, flags, true);\
	else\
		s_videoInput->setVideoSettingFilter(m_device, param, v, flags);

	if(name.equals_ignore_case(Param_Exposure))
	{
		setValue(s_videoInput->propExposure);
	}else if(name.equals_ignore_case(Param_Brightness))
	{

		setValue(s_videoInput->propBrightness);
	}else  if(name.equals_ignore_case(Param_Contrast))
	{

		setValue(s_videoInput->propContrast);
	}else  if(name.equals_ignore_case(Param_Hue))
	{
		setValue(s_videoInput->propHue);
	}else  if(name.equals_ignore_case(Param_Saturation))
	{
		setValue(s_videoInput->propSaturation);
	}else  if(name.equals_ignore_case(Param_Sharpness))
	{
		setValue(s_videoInput->propSharpness);
	}else  if(name.equals_ignore_case(Param_ColorEnable))
	{
		setValue(s_videoInput->propColorEnable);

	}else  if(name.equals_ignore_case(Param_WhiteBalance))
	{
		setValue(s_videoInput->propWhiteBalance);
	}else  if(name.equals_ignore_case(Param_BacklightCompensation))
	{
		setValue(s_videoInput->propBacklightCompensation);

	}else  if(name.equals_ignore_case(Param_Pan))
	{

		setValue(s_videoInput->propPan);
	}else  if(name.equals_ignore_case(Param_Tilt))
	{

		setValue(s_videoInput->propTilt);
	}else  if(name.equals_ignore_case(Param_Roll))
	{
		setValue(s_videoInput->propRoll);
	}else  if(name.equals_ignore_case(Param_Zoom))
	{
		setValue(s_videoInput->propZoom);
	}else  if(name.equals_ignore_case(Param_Iris))
	{
		setValue(s_videoInput->propIris);

	}else  if(name.equals_ignore_case(Param_Focus))
	{
		setValue(s_videoInput->propFocus);

	}
	else  if (name.equals_ignore_case(Param_Gain))
	{
		setValue(s_videoInput->propGain);

	}
	else  if (name.equals_ignore_case(Param_Gamma))
	{
		setValue(s_videoInput->propGamma);

	}
}

core::string DirectShowVideoGrabber::GetParameter(const core::string& name)
{
	if (m_device == -1 || !m_inited)
		return "";
	long min; long max; long SteppingDelta; long currentValue=0; long flags; long defaultValue;
	if(name.equals_ignore_case(Param_Exposure))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propExposure,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else if(name.equals_ignore_case(Param_Brightness))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propBrightness,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Contrast))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propContrast,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Hue))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propHue,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Saturation))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propSaturation,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Sharpness))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propSharpness,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_ColorEnable))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propColorEnable,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_WhiteBalance))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propWhiteBalance,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_BacklightCompensation))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propBacklightCompensation,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Pan))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propPan,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Tilt))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propTilt,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Roll))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propRoll,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Zoom))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propZoom,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Iris))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propIris,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}else  if(name.equals_ignore_case(Param_Focus))
	{
		s_videoInput->getVideoSettingFilter(m_device,s_videoInput->propFocus,min,max,SteppingDelta,currentValue,flags,defaultValue);
	}
	else  if (name.equals_ignore_case(Param_Gamma))
	{
		s_videoInput->getVideoSettingFilter(m_device, s_videoInput->propGamma, min, max, SteppingDelta, currentValue, flags, defaultValue);
	}
	else  if (name.equals_ignore_case(Param_Gamma))
	{
		s_videoInput->getVideoSettingFilter(m_device, s_videoInput->propGamma, min, max, SteppingDelta, currentValue, flags, defaultValue);
	}
	else  if (name.equals_ignore_case(Param_Gain))
	{
		s_videoInput->getVideoSettingFilter(m_device, s_videoInput->propGain, min, max, SteppingDelta, currentValue, flags, defaultValue);
	}
	else  if (name.equals_ignore_case(Param_Gamma))
	{
		s_videoInput->getVideoSettingFilter(m_device, s_videoInput->propGamma, min, max, SteppingDelta, currentValue, flags, defaultValue);
	}

	return core::StringConverter::toString(currentValue);
}


}
}