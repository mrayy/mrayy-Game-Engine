

/********************************************************************
	created:	2011/01/15
	created:	15:1:2011   23:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mraySound\GSMVoiceEncoder.h
	file path:	i:\Programing\GameEngine\mrayEngine\mraySound
	file base:	GSMVoiceEncoder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GSMVoiceEncoder_h__
#define GSMVoiceEncoder_h__

#include "compileConfig.h"
#include "IVoiceEncoder.h"


namespace mray
{
namespace sound
{

class MRAY_SOUND_DLL GSMVoiceEncoder:public IVoiceEncoder
{
private:
protected:

	void * m_encoderState;
	void * m_decoderState;

	virtual uint GetSamplesPerFrame() ;

	virtual uint GetMaxCompressedFrameSize() ;
	virtual uint CompressFrame(short *samplePtr, uchar *outputPtr);

	virtual uint GetAvgCompressedFrameSize() ;
	virtual uint DecompressFrame(short *framePtr, uchar *inputPtr, uint inSize) ;
public:
	GSMVoiceEncoder();
	virtual ~GSMVoiceEncoder();
	
};

}
}

#endif // GSMVoiceEncoder_h__
