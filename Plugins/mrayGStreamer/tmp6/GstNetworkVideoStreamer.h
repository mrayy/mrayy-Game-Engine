

#ifndef GstNetworkVideoStreamer_h__
#define GstNetworkVideoStreamer_h__

#include "IGStreamerStreamer.h"

namespace mray
{
namespace video
{

class GstNetworkVideoStreamerImpl;
class IVideoGrabber;

class GstNetworkVideoStreamer :public IGStreamerStreamer
{
protected:
	GstNetworkVideoStreamerImpl* m_impl;

	GstPipelineHandler* GetPipeline();
	friend class GstNetworkVideoStreamerImpl;
public:
	GstNetworkVideoStreamer();
	virtual ~GstNetworkVideoStreamer();

	// addr: target address to stream video to
	// videoport: port for the video stream, video rtcp is allocated as videoPort+1/videoPort+2
	void BindPorts(const std::string& addr, uint* videoPort, uint count, bool rtcp);
	void SetCameraResolution(int width, int height, int fps = 30);
	void SetFrameResolution(int width, int height);
	void SetBitRate(int bitRate);

	bool CreateStream();
	void Stream();
	bool IsStreaming();
	void Stop();
	virtual void Close();

	void SetCameras(int cam0, int cam1);
	bool IsStereo();

	virtual void SetPaused(bool paused);
	virtual bool IsPaused();
};

}
}

#endif // GstNetworkVideoStreamer_h__
