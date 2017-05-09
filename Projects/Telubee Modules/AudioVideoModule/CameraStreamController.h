

#ifndef __CAMERASTREAMCONTROLLER__
#define __CAMERASTREAMCONTROLLER__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "OVRvisionCamGrabber.h"
#include "OVRVisionCam.h"
#include "capDevice.h"

#include "EyegazeCameraVideoSrc.h"

namespace mray
{
namespace TBee
{


enum class ECameraType
{
	Webcam,
	PointGrey,
	OvrvisionCompressed,
	Ovrvision
};
struct _CameraInfo
{
public:
	CameraInfo ifo;
	int w, h, fps;
	//GCPtr<video::ICameraVideoGrabber> camera;
	math::vector2di offsets;
};

class ICameraSrcController
{
public:
	std::vector<_CameraInfo> cams;
	ECameraType type;

	virtual ~ICameraSrcController(){}

	virtual void SetCameras(std::vector<_CameraInfo> c, ECameraType type){ this->cams = c; this->type = type; }
	virtual void Start() {}
	virtual void Stop() {}
	virtual void SetCameraParameterValue(const core::string& name, const core::string& value){}
	virtual core::string GetCameraParameterValue(const core::string& name, int i){ return ""; }
	virtual video::ICustomVideoSrc* CreateVideoSrc() = 0;

	virtual void Update(){}

	virtual void DebugRender(ServiceRenderContext* context){};

	virtual bool IsStereo(){ return true; }

};

class CameraGrabberController :public ICameraSrcController
{
public:
	std::vector<GCPtr<video::ICameraVideoGrabber>> cameras;

	~CameraGrabberController()
	{
		Stop();
	}
	virtual void SetCameras(std::vector<_CameraInfo> c, ECameraType type)
	{
		ICameraSrcController::SetCameras(c, type);
		cameras.resize(c.size());

		for (int i = 0; i < c.size(); ++i)
		{
			if (c[i].ifo.index >= 0)
			{
				if (type == ECameraType::Ovrvision)
				{
					cameras[i] = new video::OVRVisionCam();
					cameras[i]->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);//1280, 720
				}
				else if (type == ECameraType::OvrvisionCompressed)
				{
					cameras[i] = new video::OVRvisionCamGrabber();
					cameras[i]->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);//1280, 720
				}
				else if (type == ECameraType::Webcam )
				{
					cameras[i] = new video::DirectShowVideoGrabber();
					cameras[i]->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);//1280, 720
				}
#if USE_POINTGREY
				else if (type == ECameraType::PointGrey)
				{
					printf("Initializing Pointgrey Camera\n");
					video::FlyCameraVideoGrabber* cam = 0;
					cameras[i]= (cam = new video::FlyCameraVideoGrabber());
					cam->SetCroppingOffset(c[i].offsets.x, c[i].offsets.y);
					cameras[i]->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);
					cameras[i]->SetImageFormat(video::EPixel_R8G8B8);
					//	m_cameraIfo[0].camera->Start();
				}
#endif
			}
		}
	}
	virtual void Start()
	{
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
				cameras[i]->Start();
		}
	}
	virtual void Stop()
	{
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
				cameras[i]->Stop();
		}
	}
	virtual void SetCameraParameterValue(const core::string& name, const core::string& value)
	{
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
			{
				cameras[i]->SetParameter(name, value);

				printf("Camera [%d] %s value is set to: %s\n", i, name.c_str(), cameras[i]->GetParameter(name).c_str());
			}
		}
	}
	virtual core::string GetCameraParameterValue(const core::string& name, int i)
	{
		if (cameras[i])
		{
			return cameras[i]->GetParameter(name);
		}
		return core::string::Empty;
	}
	video::ICustomVideoSrc* CreateVideoSrc()
	{
		std::vector<video::IVideoGrabber*> grabbers;
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
			{
				grabbers.push_back(cameras[i]);
			}
		}

		video::AppSrcVideoSrc* src = new video::AppSrcVideoSrc();
		src->SetVideoGrabber(grabbers);//

		return src;
	}

	virtual void Update()
	{
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i] && cameras[i]->IsConnected() == false)
			{
				//	printf("Camera %d has stopped, restarting it\n", i);
				cameras[i]->Start();

				//give the camera sometime to start
				OS::IThreadManager::getInstance().sleep(30);
			}
		}
	}
	void DebugRender(ServiceRenderContext* context)
	{

		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
			{
				core::string msg;
				if (cameras[i] && cameras[i]->IsConnected() == false)
				{
					msg = "Camera: " + core::StringConverter::toString(i) + " is not open!";
					context->RenderText(msg, 100, 0, video::SColor(1, 0, 0, 1));
				}

				msg = "Capture FPS [" + core::StringConverter::toString(i) + "]: " + core::StringConverter::toString(cameras[i]->GetCaptureFrameRate());
				context->RenderText(msg, 0, 0);

			}
		}
		//if (m_cameraType == ECameraType::PointGrey)
		{
			for (int i = 0; i < 2; ++i)
			{
			}
		}
	}

	virtual bool IsStereo(){ return cameras.size()>1; }

};

