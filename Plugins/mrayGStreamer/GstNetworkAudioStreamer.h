


#ifndef GstNetworkAudioStreamer_h__
#define GstNetworkAudioStreamer_h__

#include "IGStreamerStreamer.h"

namespace mray
{
namespace video
{

	class GstNetworkAudioStreamerImpl;

	
class GstNetworkAudioStreamer:public IGStreamerStreamer
{
protected:
	GstNetworkAudioStreamerImpl* m_impl;
	GstPipelineHandler* GetPipeline();
public:

	struct AudioInterface
	{
		AudioInterface() :channelsCount(1){}
		AudioInterface(const std::string& guid, int count) :deviceGUID(guid), channelsCount(count)
		{}
		std::string deviceGUID;
		int channelsCount;
	};
public:
	GstNetworkAudioStreamer();
	virtual ~GstNetworkAudioStreamer();

	// addr: target address to stream video to
	// audioport: port for the audio stream , audio rtcp is allocated as audioPort+1 and audioPort+2
	void BindPorts(const std::string& addr, uint* ports, uint count, bool rtcp);

	void SetAudioInterface(const AudioInterface& interfaces);

	bool CreateStream();
	void Stream();
	bool IsStreaming();
	void Stop();
	virtual void Close();

	virtual void SetPaused(bool paused) ;
	virtual bool IsPaused() ;
};

}
}

#endif // GstNetworkAudioStreamer_h__
