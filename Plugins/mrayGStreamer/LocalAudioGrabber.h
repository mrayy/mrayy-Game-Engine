

#ifndef __LOCALAUDIOGRABBER__
#define __LOCALAUDIOGRABBER__

#include "CustomAudioGrabber.h"
#include "mString.h"

namespace mray
{
namespace video
{
class LocalAudioGrabber:public CustomAudioGrabber
{
protected:
	core::string deviceGUID;
public:
	LocalAudioGrabber();
	virtual ~LocalAudioGrabber();

	void Init(const core::string &deviceGUID, int channels, int samplingrate);
};

}
}

#endif
