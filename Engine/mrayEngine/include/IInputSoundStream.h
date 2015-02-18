

/********************************************************************
	created:	2011/01/15
	created:	15:1:2011   20:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IInputSoundStream.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IInputSoundStream
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IInputSoundStream_h__
#define IInputSoundStream_h__

#include "mTypes.h"
#include "mString.h"

namespace mray
{
namespace sound
{

	struct InputStreamDeviceInfo
	{
		int ID;
		core::string name;
		core::string description;
	};

class IInputSoundStream
{
private:
protected:
public:
	IInputSoundStream(){}
	virtual ~IInputSoundStream(){}

	virtual void ListDevices(std::vector<InputStreamDeviceInfo> &lst)=0;

	virtual bool CreateInputStream(int device,uchar numChannels,uint samplesPerSec,uint avgBytesPerSec,uchar bitsPerSample)=0;
	virtual bool StartInputStream()=0;
	virtual void CaptureSamples(std::vector<uchar>& buffer)=0;
	virtual void StopInputStream()=0;

	virtual bool CanStream()=0;
	virtual bool IsStreaming()=0;
};

}
}


#endif // IInputSoundStream_h__

