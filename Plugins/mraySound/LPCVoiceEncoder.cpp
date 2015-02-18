#include "stdafx.h"

#include "LPCVoiceEncoder.h"

#include "lpc10.h"

namespace mray
{
namespace sound
{




LPCVoiceEncoder::LPCVoiceEncoder()
{
	m_encoderState = create_lpc10_encoder_state();
	init_lpc10_encoder_state((lpc10_encoder_state *) m_encoderState);


	m_decoderState = create_lpc10_decoder_state();
	init_lpc10_decoder_state((lpc10_decoder_state *) m_decoderState);

}
LPCVoiceEncoder::~LPCVoiceEncoder()
{
	destroy_lpc10_encoder_state((lpc10_encoder_state *) m_encoderState);
	destroy_lpc10_decoder_state((lpc10_decoder_state *) m_decoderState);
}


uint LPCVoiceEncoder::GetSamplesPerFrame() 
{
	return LPC10_SAMPLES_PER_FRAME;
}

uint LPCVoiceEncoder::GetMaxCompressedFrameSize() 
{
	return LPC10_ENCODED_FRAME_SIZE;
}
uint LPCVoiceEncoder::CompressFrame(short *samplePtr, uchar *outputPtr)
{
	return vbr_lpc10_encode(samplePtr, outputPtr, (lpc10_encoder_state *) m_encoderState);
}

uint LPCVoiceEncoder::GetAvgCompressedFrameSize()
{
	return (LPC10_ENCODED_FRAME_SIZE + 1) >> 1;
}
uint LPCVoiceEncoder::DecompressFrame(short *framePtr, uchar *inputPtr, uint inSize) 
{
	int p;
	vbr_lpc10_decode(inputPtr, inSize, framePtr, (lpc10_decoder_state *) m_decoderState, &p);
	return (uint) p;
}

}
}
