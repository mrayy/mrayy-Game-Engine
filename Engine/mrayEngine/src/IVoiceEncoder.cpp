#include "stdafx.h"

#include "IVoiceEncoder.h"

#include "mraySystem.h"

namespace mray
{
namespace sound
{

IVoiceEncoder::IVoiceEncoder()
{
}
IVoiceEncoder::~IVoiceEncoder()
{
}

std::vector<uchar>& IVoiceEncoder::EncodeBuffer(std::vector<uchar>& sampleBuffer)
{
	m_workingBuffer.resize(0);
	uint sampleCount = sampleBuffer.size() >> 1;
	uint spf = GetSamplesPerFrame();

	uint framesUsed = (uint)(sampleCount / spf);
	if(framesUsed)
	{
		uint samplesUsed = framesUsed * spf;
		uint maxSize = GetMaxCompressedFrameSize() * framesUsed;
		m_workingBuffer.resize(maxSize);

		uchar *compressedPtr = &m_workingBuffer[0];
		short *samplePtr = (short*) &sampleBuffer[0];

		uint len = 0;

		for(uint i = 0; i < samplesUsed; i += spf)
			len += CompressFrame(samplePtr + i, compressedPtr + len);

		m_workingBuffer.resize(len);

		uint newSize = (sampleCount - samplesUsed) * sizeof(ushort);
		mraySystem::memCopy(samplePtr, samplePtr + samplesUsed, newSize);

		sampleBuffer.resize(newSize);
	}
	return m_workingBuffer;
}


std::vector<uchar>& IVoiceEncoder::DecodeBuffer(std::vector<uchar>& compressedBuffer)
{

	uint spf = GetSamplesPerFrame();
	uint avgCompressedSize = GetAvgCompressedFrameSize();
	uint compressedSize = compressedBuffer.size();

	// guess the total number of frames:
	uint guessFrameCount = (compressedSize / avgCompressedSize) + 1;

	m_workingBuffer.resize(spf * sizeof(short) * guessFrameCount);

	uint p = 0;
	uchar *inputPtr = (uchar *) &compressedBuffer[0];
	uint frameCount = 0;
	short *samplePtr = (short *) &m_workingBuffer[0];

	for(uint i = 0; i < compressedSize; i += p)
	{
		if(frameCount == guessFrameCount)
		{
			guessFrameCount = frameCount + ( (compressedSize - i) / avgCompressedSize ) + 1;
			m_workingBuffer.resize(spf * sizeof(short) * guessFrameCount);

			samplePtr = (short *) &m_workingBuffer[0];
		}
		p = DecompressFrame(samplePtr + frameCount * spf, inputPtr + i, compressedSize - i);
		frameCount++;
	}
	m_workingBuffer.resize(frameCount * spf * sizeof(short));
	return m_workingBuffer;
}


}
}