class EncodedCameraStreamController :public ICameraSrcController
{
	std::vector<int> _captureDevices;
	capDeviceInput* _capDev;
	bool _eyegaze;
	core::string _GetParameter(int device,const core::string& name)
	{
		if (device == -1 )
			return "";
		long min; long max; long SteppingDelta; long currentValue = 0; long flags; long defaultValue;
		if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Exposure))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propExposure, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Brightness))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propBrightness, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Contrast))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propContrast, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Hue))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propHue, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Saturation))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propSaturation, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Sharpness))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propSharpness, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_ColorEnable))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propColorEnable, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_WhiteBalance))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propWhiteBalance, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_BacklightCompensation))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propBacklightCompensation, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Pan))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propPan, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Tilt))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propTilt, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Roll))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propRoll, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Zoom))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propZoom, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Iris))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propIris, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Focus))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propFocus, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Gamma))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propGamma, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Gamma))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propGamma, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Gain))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propGain, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Gamma))
		{
			_capDev->getVideoSettingFilter(device, _capDev->propGamma, min, max, SteppingDelta, currentValue, flags, defaultValue);
		}

		return core::StringConverter::toString(currentValue);
	}
	void _setParam(int device,const core::string& name, const core::string& value)
	{
		if (device == -1 )
			return;
		if (_GetParameter(device,name) == value)
			return;
		bool isauto = (value == "auto");
		float v = 0;
		if (!isauto)
			v = core::StringConverter::toFloat(value);

		long flags = 0;
		if (isauto)
			flags = 0x1;// VideoProcAmp_Flags_Auto;
		else flags = 0x2;// VideoProcAmp_Flags_Manual;

#define setValue(param)\
		if (isauto)\
		_capDev->setVideoSettingFilter(device, param, v, flags, true); \
		else\
		_capDev->setVideoSettingFilter(device, param, v, flags);

		if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Exposure))
		{
			setValue(_capDev->propExposure);
		}
		else if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Brightness))
		{

			setValue(_capDev->propBrightness);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Contrast))
		{

			setValue(_capDev->propContrast);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Hue))
		{
			setValue(_capDev->propHue);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Saturation))
		{
			setValue(_capDev->propSaturation);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Sharpness))
		{
			setValue(_capDev->propSharpness);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_ColorEnable))
		{
			setValue(_capDev->propColorEnable);

		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_WhiteBalance))
		{
			setValue(_capDev->propWhiteBalance);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_BacklightCompensation))
		{
			setValue(_capDev->propBacklightCompensation);

		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Pan))
		{

			setValue(_capDev->propPan);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Tilt))
		{

			setValue(_capDev->propTilt);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Roll))
		{
			setValue(_capDev->propRoll);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Zoom))
		{
			setValue(_capDev->propZoom);
		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Iris))
		{
			setValue(_capDev->propIris);

		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Focus))
		{
			setValue(_capDev->propFocus);

		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Gain))
		{
			setValue(_capDev->propGain);

		}
		else  if (name.equals_ignore_case(video::ICameraVideoGrabber::Param_Gamma))
		{
			setValue(_capDev->propGamma);

		}
	}
public:
	TBee::TelubeeCameraConfiguration::ECameraCaptureType CaptureType;


	EncodedCameraStreamController(TBee::TelubeeCameraConfiguration::ECameraCaptureType t)
	{
		CaptureType = t;
		_capDev = new capDeviceInput();
		_eyegaze = false;
	}
	virtual ~EncodedCameraStreamController()
	{
		delete _capDev;
	}
	virtual void Start()
	{
		for (int i = 0; i < _captureDevices.size(); ++i)
		{
			_capDev->setupDevice(_captureDevices[i]);
		}
	}
	virtual void Stop() 
	{
		for (int i = 0; i < _captureDevices.size(); ++i)
		{
			_capDev->stopDevice(_captureDevices[i]);
		}
	}

	void EnableEyegaze(bool e)
	{
		_eyegaze = e;
	}

	video::ICustomVideoSrc* CreateVideoSrc()
	{
		for (int i = 0; i < cams.size(); ++i)
		{
			if (cams[i].ifo.index>=0)
			{
				_captureDevices.push_back(cams[i].ifo.index);
				
			}
		}
		video::ICustomVideoSrc* ret = 0;
		if (true)
		{
			video::CameraVideoSrc* src;
			src = new video::CameraVideoSrc();
			src->SetCameraIndex(_captureDevices);
			if (CaptureType == TBee::TelubeeCameraConfiguration::CaptureJpeg)
			{
				src->SetCaptureType("JPEG");
				src->SetEncoderType("H264");
			}
			else  if (CaptureType == TBee::TelubeeCameraConfiguration::CaptureH264)
			{
				src->SetCaptureType("H264");
				src->SetEncoderType("H264");
			}
			else {
				src->SetCaptureType("RAW");
				src->SetEncoderType("H264");
			}

			ret = src;
		}
		else
		{
			video::AppSrcVideoSrc* src;
			src = new video::AppSrcVideoSrc();

			std::vector<video::IVideoGrabber*> devices;
			for (int i = 0; i < _captureDevices.size(); ++i)
			{
				video::DirectShowVideoGrabber* dev = new video::DirectShowVideoGrabber();
				devices.push_back( dev);
				dev->SetDevice(_captureDevices[i]);
			}

			src->SetVideoGrabber(devices);

			ret = src;
		}

		if (_eyegaze)
		{
			video::EyegazeCameraVideoSrc* res = new video::EyegazeCameraVideoSrc();
			res->SetCameraSource(ret);
			return res;
		}

		return ret;
	}
	virtual bool IsStereo(){ 
		int count = 0;
		for (int i = 0; i<cams.size(); ++i)
		{
			count += (cams[i].ifo.index >= 0?1:0);
		}
		return count > 1; 
	}
	virtual void SetCameraParameterValue(const core::string& name, const core::string& value)
	{
		for (int i = 0; i < _captureDevices.size(); ++i)
		{
			_setParam(_captureDevices[i], name, value);
			
		}
	}
	virtual core::string GetCameraParameterValue(const core::string& name, int i)
	{
		return _GetParameter(_captureDevices[i],name);
		
	}
};

}
}

#endif
