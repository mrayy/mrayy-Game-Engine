

#ifndef __CAMERASTREAMCONTROLLER__
#define __CAMERASTREAMCONTROLLER__

// Created: 2015/12/27
// Author: MHD Yamen Saraiji

#include "OVRvisionCamGrabber.h"
#include "OVRVisionCam.h"

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
public:
	TBee::TelubeeCameraConfiguration::ECameraCaptureType CaptureType;

	EncodedCameraStreamController(TBee::TelubeeCameraConfiguration::ECameraCaptureType t)
	{
		CaptureType = t;
	}

	video::ICustomVideoSrc* CreateVideoSrc()
	{
		std::vector<int> grabbers;
		for (int i = 0; i < cams.size(); ++i)
		{
			if (cams[i].ifo.index>=0)
			{
				grabbers.push_back(cams[i].ifo.index);
			}
		}

		video::CameraVideoSrc* src = new video::CameraVideoSrc();
		src->SetCameraIndex(grabbers);
		if (CaptureType == TBee::TelubeeCameraConfiguration::CaptureJpeg)
		{
			src->SetCaptureType("JPEG");
			src->SetEncodeType("H264");
		}
		else  if (CaptureType == TBee::TelubeeCameraConfiguration::CaptureH264)
		{
			src->SetCaptureType("H264");
			src->SetEncodeType("H264");
		}
		else {
			src->SetCaptureType("RAW");
			src->SetEncodeType("H264");
		}

		return src;
	}
	virtual bool IsStereo(){ 
		int count = 0;
		for (int i = 0; i<cams.size(); ++i)
		{
			count += (cams[i].ifo.index >= 0?1:0);
		}
		return count > 1; 
	}
};

}
}

#endif
