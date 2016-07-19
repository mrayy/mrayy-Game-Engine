
#include "stdafx.h"
#include "OVRVisionCam.h"
#include "IThreadManager.h"
#include <ovrvision\ovrvision_pro.h>


namespace mray
{
namespace video
{
class OVRVisionCamData
{
	static OVRVisionCamData* _instance;
	static int _refcounter;
	OS::IMutex* _lock;
	OS::IMutex* _datalock;
public:
	OVRVisionCamData()
	{
		fps = 0;
		hasNewFrame[0] = hasNewFrame[1] = false;
		BufferID = 0;
		_inited = false;
		_lastTime = 0;
		type = OVR::OV_CAMVR_FULL;
		_lock = OS::IThreadManager::getInstance().createMutex();
		_datalock = OS::IThreadManager::getInstance().createMutex();
	}
	~OVRVisionCamData()
	{
		delete _lock;
		delete _datalock;
	}
	OVR::OvrvisionPro ovr_Ovrvision;

	math::vector2di size;
	int fps;
	bool hasNewFrame[2];

	uint BufferID;

	ImageInfo imageData[2];

	core::FPSCalc fpsCalc;
	bool _inited;

	ulong _lastTime;

	OVR::Camprop type;
	int _w, _h, _fps;
	bool isOpen()
	{
		return ovr_Ovrvision.isOpen();
	}
	bool Init(int w, int h, int fps)
	{
		_w = w; _h = h; _fps = _fps;
		if ( w < 800)
			type = OVR::OV_CAMVR_VGA;
		else if (w >= 800 && w < 1000)
			type = OVR::OV_CAMVR_FULL;
		else
			type = OVR::OV_CAMVR_WIDE;
		if (_inited)
			return true;
		try{
			if (!ovr_Ovrvision.Open(0, type, 0, NULL))
				return false;
		}
		catch (int e)
		{
			return false;
		}
		_inited = true;
		size.x = ovr_Ovrvision.GetCamWidth();
		size.y = ovr_Ovrvision.GetCamHeight();
		fps = ovr_Ovrvision.GetCamFramerate();

		ovr_Ovrvision.Close();
		hasNewFrame[0] = hasNewFrame[1] = true;

		return true;

	}
	bool Open()
	{
		if (!_inited)
			Init(_w,_h,_fps);
		if (isOpen())return true;
		try{
			if (!ovr_Ovrvision.Open(0, type, 0, NULL))
				return false;
		}
		catch (int e)
		{
			return false;
		}

		OS::IThreadManager::getInstance().sleep(200);
		if (!ovr_Ovrvision.isOpen())
			return false;

		ovr_Ovrvision.SetCameraSyncMode(false);



		imageData[0].createData(math::vector3di(size.x, size.y, 1), video::EPixel_B8G8R8A8);
		imageData[1].createData(math::vector3di(size.x, size.y, 1), video::EPixel_B8G8R8A8);
		fpsCalc.resetTime(gEngine.getTimer()->getSeconds());
		ovr_Ovrvision.SetCallbackImageFunction(OVRVisionCamData::onNewFrame, this);

		_lastTime = gEngine.getTimer()->getMilliseconds();
		hasNewFrame[0] = hasNewFrame[1] = true;
		NewFrame();

		return true;
	}
	void Close()
	{
		if (!isOpen())
			return;
		ovr_Ovrvision.Close();
		BufferID = 0;
		size = 0;
		hasNewFrame[0] = hasNewFrame[1] = false;
		imageData[0].clear();
		imageData[1].clear();
		_inited = false;

	}

	static OVRVisionCamData* Instance()
	{
		return _instance;
	}

	void GrabImage(int i)
	{
		_lock->lock();
		hasNewFrame[i] = false;
		_lock->unlock();
	}

	bool HasNewFrame(int i)
	{
		return hasNewFrame[i];
	}

