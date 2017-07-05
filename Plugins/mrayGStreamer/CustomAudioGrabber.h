

#ifndef __CustomAudioGrabber__
#define __CustomAudioGrabber__

#include "IAudioGrabber.h"
#include "mString.h"

namespace mray
{
namespace video
{
class CustomAudioGrabberImpl;
class CustomAudioGrabber :public IAudioGrabber
{
protected:
	CustomAudioGrabberImpl* m_impl;
public:
	CustomAudioGrabber();
	virtual ~CustomAudioGrabber();

	virtual void Init(const core::string &pipeline, int channels, int samplingrate);

	virtual bool Start();
	virtual void Pause();
	virtual void Close();
	virtual bool IsStarted();

	virtual uint GetSamplingRate();
	virtual uint GetChannelsCount();

	virtual bool GrabFrame();
	virtual uchar* GetAudioFrame();
	virtual uint GetAudioFrameSize();

};

}
}

#endif
