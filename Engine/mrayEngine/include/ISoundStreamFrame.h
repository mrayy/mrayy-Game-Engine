

/********************************************************************
	created:	2011/01/16
	created:	16:1:2011   20:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISoundStreamFrame.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISoundStreamFrame
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ISoundStreamFrame_h__
#define ISoundStreamFrame_h__

#include "IResource.h"
#include "SoundEnums.h"

namespace mray
{
namespace sound
{

class MRAY_DLL ISoundStreamFrame:public IResource
{
protected:
	virtual uint calcSizeInternal()
	{
		return GetSamplesCount()*(GetFormat()==ESF_8?1:2);
	}
public:


	ISoundStreamFrame()
	{}
	virtual~ISoundStreamFrame()
	{
	}

	//data: samples data
	//size: samples count
	//freq: frequency
	//fmt : format of the data(byte/short)
	//channels: number of channels (1/2)
	virtual bool SetStreamFrameData(void* data,uint size,uint freq,ESampleFormat fmt,int channels)=0;

	virtual void* GetSamples()=0;

	virtual uint GetSamplesCount()=0;
	virtual uint GetFrequency()=0;
	virtual uint GetChannels()=0;
	virtual ESampleFormat GetFormat()=0;

	virtual ISoundStreamFrame* duplicate()=0;

	virtual uint getFrameSize(){
		return GetChannels() * (GetFormat()==ESF_8 ? 1:2);
	}


};

MakeSharedPtrType(ISoundStreamFrame)

}
}

#endif // ISoundStreamFrame_h__

