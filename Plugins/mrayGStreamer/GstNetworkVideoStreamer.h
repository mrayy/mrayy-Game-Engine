




/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Plugins\mrayGStreamer\GstCustomVideoStreamer
	file base:	GstCustomVideoStreamer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GstNetworkVideoStreamer_h__
#define GstNetworkVideoStreamer_h__

#include "IGStreamerStreamer.h"
#include "ICustomVideoSrc.h"

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

	friend class GstNetworkVideoStreamerImpl;
	GstPipelineHandler* GetPipeline();
public:
	GstNetworkVideoStreamer();
	virtual ~GstNetworkVideoStreamer();

	// addr: target address to stream video to
	// baseVideoPort: base port for the video streams, 
	void BindPorts(const std::string& addr, uint *videoPorts, uint count, bool rtcp);
	
	void SetVideoSrc(ICustomVideoSrc* src);

	virtual int GetAverageBytesSent();

	bool CreateStream();
	void Stream();
	bool IsStreaming();
	void Stop();
	virtual void Close();

	virtual void SetPaused(bool paused);
	virtual bool IsPaused();
};

}
}

#endif // GstCustomVideoStreamer_h__
