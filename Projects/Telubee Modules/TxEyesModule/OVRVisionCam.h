

#ifndef __OVRVISIONCAM__
#define __OVRVISIONCAM__

#include "ICameraVideoGrabber.h"

namespace mray
{
namespace video
{
	class OVRVisionCamData;
class OVRVisionCam:public ICameraVideoGrabber
{
protected:
	int _index;
public:
	OVRVisionCam();
	virtual ~OVRVisionCam();


	virtual int ListDevices() ;
	virtual core::string GetDeviceName(int id) ;
	virtual void SetDevice(int id) ;

	virtual void SetFrameRate(int fps) ;
	virtual int GetFrameRate() ;

	virtual bool InitDevice(int device, int w, int h, int fps) ;
	virtual void Stop() ;
	virtual void Start() ;
	virtual bool IsConnected() ;


	virtual void SetFrameSize(int w, int h) ;
	virtual const math::vector2di& GetFrameSize() ;

	virtual void SetImageFormat(video::EPixelFormat fmt) ;
	virtual video::EPixelFormat GetImageFormat() ;

	virtual int GetFramesCount();
	virtual bool GrabFrame(int i ) ;
	virtual bool HasNewFrame(int i ) ;
	virtual ulong GetBufferID(int i ) ;// incremented once per frame
	virtual float GetCaptureFrameRate(int i ) ;


	virtual const ImageInfo* GetLastFrame(int index ) ;

	virtual void SetParameter(const core::string& name, const core::string& value);
	virtual core::string GetParameter(const core::string& name);
};

}
}


#endif
