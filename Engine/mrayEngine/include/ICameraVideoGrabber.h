
/********************************************************************
	created:	2012/07/27
	created:	27:7:2012   14:38
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ICameraVideoGrabber.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ICameraVideoGrabber
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ICameraVideoGrabber___
#define ___ICameraVideoGrabber___

#include "IVideoGrabber.h"

namespace mray
{
namespace video
{


class MRAY_DLL ICameraVideoGrabber:public IVideoGrabber
{
public:
	static const core::string Param_Brightness;
	static const core::string Param_Contrast;
	static const core::string Param_Hue;
	static const core::string Param_Saturation;
	static const core::string Param_Sharpness;
	static const core::string Param_ColorEnable;
	static const core::string Param_WhiteBalance;
	static const core::string Param_Gain;
	static const core::string Param_BacklightCompensation;

	static const core::string Param_Pan;
	static const core::string Param_Tilt;
	static const core::string Param_Roll;
	static const core::string Param_Zoom;
	static const core::string Param_Exposure;
	static const core::string Param_Iris;
	static const core::string Param_Focus;
protected:

public:
	ICameraVideoGrabber(){}
	virtual~ICameraVideoGrabber(){}

	virtual int ListDevices()=0;
	virtual core::string GetDeviceName(int id)=0;
	virtual void SetDevice(int id)=0;

	virtual void SetFrameRate(int fps)=0;
	virtual int GetFrameRate()=0;
	
	virtual bool InitDevice(int device,int w,int h,int fps)=0;
	virtual void Stop()=0;
	virtual void Start()=0;

	virtual void SetParameter(const core::string& name,const core::string& value){}
	virtual core::string GetParameter(const core::string& name){return core::string::Empty;}
};

}
}

#endif
