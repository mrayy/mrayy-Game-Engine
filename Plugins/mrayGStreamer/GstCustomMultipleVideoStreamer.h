




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
	void BindPorts(const core::string& addr, uint baseVideoPort,uint clockPort, bool rtcp);
	void SetBitRate(int bitRate);
	void SetResolution(int width, int height,int fps,bool freeSize);

	bool CreateStream();
	void Stream();
	bool IsStreaming();
	void Stop();
	virtual void Close();

	void SetVideoGrabber(const std::vector<IVideoGrabber*> &grabbers);

	virtual void SetPaused(bool paused);
	virtual bool IsPaused();
};

}
}

#endif // GstCustomVideoStreamer_h__
