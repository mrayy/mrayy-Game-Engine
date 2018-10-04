

#ifndef __CAMERASTREAMCONTROLLER__
#define __CAMERASTREAMCONTROLLER__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "OVRvisionCamGrabber.h"
#include "OVRVisionCam.h"
#include "capDevice.h"

#include "EyegazeCameraVideoSrc.h"
#include "OVRVisionEyegazeCameraVideoSrc.h"

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

	virtual void SetResolution(int w, int h){}
	virtual void SetCameras(std::vector<_CameraInfo> c, ECameraType type){ this->cams = c; this->type = type; }
	virtual void Start() {}
	virtual void Stop() {}
	virtual void OnStreamStarted(){}
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
	std::vector<_CameraInfo> cIfo;
	~CameraGrabberController()
	{
		Stop();
	}
	virtual void SetCameras(std::vector<_CameraInfo> c, ECameraType type)
	{
		ICameraSrcController::SetCameras(c, type);
		cameras.clear();
		cIfo.clear();
		for (int i = 0; i < c.size(); ++i)
		{
			if (c[i].ifo.index >= 0)
			{
				cIfo.push_back(c[i]);
				video::ICameraVideoGrabber* cam;
#ifdef OVRVISION_SUPPORT
				if (type == ECameraType::Ovrvision)
				{
					cam = new video::OVRVisionCam();
					cam->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);//1280, 720
				}
				else if (type == ECameraType::OvrvisionCompressed)
				{
					cam = new video::OVRvisionCamGrabber();
					cam->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);//1280, 720
				}
				else
#endif
				if (type == ECameraType::Webcam )
				{
					cam = new video::DirectShowVideoGrabber();
					//cameras[i]->InitDevice(c[i].ifo.index, c[i].w, c[i].h, c[i].fps);//1280, 720
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

				cameras.push_back(cam);
			}
		}
	}
	virtual void Start()
	{
		gLogManager.log("Initing cameras.", ELL_INFO);
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
			{
				if (!cameras[i]->InitDevice(cIfo[i].ifo.index, cIfo[i].w, cIfo[i].h, cIfo[i].fps))
				{
					gLogManager.log("Failed to init camera index:" + core::StringConverter::toString(cIfo[i].ifo.index), ELL_ERROR);
				}
				cameras[i]->Start();
			}
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
		gLogManager.log("Creating src",ELL_INFO);
		std::vector<video::IVideoGrabber*> grabbers;
		for (int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i])
			{
				grabbers.push_back(cameras[i]);
			}
		}
		gLogManager.log("setting video grabber", ELL_INFO);

		video::AppSrcVideoSrc* src = new video::AppSrcVideoSrc();
		src->SetVideoGrabber(grabbers);//
		gLogManager.log("done", ELL_INFO);

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
		if (device <0 )
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


class ImageProcessorListener :public IMyListenerCallback
{
public:
	 int width,height;
	 uchar* buffer;
	ImageProcessorListener()
	{
		buffer = 0;
	}
	~ImageProcessorListener()
	{
		delete[]buffer;
	}

