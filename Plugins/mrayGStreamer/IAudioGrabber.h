

#ifndef __IAUDIOGRABBER__
#define __IAUDIOGRABBER__

#include "mTypes.h"

namespace mray
{
namespace video
{

class IAudioGrabber
{
protected:
public:
	IAudioGrabber(){}
	virtual ~IAudioGrabber(){}

	virtual bool Start() = 0;
	virtual void Pause() = 0;
	virtual void Close() = 0;
	virtual bool IsStarted() = 0;

	virtual uint GetSamplingRate() = 0;
	virtual uint GetChannelsCount() = 0;

	virtual bool GrabFrame() = 0;
	virtual uchar* GetAudioFrame() = 0;
	virtual uint GetAudioFrameSize() = 0;


};

}
}


#endif
