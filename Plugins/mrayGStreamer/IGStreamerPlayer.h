

/********************************************************************
	created:	2014/02/22
	created:	22:2:2014   17:54
	filename: 	C:\Development\mrayEngine\Plugins\mrayGStreamer\IGStreamerPlayer.h
	file path:	C:\Development\mrayEngine\Plugins\mrayGStreamer
	file base:	IGStreamerPlayer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IGStreamerPlayer__
#define __IGStreamerPlayer__


#include "mString.h"
#include "IStreamListener.h"
#include "ListenerContainer.h"


namespace mray
{
namespace video
{
	class GstPipelineHandler;

class IGStreamerPlayer :public ListenerContainer<IGStreamerPlayerListener*>
{
protected:
	DECLARE_FIRE_METHOD(OnPlayerStarted, (IGStreamerPlayer* s), (s));
	DECLARE_FIRE_METHOD(OnPlayerStopped, (IGStreamerPlayer* s), (s));
	DECLARE_FIRE_METHOD(OnPlayerReady, (IGStreamerPlayer* s), (s));

	virtual GstPipelineHandler* GetPipeline() = 0;
public:
	IGStreamerPlayer(){}
	virtual~IGStreamerPlayer(){}

	virtual bool IsStream() = 0;
	virtual void SetVolume(float vol) = 0;

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual bool IsLoaded() = 0;
	virtual bool IsPlaying() = 0;
	virtual void Close() = 0;

	virtual bool QueryLatency(bool &isLive, ulong& minLatency, ulong& maxLatency);

	virtual void SetClockBase(ulong c);
	virtual ulong GetClockBase();
};

}
}


#endif
