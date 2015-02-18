

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
public:
	GstNetworkVideoStreamer();
	virtual ~GstNetworkVideoStreamer();

	// addr: target address to stream video to
	// videoport: port for the video stream, video rtcp is allocated as videoPort+1/videoPort+2
	void BindPorts(const core::string& addr, int videoPort, bool rtcp);
	void SetResolution(int width, int height,int fps=30);
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
