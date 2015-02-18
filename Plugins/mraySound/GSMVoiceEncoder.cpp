#include "stdafx.h"
#include "GSMVoiceEncoder.h"

#include "gsm.h"

namespace mray
{
namespace sound
{




GSMVoiceEncoder::GSMVoiceEncoder()
{
	m_encoderState = gsm_create();
	m_decoderState = gsm_create();

}
GSMVoiceEncoder::~GSMVoiceEncoder()
{
	gsm_destroy((struct gsm_state *) m_encoderState);
	gsm_destroy((struct gsm_state *) m_decoderState);
}


uint GSMVoiceEncoder::GetSamplesPerFrame() 
{
	return GSM_SAMPLES_PER_FRAME;
}

uint GSMVoiceEncoder::GetMaxCompressedFrameSize() 
{
	return GSM_ENCODED_FRAME_SIZE;
}
uint GSMVoiceEncoder::CompressFrame(short *samplePtr, uchar *outputPtr)
{
	return gsm_encode((struct gsm_state *) m_encoderState, samplePtr, outputPtr);
}

uint GSMVoiceEncoder::GetAvgCompressedFrameSize()
{
	return GSM_ENCODED_FRAME_SIZE;
}
uint GSMVoiceEncoder::DecompressFrame(short *framePtr, uchar *inputPtr, uint inSize) 
{
	gsm_decode((struct gsm_state *) m_decoderState, inputPtr, framePtr);
	return GSM_ENCODED_FRAME_SIZE;
}

}
}
