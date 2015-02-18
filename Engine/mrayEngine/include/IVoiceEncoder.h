



/********************************************************************
	created:	2011/01/15
	created:	15:1:2011   22:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IVoiceEncoder.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IVoiceEncoder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IVoiceEncoder_h__
#define IVoiceEncoder_h__

#include "mtypes.h"

#include "CompileConfig.h"

namespace mray
{
namespace sound
{

class MRAY_DLL IVoiceEncoder
{
private:
protected:
	virtual uint GetSamplesPerFrame() = 0;

	virtual uint GetMaxCompressedFrameSize() = 0;
	virtual uint CompressFrame(short *samplePtr, uchar *outputPtr) = 0;

	virtual uint GetAvgCompressedFrameSize() = 0;
	virtual uint DecompressFrame(short *framePtr, uchar *inputPtr, uint inSize) = 0;

	std::vector<uchar> m_workingBuffer;
public:
	IVoiceEncoder();
	virtual ~IVoiceEncoder();

	std::vector<uchar>& EncodeBuffer(std::vector<uchar>& sampleBuffer);
	std::vector<uchar>& DecodeBuffer(std::vector<uchar>& compressedBuffer);
};

}
}

#endif // IVoiceEncoder_h__
