

#include "stdafx.h"
#include "LocalAudioGrabber.h"

namespace mray
{
namespace video
{



LocalAudioGrabber::LocalAudioGrabber()
{
}
LocalAudioGrabber::~LocalAudioGrabber()
{
}

void LocalAudioGrabber::Init(const core::string &deviceGUID, int channels, int samplingrate)
{
	this->deviceGUID = deviceGUID;

	core::string audioStr = "directsoundsrc buffer-time=100  ";

	if (deviceGUID != "")
	{
		audioStr += "device=\"" + deviceGUID + "\"";
	}

	CustomAudioGrabber::Init(audioStr, channels, samplingrate);
}

}
}
