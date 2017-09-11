
#ifndef VideoAppSinkHandler_h__
#define VideoAppSinkHandler_h__

#include "IAppSinkHandler.h"
#include "ImageInfo.h"
#include "IMutex.h"
#include "FPSCalc.h"

namespace mray
{
namespace video
{
class VideoAppSinkHandler :public IAppSinkHandler
{
public:

protected:
	std::shared_ptr<GstSample> 	buffer, prevBuffer;
	GstMapInfo mapinfo;

	video::EPixelFormat m_pixelFormat;
	//quick hack for YUV support: maximum of 3 surfaces
	GstImageFrame m_pixels[1];				// 24 bit: rgb
	GstImageFrame m_backPixels[1];
	bool			m_IsFrameNew;			// if we are new
	bool			m_HavePixelsChanged;
	bool			m_BackPixelsChanged;
	bool			m_IsAllocated;
	OS::IMutex*			m_mutex;
	math::vector2di m_frameSize;
	int m_surfaceCount;

	core::FPSCalc m_fps;

	uint m_frameID;
	bool _Allocate(int width, int height, video::EPixelFormat fmt);
public:
	VideoAppSinkHandler();
	virtual ~VideoAppSinkHandler();

	virtual void Close();
	bool 			isFrameNew(){ return m_IsFrameNew; }
	video::ImageInfo*	getPixelsRef(int surface = 0){ return &m_pixels[surface].data; }
	GstImageFrame*	getPixelFrame(int surface = 0){ return &m_pixels[surface]; }
	int getSurfaceCount(){ return m_surfaceCount; }
	bool GrabFrame();
	uint GetFrameID(){ return m_frameID; }
	virtual const math::vector2di& GetFrameSize(){ return m_frameSize; }

	float GetCaptureFrameRate();

	virtual GstFlowReturn process_sample(std::shared_ptr<GstSample> sample);
};

}
}

#endif // VideoAppSinkHandler_h__
