#include "stdafx.h"
#include "FSLSoundBuffer.h"



namespace mray{
namespace sound{

	int buffer_id=0;

FSLSoundBuffer::FSLSoundBuffer()
{
	m_buffer=0;
	buffer_id++;
}
FSLSoundBuffer::~FSLSoundBuffer()
{
	if(m_buffer)
		fslDestroyBuffer(m_buffer);

	buffer_id--;
	printf("destroyed %d\n",buffer_id);
}

bool FSLSoundBuffer::SetStreamFrameData(void* data,uint size,uint freq,ESampleFormat fmt,int channels)
{
	m_samplesCount=size;
	if(m_buffer)
		fslDestroyBuffer(m_buffer);
	m_buffer=fslCreateBuffer();
	FSLenum format;
	if(channels==1)
	{
		if(fmt==ESF_8)format= FSL_FORMAT_MONO8;
		else
		{
			format= FSL_FORMAT_MONO16;
			size*=2;
		}
	}else
	{
		if(fmt==ESF_8)format=  FSL_FORMAT_STEREO8;
		else
		{
			format=  FSL_FORMAT_STEREO16;
			size*=2;
		}
	}
	fslSetBufferData(m_buffer,format,data,size,freq);


	return true;
}

void FSLSoundBuffer::setBuffer(FSLbuffer buff){
	m_buffer=buff;
}
FSLbuffer FSLSoundBuffer::getBuffer( ){
	return m_buffer;
}

uint FSLSoundBuffer::GetSamplesCount()
{
	return m_samplesCount;
}
uint FSLSoundBuffer::GetFrequency()
{
	if(!m_buffer)return 0;
	return fslBufferGetInfo(m_buffer).uiFrequency;
}
uint FSLSoundBuffer::GetChannels()
{
	if(!m_buffer)return 0;
	return fslBufferGetInfo(m_buffer).uiChannels;
}
ESampleFormat FSLSoundBuffer::GetFormat()
{
	if(!m_buffer)return ESF_8;
	return fslBufferGetInfo(m_buffer).uiSampling==8? ESF_8 : ESF_16;
}

ISoundStreamFrame* FSLSoundBuffer::duplicate(){
	FSLSoundBuffer* b=new FSLSoundBuffer();
	b->m_buffer=m_buffer;
	return b;
}


}
}
