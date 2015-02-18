#include "stdafx.h"


#include "IOGGLoader.h"
#include <mString.h>
#include <mArray.h>
#include <IFileSystem.h>
#include <LogManager.h>
#include <ISoundStreamFrame.h>
#include <vector>

#include <Vorbis\codec.h>
#include <Vorbis\vorbisfile.h>
#include "mraySystem.h"

namespace mray{
namespace loaders{

class OGGSoundStream:public sound::ISoundStreamFrame
{
public:
    OggVorbis_File         FVorbisFile;
	std::vector<char>	   FDecodingBuffer;
    int                    FOGGCurrentSection;
	uint				   FDecodingBufferUsed;
	uint                   FWaveDataSize;
	byte*                  FOGGRawData;
	uint                   FOGGRawPosition;

	uchar channelsCount;
	uint samplesPerSec;
	sound::ESampleFormat sampleFormat;

	OGGSoundStream(const core::string&filename)
		:channelsCount(0),samplesPerSec(0),sampleFormat(sound::ESF_8),FOGGRawData(0)
   {

	   setResourceName(filename);
	}
	virtual ~OGGSoundStream()
	{
		ov_clear( &FVorbisFile );
		delete [] FOGGRawData;
	}

	virtual uint GetSamplesCount()
	{
		return FDecodingBufferUsed;
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

	virtual void*GetSamples(){return (&FDecodingBuffer[0]);}

	virtual bool isStreaming()
	{
		return true;
	}
	virtual void seek(float time)
	{
		ov_time_seek( &FVorbisFile, time );
	}
	bool SetStreamFrameData(void* data,uint size,uint freq,sound::ESampleFormat fmt,int channels)
	{
		return false;
	}

	virtual int streamData(int size){
		if(size>FDecodingBuffer.size())
			FDecodingBuffer.resize(size);

		int bps=0;
		if(sampleFormat==sound::ESF_16)
			bps=2;
		else 
			bps=1;

		long bytesRead=0;
		while(bytesRead<size){
			long ret= ov_read( &FVorbisFile, 
                             &FDecodingBuffer[0] + bytesRead, 
                             size - bytesRead, 
                             0, 
							  bps,
                             1,
                             &FOGGCurrentSection );
         if ( ret == 0) break;

         if ( ret < 0)
         {
			 gLogManager.log(mT("OGGSoundStream::stream()"),ELL_ERROR);
			 return 0;
            // error in bitstream
         }
         else
         {
            bytesRead += ret;
         }
      }

      FDecodingBufferUsed = bytesRead;

      return FDecodingBufferUsed;
	}
	
	virtual ISoundStreamFrame* duplicate(){
		OGGSoundStream* data=new OGGSoundStream(getResourceName());
		data->sampleFormat=sampleFormat;
		data->samplesPerSec=samplesPerSec;
		data->channelsCount=channelsCount;
		data->FDecodingBuffer.resize(FDecodingBuffer.size());
		mraySystem::memCopy(&data->FDecodingBuffer[0],&FDecodingBuffer[0],FDecodingBuffer.size()*sizeof(char));

		data->FDecodingBufferUsed=FDecodingBufferUsed;
		data->FOGGCurrentSection=FOGGCurrentSection;
		data->FOGGRawData=new byte[FWaveDataSize];
		mraySystem::memCopy(data->FOGGRawData,FOGGRawData,FWaveDataSize*sizeof(byte));

		data->FWaveDataSize=FWaveDataSize;
		return data;
	}
};

size_t OGG_ReadFunc(void* Ptr, size_t Size, size_t NMemB, void* DataSource)
{
    OGGSoundStream* OGG = static_cast<OGGSoundStream*>( DataSource );

    int BytesRead = OGG->FWaveDataSize - OGG->FOGGRawPosition;
    int BytesSize = Size * NMemB;

    if ( BytesSize < BytesRead ) 
		BytesRead = BytesSize;
	else 
		BytesSize = BytesRead;

	if(BytesSize>0){
		mraySystem::memCopy( Ptr, OGG->FOGGRawData + OGG->FOGGRawPosition, BytesRead );

		OGG->FOGGRawPosition += BytesRead;
	}

    return BytesRead;
}
int OGG_SeekFunc(void* DataSource, ogg_int64_t Offset, int Whence)
{
    OGGSoundStream* OGG = static_cast<OGGSoundStream*>( DataSource );

    if ( Whence == SEEK_SET )
    {
        OGG->FOGGRawPosition = static_cast<int>( Offset );
    } else
    if ( Whence == SEEK_CUR )
    {
        OGG->FOGGRawPosition += static_cast<int>( Offset );
    } else
    if ( Whence == SEEK_END )
    {
        OGG->FOGGRawPosition = OGG->FWaveDataSize + 1;//static_cast<int>( Offset );
    }

    if ( OGG->FOGGRawPosition > OGG->FWaveDataSize ) OGG->FOGGRawPosition = OGG->FWaveDataSize;
    if ( OGG->FOGGRawPosition < 0                  ) OGG->FOGGRawPosition = 0;

    return OGG->FOGGRawPosition;
}
int OGG_CloseFunc(void* DataSource)
{
    return 1;
}   
long OGG_TellFunc(void* DataSource)
{
    return static_cast<OGGSoundStream*>( DataSource )->FOGGRawPosition;
}

sound::ISoundStreamFrame* IOGGLoader::load(const mchar*filename){
	if(!canLoad(filename))return 0;
	GCPtr<OS::IStream> file=gFileSystem.createBinaryFileReader(filename);
	if(!file->isOpen())return 0;

	OGGSoundStream* oggdata=new OGGSoundStream(filename);
	oggdata->FWaveDataSize=file->length();
	oggdata->FOGGRawData=new byte[oggdata->FWaveDataSize];
	oggdata->FOGGRawPosition=0;

	file->read(oggdata->FOGGRawData,oggdata->FWaveDataSize);

	ov_callbacks callBacks;
	callBacks.read_func  = OGG_ReadFunc;
    callBacks.seek_func  = OGG_SeekFunc;
    callBacks.close_func = OGG_CloseFunc;
    callBacks.tell_func  = OGG_TellFunc;

    vorbis_comment*   VorbisComment;
    vorbis_info*      VorbisInfo;
	
	if ( ov_open_callbacks( oggdata, &oggdata->FVorbisFile, NULL, -1, callBacks) < 0 )
    {
		gLogManager.getInstance().log( mT("Invalid OGG file"), filename,ELL_WARNING);
		oggdata=0;
		return 0;
    }

    VorbisComment = ov_comment( &oggdata->FVorbisFile, -1);
    VorbisInfo    = ov_info ( &oggdata->FVorbisFile, -1 );

    oggdata->channelsCount= VorbisInfo->channels;
    oggdata->samplesPerSec = VorbisInfo->rate;
	oggdata->sampleFormat= sound::ESF_16; 
   
	
	gLogManager.log(mT("ogg file loaded"),filename,ELL_SUCCESS);

	return oggdata;
}



bool IOGGLoader::canLoad(const mchar*filename)
{
	core::string str(filename);
	str.make_lower();
	return str==mT(".ogg");
}



}
}
