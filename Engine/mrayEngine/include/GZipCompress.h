


/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   15:17
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\GZipCompress.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GZipCompress
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GZipCompress__
#define __GZipCompress__

#include "ICompressor.h"

namespace mray
{
namespace Compress
{

class MRAY_DLL GZipCompress:public ICompressor
{
private:
protected:
	static const int m_defaultChunk;
	std::vector<byte> m_bytes;
	std::vector<byte> m_outbytes;
	bool ReadChunk(OS::IStream* input);
public:
	GZipCompress();
	virtual~GZipCompress();

	virtual bool isLossless();

	virtual int compress(OS::IStream* input,OS::IStream* output);
	virtual int decompress(OS::IStream* input,OS::IStream* output);
};

}
}

#endif
