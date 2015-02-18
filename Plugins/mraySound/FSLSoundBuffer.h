
/********************************************************************
	created:	2009/08/05
	created:	5:8:2009   7:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mraySound\FSLSoundBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mraySound
	file base:	FSLSoundBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FSLSoundBuffer___
#define ___FSLSoundBuffer___

#include <ISoundStreamFrame.h>
#include <FreeSL.h>

#include "compileConfig.h"

namespace mray{
namespace sound{

class MRAY_SOUND_DLL FSLSoundBuffer:public ISoundStreamFrame
{
private:
protected:
	FSLbuffer m_buffer;
	uint m_samplesCount;
public:

	core::string fileName;

	FSLSoundBuffer();
	virtual~FSLSoundBuffer();

	void setBuffer(FSLbuffer buff);
	FSLbuffer getBuffer( );

	virtual bool SetStreamFrameData(void* data,uint size,uint freq,ESampleFormat fmt,int channels);
	virtual void* GetSamples(){return 0;}

	virtual uint GetSamplesCount();
	virtual uint GetFrequency();
	virtual uint GetChannels();
	virtual ESampleFormat GetFormat();

	virtual ISoundStreamFrame* duplicate();

};

}
}


#endif //___FSLSoundBuffer___
