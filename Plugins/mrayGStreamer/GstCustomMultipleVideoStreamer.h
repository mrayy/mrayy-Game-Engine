




/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Plugins\mrayGStreamer\GstCustomVideoStreamer
	file base:	GstCustomVideoStreamer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GstCustomMultipleVideoStreamer_h__
#define GstCustomMultipleVideoStreamer_h__

#include "IGStreamerStreamer.h"
#include "ICustomVideoSrc.h"

namespace mray
{
namespace video
{

	class GstCustomMultipleVideoStreamerImpl;
class IVideoGrabber;

class GstCustomMultipleVideoStreamer :public IGStreamerStreamer
{
protected:
	GstCustomMultipleVideoStreamerImpl* m_impl;

	friend class GstCustomMultipleVideoStreamerImpl;
	GstPipelineHandler* GetPipeline();
public:
	GstCustomMultipleVideoStreamer();
	virtual ~GstCustomMultipleVideoStreamer();

	// addr: target address to stream video to
	// baseVideoPort: base port for the video streams, 
	void BindPorts(const std::string& addr, uint *videoPorts,uint count,uint clockPort, bool rtcp);

	void SetVideoSrc(ICustomVideoSrc* src);


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
