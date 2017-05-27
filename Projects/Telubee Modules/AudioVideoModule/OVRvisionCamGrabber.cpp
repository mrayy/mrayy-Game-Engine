

#include "stdafx.h"
#include "OVRvisionCamGrabber.h"
#include "IThreadManager.h"
#include "PixelUtil.h"
#include <ovrvision\ovrvision_pro.h>



namespace mray
{
namespace video
{

class OVRvisionCamGrabberData
{
	static OVRvisionCamGrabberData* _instance;
	static int _refcounter;

	bool _inited;

	OVRvisionCamGrabberData()
	{
		BufferID = 0;
		hasNewFrame = 0;
		_lastTime = 0;
		fps = 0;
		_inited = false;
		_remapData = 0;

		type = OVR::OV_CAMVR_VGA;

		_lock = OS::IThreadManager::getInstance().createMutex();
		_datalock = OS::IThreadManager::getInstance().createMutex();
	}

	OS::IMutex* _lock;
	OS::IMutex* _datalock;
public:
	~OVRvisionCamGrabberData()
	{
		delete _lock;
		delete _datalock;
	}

	OVR::OvrvisionPro ovr;
	bool hasNewFrame;

	uint BufferID;

	ImageInfo imageData;

	core::FPSCalc fpsCalc;

	ulong _lastTime;

	math::vector2di size,flatsize;
	int fps;

	OVR::Camprop type ;

	uchar* _remapData;

	video::EPixelFormat imageFormat;
	int _w, _h, _fps;

	bool isOpen()
	{
		return ovr.isOpen();
	}
	bool Init(int w, int h, int fps)
	{
		if (_inited)
			return true;
		_w = w; _h = h; _fps = _fps;
		if (w ==640)
			type = OVR::OV_CAMVR_VGA;
		else if (w ==960)
			type = OVR::OV_CAMVR_FULL;
		else if (w==1280)
			type = OVR::OV_CAMVR_WIDE;
		else return false;

		try{
			if (!ovr.Open(0, type, 0, NULL, true))
				return false;
		}
		catch (int e)
		{
			return false;
		}
		OS::IThreadManager::getInstance().sleep(500);
		_inited = true;
		size.x = ovr.GetCamWidth();
		size.y = ovr.GetCamHeight();
		fps = ovr.GetCamFramerate();
		/*size.y *= 2;
		
		if (size.x == 640)
			size.y = 320;
		else if (size.y == 950)
			size.x = 640;*/

		flatsize = size;

		imageFormat = video::EPixel_Alpha8;
		int len = size.x*size.y*PixelUtil::getPixelDescription(imageFormat).elemSizeB;
		flatsize.x *= 2 / PixelUtil::getPixelDescription(imageFormat).elemSizeB;
		_remapData = new uchar[len];

		ovr.Close();
		OS::IThreadManager::getInstance().sleep(300);
		hasNewFrame = true;

		return true;

	}
	bool Open()
	{
		if (!_inited)
			Init(_w, _h, _fps);
		if (isOpen())return true;
		try{
			if (!ovr.Open(0, type, 0, NULL, true))
				return false;
		}
		catch (int e)
		{
			return false;
		}

		OS::IThreadManager::getInstance().sleep(200);
		if (!ovr.isOpen())
			return false;

		ovr.SetCameraSyncMode(false);

		hasNewFrame = true;
		ovr.SetCallbackImageFunction(OVRvisionCamGrabberData::onNewFrame, this);
		imageData.createData(math::vector3di(size.x, size.y, 1), imageFormat);
		fpsCalc.resetTime(gEngine.getTimer()->getSeconds());
		_lastTime = gEngine.getTimer()->getMilliseconds();

		return true;
	}
	void Close()
	{
		if (!isOpen())
			return;
		ovr.SetCallbackImageFunction(0, 0);
		ovr.Close();
		BufferID = 0;
		size = 0;
		hasNewFrame= false;
		imageData.clear();
		_inited = false;
		delete[] _remapData;
		_remapData = 0;

	}

	bool HasNewFrame()
	{
		return hasNewFrame;
	}

	static void onNewFrame(void*)
	{
		Instance()->_lock->lock();
		Instance()->hasNewFrame = true;
		Instance()->_lock->unlock();
	}


	void RemapData(ushort* data,uchar*dest)
	{
		int w = ovr.GetCamWidth();
		int h = ovr.GetCamHeight();
		int buffersize=ovr.GetCamBuffersize();
		int offset = w*h;
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int ps = (y * w) + x;
				dest[ps] = (data[ps] & 0x00FF);
				dest[ps + offset] =  (data[ps] >> 8);
			}
		}

	}

