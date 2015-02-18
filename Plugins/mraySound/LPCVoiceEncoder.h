

/********************************************************************
	created:	2011/01/15
	created:	15:1:2011   23:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mraySound\LPCVoiceEncoder.h
	file path:	i:\Programing\GameEngine\mrayEngine\mraySound
	file base:	LPCVoiceEncoder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef LPCVoiceEncoder_h__
#define LPCVoiceEncoder_h__

#include "compileConfig.h"
#include "IVoiceEncoder.h"

namespace mray
{
namespace sound
{

class MRAY_SOUND_DLL LPCVoiceEncoder:public IVoiceEncoder
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
	LPCVoiceEncoder();
	virtual ~LPCVoiceEncoder();
	
};

}
}
#endif // LPCVoiceEncoder_h__

