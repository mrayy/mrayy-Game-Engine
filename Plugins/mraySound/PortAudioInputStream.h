

/********************************************************************
	created:	2012/11/18
	created:	18:11:2012   0:37
	filename: 	D:\Development\mrayEngine\Plugins\mraySound\PortAudioInputStream.h
	file path:	D:\Development\mrayEngine\Plugins\mraySound
	file base:	PortAudioInputStream
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __PortAudioInputStream__
#define __PortAudioInputStream__

#include "IInputSoundStream.h"


namespace mray
{
namespace sound
{

class PortAudioInputStream:public IInputSoundStream
{
protected:
public:
	PortAudioInputStream();
	virtual~PortAudioInputStream();

	bool CreateInputStream(uchar numChannels,uint samplesPerSec,uint avgBytesPerSec,uchar bitsPerSample);
	bool StartInputStream();
	void CaptureSamples(std::vector<uchar>& buffer);
	void StopInputStream();

	bool CanStream();
	bool IsStreaming();
};

}
}


#endif
