#include "stdafx.h"

#include "IMP3Loader.h"
#include <IFileSystem.h>
#include <LogManager.h>
#include <mArray.h>
#include <ISoundStreamFrame.h>
#include <decoder\mpaudec.h>
#include "mraySystem.h"


namespace mray{
namespace loaders{

class QueueBuffer{
	int Capacity;
	int Size;
	byte *Buffer;
public:
	QueueBuffer()
	{
		Capacity = 256;
		Size = 0;

		Buffer = (byte*)malloc(Capacity);
	}



	~QueueBuffer() 
	{
		clear();
	}
	void clear(){
		free(Buffer);
		Buffer=0;
		Capacity=Size=0;
	}

	void setSize(int sz){
		bool b=0;
		while (sz > Capacity) 
		{
			Capacity = Capacity*2+1;
			b=1;
		}
		if(b)
			Buffer = (byte*)realloc(Buffer, Capacity);
		Size=sz;
	}
	int getSize()
	{
		return Size;
	}

	void write(const void* buffer, int size) 
	{
		bool needRealloc = false;
		
		while (size + Size > Capacity) 
		{
			Capacity = Capacity*2+1;
			needRealloc = true;
		}

		if (needRealloc) 
		{
			Buffer = (byte*)realloc(Buffer, Capacity);
		}

		mraySystem::memCopy(Buffer + Size, buffer, size);
		Size += size;
	}


	int read(void* buffer, int size) 
	{
		int toRead = math::Min<int>(size, Size);

		mraySystem::memCopy(buffer, Buffer, toRead);
		mraySystem::memMove(Buffer, Buffer + toRead, Size - toRead);

		Size -= toRead;
		return toRead;
	}
};

class MP3SoundStream:public sound::ISoundStreamFrame
{
#define MP3_INPUT_BUFFER_SIZE 4096
public:

	GCPtr<OS::IStream>file;
	bool eofReached;
	int outputSize;

	bool firstFrameRead;
	int inputPosition;
	int inputLength;

	int framesCount;

	uchar channelsCount;
	uint samplesPerSec;
	sound::ESampleFormat sampleFormat;

	QueueBuffer buffer;
	std::vector<byte> outBuffer;
	byte*         decodeBuffer;

	MPAuDecContext*context;
	byte inputBuffer[MP3_INPUT_BUFFER_SIZE];

	MP3SoundStream():
	channelsCount(0),samplesPerSec(0),sampleFormat(sound::ESF_8)
    {
		eofReached=false;
		outputSize=0;
		firstFrameRead=0;
		inputPosition=0;
		inputLength=0;
		framesCount=0;
		decodeBuffer=0;
	}
	virtual ~MP3SoundStream(){
		buffer.clear();
		outBuffer.clear();
		delete [] decodeBuffer;
		mpaudec_clear(context);
		delete context;
	}


	void* GetSamples()
	{
		if(outBuffer.size())
			return &outBuffer[0];
		return 0;
	}
	uint GetSamplesCount()
	{
		return outBuffer.size();
	}
	virtual uint GetChannels()
	{
		return channelsCount;
	}
	virtual uint  GetFrequency()
	{
		return samplesPerSec;
	}
	virtual sound::ESampleFormat GetFormat()
	{
		return sampleFormat;
	}
	bool SetStreamFrameData(void* data,uint size,uint freq,sound::ESampleFormat fmt,int channels)
	{
		return false;
	}

	virtual bool isStreaming(){return 1;}
	virtual void seek(float time){
		if ( time == 0 && file && context )
		{
			file->seek(0,OS::ESeek_Set);
			eofReached = false;

			buffer.setSize(0);

			MPAuDecContext oldContext = *context;

			mpaudec_clear(context);
			mpaudec_init(context);

			context->bit_rate = oldContext.bit_rate;
			context->channels = oldContext.channels;
			context->frame_size = oldContext.frame_size;
			context->sample_rate = oldContext.sample_rate;

			inputPosition = 0;
			inputLength = 0;

		}

	}
	virtual int streamData(int size){

		const int frameSize = getFrameSize();

		outBuffer.resize(size);
		size/=frameSize;
		int framesRead = 0;
		byte* out=&outBuffer[0];

		while (framesRead < size) 
		{
			// no more samples?  ask the MP3 for more
			if (buffer.getSize() < frameSize)
			{
				if (!decodeFrame() || eofReached)
					return framesRead;

				// if the buffer is still empty, we are done
				if (buffer.getSize() < frameSize) 
					return framesRead;
			}

			const int framesLeft = size - framesRead;
			const int framesToRead = math::Min<int>( framesLeft, buffer.getSize() / frameSize);

			int toRead=math::Min<int>(framesToRead * frameSize,buffer.getSize());
			buffer.read(out,toRead);

			out += toRead;
			framesRead += framesToRead;
		}

		return framesRead;
	}
	