	void Init(int w, int h)
	{
		width = w;
		height = h;
		buffer = new uchar[w*h ];
	}
	virtual void ListenerOnDataChained(_GstMyListener* src, GstBuffer * bfr)
	{
		if (!buffer)
			return;
		GstMapInfo map;
		gst_buffer_map(bfr, &map, (GstMapFlags)(GST_MAP_READ));
		ushort*data = (ushort*)map.data;
		if (!data){
			gst_buffer_unmap(bfr, &map);
			return;
		}
		int count = width*height;
		uchar* ptr = (uchar*)data;

		//First byte lossless compression
		//second byte, lossy
		/*for (int ps = 0; ps < count; ++ps) {
			ptr[ps*2+1] = 0;
		}*/
		
		//rearrange the byte coding for the left and right images of OVRVision Pro camera
		for (int ps = 0; ps < count; ++ps) {
			buffer[ps] = ptr[ps*2 + 1];
			ptr[ps] = ptr[ps*2];
		}
		memcpy(ptr + count, buffer, count);
		gst_buffer_unmap(bfr, &map);

	}
};
public:
	TBee::TelubeeCameraConfiguration::ECameraCaptureType CaptureType;
	ECameraType camtype;
	video::OVRvisionCamGrabber* _ovrCam;
	ImageProcessorListener _ovrListener;
	core::string _ovrSettings;

	bool optimizeOVR;

	bool ksSupport;
	EncodedCameraStreamController(TBee::TelubeeCameraConfiguration::ECameraCaptureType t, ECameraType cam)
	{
		ksSupport = true;
		camtype = cam;
		CaptureType = t;
		_capDev = new capDeviceInput();
		_eyegaze = false;
		_ovrCam = 0;
		optimizeOVR = false;

	}
	virtual ~EncodedCameraStreamController()
	{
		delete _capDev;
		delete _ovrCam;
	}
	void EnableKernelStreaming(bool ks)
	{
		ksSupport = ks;
	}

	virtual void Start()
	{
		gLogManager.log("Starting camera grabber", ELL_INFO);
		for (int i = 0; i < _captureDevices.size(); ++i)
		{
			if (_captureDevices[i]>=0)
				_capDev->setupDevice(_captureDevices[i]);
		}
		gLogManager.log("Done camera grabber", ELL_INFO);
	}
	virtual void Stop() 
	{
		for (int i = 0; i < _captureDevices.size(); ++i)
		{
			if (_captureDevices[i] >= 0)
				_capDev->stopDevice(_captureDevices[i]);
		}
	}

	virtual void OnStreamStarted()
	{
		if (camtype == ECameraType::Ovrvision ||
			camtype == ECameraType::OvrvisionCompressed)
		{
			gLogManager.log("begin OVRvision Settings", ELL_INFO);
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Exposure, "1700");
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gain, "12");
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Gamma, "1200");
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_Brightness, "9000");
			SetCameraParameterValue(video::ICameraVideoGrabber::Param_WhiteBalance, "2000");
			gLogManager.log("done OVRvision Settings", ELL_INFO);
		}
	}

	void EnableEyegaze(bool e)
	{
		_eyegaze = e;
	}

	void SetResolution(int w, int h)
	{
		if (camtype == ECameraType::Ovrvision ||
			camtype == ECameraType::OvrvisionCompressed)
		{
			gLogManager.log("Begin OVRvision Init", ELL_INFO);
			_ovrCam->InitDevice(0, w, h, 60);
			_ovrListener.Init(w, h);
			gLogManager.log("Done OVRvision Init", ELL_INFO);
		}
	}

	video::ICustomVideoSrc* CreateVideoSrc()
	{
		gLogManager.log("Initing camera grabber", ELL_INFO);
		for (int i = 0; i < cams.size(); ++i)
		{
			//if (cams[i].ifo.index>=0)
			{
				_captureDevices.push_back(cams[i].ifo.index);
				
			}
		}
		video::ICustomVideoSrc* ret = 0;
		if (ksSupport)
		{
			gLogManager.log("Using Kernal Streaming", ELL_INFO);
			video::CameraVideoSrc* src;
			src = new video::CameraVideoSrc();
			gLogManager.log("SetCameraIndex: "+core::StringConverter::toString(_captureDevices[0]), ELL_INFO);
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
				gLogManager.log("RAW/H264", ELL_INFO);
				src->SetCaptureType("RAW");
				src->SetEncoderType("H264");
			}

			if (camtype == ECameraType::Ovrvision ||
				camtype == ECameraType::OvrvisionCompressed)
			{
				gLogManager.log("Begin Linking Overvision",ELL_INFO);
				src->AddPostCaptureListener(&_ovrListener,0);
				src->ConvertToGray8(!optimizeOVR);
				if (!_ovrCam)
				{
					_ovrCam = new video::OVRvisionCamGrabber();
					_ovrCam->InitDevice(0,640,480,90);
					gLogManager.log("Getting Camera Settings", ELL_INFO);
					_ovrSettings = _ovrCam->GetCameraSettings();
					_ovrCam->Stop();
				}
				gLogManager.log("Done Linking Overvision", ELL_INFO);
			}
			ret = src;
		}
		else
		{
			gLogManager.log("Using DirectShow grabber", ELL_INFO);
			video::AppSrcVideoSrc* src;
			src = new video::AppSrcVideoSrc();

			std::vector<video::IVideoGrabber*> devices;
			for (int i = 0; i < _captureDevices.size(); ++i)
			{
				if (_captureDevices[i] >= 0)
				{
					video::DirectShowVideoGrabber* dev = new video::DirectShowVideoGrabber();
					devices.push_back(dev);
					dev->SetDevice(_captureDevices[i]);
				}
				else
				{
					devices.push_back(0);
				}
			}

			gLogManager.log("SetVideoGrabber()", ELL_INFO);
			src->SetVideoGrabber(devices);

			ret = src;
		}
		gLogManager.log("Done initing camera grabber", ELL_INFO);

		if (_eyegaze)
		{

			if (camtype == ECameraType::Ovrvision ||
				camtype == ECameraType::OvrvisionCompressed)
			{
				video::OVRVisionEyegazeCameraVideoSrc* res = new video::OVRVisionEyegazeCameraVideoSrc();
				res->SetCameraSource(ret);
				return res;
			}
			else{
				video::EyegazeCameraVideoSrc* res = new video::EyegazeCameraVideoSrc();
				res->SetCameraSource(ret);
				return res;
			}
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
//			gLogManager.log("Changing Camera Parameter:" + name + " to:" + _GetParameter(_captureDevices[i], video::ICameraVideoGrabber::Param_Gain), ELL_INFO);

		}
	}
	virtual core::string GetCameraParameterValue(const core::string& name, int i)
	{
		if (name == "settings" &&
			(camtype == ECameraType::Ovrvision ||
			camtype == ECameraType::OvrvisionCompressed))
		{
			return _ovrSettings;
		}
		return _GetParameter(_captureDevices[i],name);
		
	}
};

}
}

#endif
