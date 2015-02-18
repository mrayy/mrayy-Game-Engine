#include "stdafx.h"

#include "IWaveLoader.h"
#include <mstring.h>
#include <IFileSystem.h>
#include <LogManager.h>
#include <ISoundStreamFrame.h>
#include "mraySystem.h"

namespace mray{
namespace loaders{

class WAVSoundStream:public sound::ISoundStreamFrame
{
public:
   byte   *sampleData;
   uint	  dataSize;

   uchar channelsCount;
   uint samplesPerSec;
   sound::ESampleFormat sampleFormat;
	
   WAVSoundStream(const core::string&filename)
	   :channelsCount(0),samplesPerSec(0),sampleFormat(sound::ESF_8),sampleData(0)
   {
	   setResourceName(filename);
   }
	virtual~WAVSoundStream(){
	   if(sampleData)
		   delete [] sampleData;
	}


	virtual bool isStreaming(){return 0;}
	virtual uint GetSamplesCount()
	{
		return dataSize;
	}
	virtual uint GetFrequency()
	{
		return samplesPerSec;
	}
	virtual uint GetChannels()
	{
		return channelsCount;
	}
	virtual sound::ESampleFormat GetFormat()
	{
		return sampleFormat;
	}

	virtual void*GetSamples(){return sampleData;}

	bool SetStreamFrameData(void* data,uint size,uint freq,sound::ESampleFormat fmt,int channels)
	{
		if(sampleData)delete sampleData;
		sampleData=new byte[size];
		dataSize=size;
		samplesPerSec=freq;
		sampleFormat=fmt;
		channelsCount=channels;
		memcpy(sampleData,data,size);
		return true;
	}
	virtual ISoundStreamFrame* duplicate(){
		WAVSoundStream* data=new WAVSoundStream(getResourceName());
		data->sampleData=new byte[dataSize];
		mraySystem::memCopy(data->sampleData,sampleData,dataSize*sizeof(byte));
		data->dataSize=dataSize;

		data->sampleFormat=sampleFormat;
		data->samplesPerSec=samplesPerSec;
		data->channelsCount=channelsCount;
		return data;
	}
};

sound::ISoundStreamFrame* IWaveLoader::load(const mchar*filename){
	if(!canLoad(filename))return 0;
	GCPtr<OS::IStream> file=gFileSystem.createBinaryFileReader(filename);
	if(!file->isOpen())return 0;
	SWAVHeader header;
	WAVSoundStream* data=new WAVSoundStream(filename);

	file->read(&header,sizeof(header));
	if( header.RIFF[0]!='R' ||
		header.RIFF[1]!='I' ||
		header.RIFF[2]!='F' ||
		header.RIFF[3]!='F')
	{
		gLogManager.getInstance().log(mT("can read only RIFF files"),filename,ELL_WARNING);
		return 0;
	}
	
	if(header.FormatTag!=1)
	{
		gLogManager.getInstance().log(mT("can read only PCM wave files"),filename,ELL_WARNING);
		return 0;
	}

	if(header.DataSize>file->length()-sizeof(header))
	{
		gLogManager.getInstance().log(mT("invalid wav file"),filename,ELL_WARNING);
		return 0;
	}
	data->dataSize=header.DataSize;
	data->sampleData=new byte[header.DataSize];
	file->read(data->sampleData,header.DataSize);

	data->channelsCount=header.Channels;
	data->sampleFormat=header.nBitsperSample==8 ? sound::ESF_8 : sound::ESF_16;
	data->samplesPerSec=header.SampleRate;


	gLogManager.log(mT("wav file loaded"),filename,ELL_SUCCESS);

	return data;
	
}


bool IWaveLoader::canLoad(const mchar*filename)
{
	core::string str(filename);
	str.make_lower();
	return str==mT(".wav");
}

}
}