	bool decodeFrame(){
		int outputSize = 0;
		
		while (!outputSize) 
		{
			if (inputPosition == inputLength) 
			{
				inputPosition = 0;
				inputLength = file->read(inputBuffer, MP3_INPUT_BUFFER_SIZE);

				if (inputLength == 0) 
				{
					eofReached = true;
					return true;
				}
			}

			int rv = mpaudec_decode_frame( context, (byte*)decodeBuffer,
										   &outputSize,
										   (byte*)inputBuffer + inputPosition,
										   inputLength - inputPosition);
		    
			if (rv < 0)
				return false;

			inputPosition += rv;
		} // end while

		if (!firstFrameRead) 
		{
			this->channelsCount = context->channels;
			this->samplesPerSec = context->sample_rate;
			this->sampleFormat = sound::ESF_16;
			framesCount= -1; // unknown lenght

			firstFrameRead = true;
		}
		else
		if( context->channels != this->channelsCount ||
			context->sample_rate != this->samplesPerSec) 
		{
			// Can't handle format changes mid-stream.
			return false;
		}

		if (!context->parse_only) 
		{
			if (outputSize < 0) 
			{
				// Couldn't decode this frame.  Too bad, already lost it.
				// This should only happen when seeking.

				outputSize = context->frame_size;
				memset(decodeBuffer, 0, outputSize * getFrameSize());
			}

			buffer.write(decodeBuffer,outputSize);
		}

		return true;
	}
	virtual ISoundStreamFrame* duplicate(){
		MP3SoundStream* mp3data=new MP3SoundStream();
		mp3data->file=file;
		mp3data->context=new MPAuDecContext();
		if(!mp3data->context || mpaudec_init(mp3data->context)<0){
			gLogManager.log(mT("IMP3Loader::duplicate(), Invalid data:")+getResourceName(),ELL_WARNING);
			mp3data=0;
			return 0;
		}
		mp3data->sampleFormat=sampleFormat;
		mp3data->samplesPerSec=samplesPerSec;
		mp3data->channelsCount=channelsCount;

		mp3data->decodeBuffer =new byte[MPAUDEC_MAX_AUDIO_FRAME_SIZE];

		mp3data->framesCount=0;
		if(mp3data->file->length()>0){
			mp3data->context->parse_only=1;

			while(!mp3data->eofReached){
				if(!mp3data->decodeFrame())
					break;
				mp3data->framesCount+=mp3data->context->frame_size;
			}
			mp3data->context->parse_only=0;
			mp3data->seek(0);
		}else
			mp3data->decodeFrame();

		if(!mp3data->context->channels || !mp3data->context->sample_rate){
			delete mp3data;
			gLogManager.log(mT("IMP3Loader::duplicate(), Couldn't duplicate data")+getResourceName(),ELL_WARNING);
			return 0;
		}

		return mp3data;
	}
};

sound::ISoundStreamFrame* IMP3Loader::load(OS::IStream* file){
	MP3SoundStream*mp3data=new MP3SoundStream();
	mp3data->file=file;
	mp3data->context=new MPAuDecContext();

	if(!mp3data->context || mpaudec_init(mp3data->context)<0){
		gLogManager.log(mT("IMP3Loader::load(), Invalid file"),file->getStreamName(),ELL_WARNING);
		delete mp3data;
		return 0;
	}

	mp3data->decodeBuffer =new byte[MPAUDEC_MAX_AUDIO_FRAME_SIZE];

	mp3data->framesCount=0;

	if(mp3data->file->length()>0){
		mp3data->context->parse_only=1;

		while(!mp3data->eofReached){
			if(!mp3data->decodeFrame())
				break;
			mp3data->framesCount+=mp3data->context->frame_size;
		}
		mp3data->context->parse_only=0;
		mp3data->seek(0);
	}else
		mp3data->decodeFrame();
	
	if(!mp3data->context->channels || !mp3data->context->sample_rate){
		delete mp3data;
		gLogManager.log(mT("IMP3Loader::load(), Couldn't load file"),file->getStreamName(),ELL_WARNING);
		return 0;
	}


	gLogManager.log(mT("mp3 file loaded"),file->getStreamName(),ELL_SUCCESS);

	return mp3data;
}
sound::ISoundStreamFrame*  IMP3Loader::load(const  mchar*filename){

	if(!canLoad(filename))return 0;
	GCPtr<OS::IStream> file=gFileSystem.createBinaryFileReader(filename);
	if(!file->isOpen()){
		gLogManager.log(mT("IMP3Loader::load(), file Not Found"),filename,ELL_WARNING);
		return 0;
	}

	return load(file);
}



bool IMP3Loader::canLoad(const  mchar*filename){

	core::string str(filename);
	str.make_lower();
	return compareStr(str.c_str(),mT(".mp3"))!=0;
}



}
}