	void NewFrame()
	{
		++BufferID;
		ovr_Ovrvision.PreStoreCamData(OVR::Camqt::OV_CAMQT_DMS);
		fpsCalc.regFrame(gEngine.getTimer()->getSeconds());
		_lock->lock();
		hasNewFrame[0] = hasNewFrame[1] = true;
		_lock->unlock();
	}

	static void onNewFrame(void*)
	{
		if (!_instance)
			return;
		_instance->NewFrame();
	}

	static void Ref()
	{
		if (!_instance)
		{
			_instance = new OVRVisionCamData();
		}
		++_refcounter;
	}
	static void Release()
	{
		--_refcounter;
		if (_refcounter == 0)
		{
			delete _instance;
		}
	}
};
OVRVisionCamData* OVRVisionCamData::_instance = 0;
int OVRVisionCamData::_refcounter= 0;


OVRVisionCam::OVRVisionCam()
{
	OVRVisionCamData::Ref();
}
OVRVisionCam::~OVRVisionCam()
{
	OVRVisionCamData::Release();
}


int OVRVisionCam::ListDevices()
{
	return 0;
}
core::string OVRVisionCam::GetDeviceName(int id)
{
	return "OVRVision";
}
void OVRVisionCam::SetDevice(int id)
{

}

void OVRVisionCam::SetFrameRate(int fps)
{

}
int OVRVisionCam::GetFrameRate()
{
	return OVRVisionCamData::Instance()->fps;
}

bool OVRVisionCam::InitDevice(int device, int w, int h, int fps)
{
	_index = device;
	OVRVisionCamData::Instance()->Init(w,h,fps);
	return true;
}
void OVRVisionCam::Stop()
{
	try{
		return OVRVisionCamData::Instance()->Close();
	}
	catch (int e)
	{
	}

}
void OVRVisionCam::Start()
{

	OVRVisionCamData::Instance()->Open();


}
bool OVRVisionCam::IsConnected()
{
	return OVRVisionCamData::Instance()->isOpen();
}


void OVRVisionCam::SetFrameSize(int w, int h)
{

}
const math::vector2di& OVRVisionCam::GetFrameSize()
{
	return OVRVisionCamData::Instance()->size;
}

void OVRVisionCam::SetImageFormat(video::EPixelFormat fmt)
{

}
video::EPixelFormat OVRVisionCam::GetImageFormat()
{
	return video::EPixelFormat::EPixel_B8G8R8A8;
}

int OVRVisionCam::GetFramesCount()
{ 
	return 1; 
}
bool OVRVisionCam::GrabFrame(int _i)
{
	OVR::Cameye eyes[2] = { OVR::OV_CAMEYE_LEFT, OVR::OV_CAMEYE_RIGHT };
	if (OVRVisionCamData::Instance()->isOpen() && OVRVisionCamData::Instance()->HasNewFrame(_index))
	{
		OVRVisionCamData::Instance()->GrabImage(_index);
		//This function gets data from OvrPro inside.
		uchar* data = OVRVisionCamData::Instance()->ovr_Ovrvision.GetCamImageBGRA(eyes[_index]);

		OVRVisionCamData::Instance()->imageData[_index].setData(data, math::vector3di(OVRVisionCamData::Instance()->size.x, OVRVisionCamData::Instance()->size.y, 1), video::EPixel_B8G8R8A8);

		return true;
	}
	return false;
}
bool OVRVisionCam::HasNewFrame(int i)
{
	return OVRVisionCamData::Instance()->HasNewFrame(_index);
}
ulong OVRVisionCam::GetBufferID(int i)
{
	return OVRVisionCamData::Instance()->BufferID;
}
float OVRVisionCam::GetCaptureFrameRate(int i)
{
	return OVRVisionCamData::Instance()->fpsCalc.getFPS();
}
const ImageInfo* OVRVisionCam::GetLastFrame(int i)
{
	return &OVRVisionCamData::Instance()->imageData[_index];
}

void OVRVisionCam::SetParameter(const core::string& name, const core::string& value){}
core::string OVRVisionCam::GetParameter(const core::string& name){ return core::string::Empty; }

}
}