	bool GrabFrame()
	{
		if (isOpen() && HasNewFrame())
		{
			fpsCalc.regFrame(gEngine.getTimer()->getSeconds());
			_lock->lock();
			hasNewFrame = false;
			_lock->unlock();
			//Full Draw
			ovr.CaptureImage();
			//This function gets data from OvrPro inside.
			ushort* data = (ushort*)ovr.GetFrame();
			RemapData(data, _remapData);

			_datalock->lock();
			imageData.setData(_remapData, math::vector3di(size.x, size.y, 1), imageFormat);
			_datalock->unlock();
			return true;
		}
		return false;
	}

	void Lock()
	{
		_datalock->lock();
	}
	void Unlock()
	{
		_datalock->unlock();
	}

	static OVRvisionCamGrabberData* Instance()
	{
		return _instance;
	}
	static void Ref()
	{
		if (!_instance)
		{
			_instance = new OVRvisionCamGrabberData();
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
OVRvisionCamGrabberData* OVRvisionCamGrabberData::_instance = 0;
int OVRvisionCamGrabberData::_refcounter = 0;




OVRvisionCamGrabber::OVRvisionCamGrabber()
{
	OVRvisionCamGrabberData::Ref();
}

OVRvisionCamGrabber::~OVRvisionCamGrabber()
{
	OVRvisionCamGrabberData::Release();
}



int OVRvisionCamGrabber::ListDevices()
{
	return 0;
}

core::string OVRvisionCamGrabber::GetDeviceName(int id)
{
	return "ovrvision";
}

void OVRvisionCamGrabber::SetDevice(int id)
{
}


void OVRvisionCamGrabber::SetFrameRate(int fps)
{
}

int OVRvisionCamGrabber::GetFrameRate()
{
	return OVRvisionCamGrabberData::Instance()->fps;
}


bool OVRvisionCamGrabber::InitDevice(int device, int w, int h, int fps)
{
	OVRvisionCamGrabberData::Instance()->Init(w,h,fps);
	return true;
}

void OVRvisionCamGrabber::Stop()
{
	try{
		return OVRvisionCamGrabberData::Instance()->Close();
	}
	catch (int e)
	{
	}
}

void OVRvisionCamGrabber::Start()
{
	OVRvisionCamGrabberData::Instance()->Open();
}

bool OVRvisionCamGrabber::IsConnected()
{
	return OVRvisionCamGrabberData::Instance()->isOpen();
}



void OVRvisionCamGrabber::SetFrameSize(int w, int h)
{
}

const math::vector2di& OVRvisionCamGrabber::GetFrameSize()
{
	return OVRvisionCamGrabberData::Instance()->flatsize;
}


void OVRvisionCamGrabber::SetImageFormat(video::EPixelFormat fmt)
{
}

video::EPixelFormat OVRvisionCamGrabber::GetImageFormat()
{
	return OVRvisionCamGrabberData::Instance()->imageFormat;
}


int OVRvisionCamGrabber::GetFramesCount()
{
	return 1;
}

bool OVRvisionCamGrabber::GrabFrame(int i)
{
	return OVRvisionCamGrabberData::Instance()->GrabFrame();
}

bool OVRvisionCamGrabber::HasNewFrame(int i)
{
	return OVRvisionCamGrabberData::Instance()->HasNewFrame();
}

ulong OVRvisionCamGrabber::GetBufferID(int i)
{
	return OVRvisionCamGrabberData::Instance()->BufferID;
}

float OVRvisionCamGrabber::GetCaptureFrameRate(int i)
{
	return OVRvisionCamGrabberData::Instance()->fpsCalc.getFPS();
}



const ImageInfo* OVRvisionCamGrabber::GetLastFrame(int index)
{
	return &OVRvisionCamGrabberData::Instance()->imageData;
}


void OVRvisionCamGrabber::Lock()
{
	OVRvisionCamGrabberData::Instance()->Lock();
}
void OVRvisionCamGrabber::Unlock()
{
	OVRvisionCamGrabberData::Instance()->Unlock();
}
void OVRvisionCamGrabber::SetParameter(const core::string& name, const core::string& value)
{
}

core::string OVRvisionCamGrabber::GetParameter(const core::string& name)
{
	return core::string::Empty;
}

}
}
