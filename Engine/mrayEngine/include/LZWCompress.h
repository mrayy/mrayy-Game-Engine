
/********************************************************************
	created:	2009/05/31
	created:	31:5:2009   19:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LZWCompress.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LZWCompress
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LZWCompress___
#define ___LZWCompress___

#include "ICompressor.h"
#include "TreeDictionary.h"

namespace mray{
namespace Compress{

class MRAY_DLL LZWCompress:public ICompressor
{
private:
protected:
	core::TreeDictionary<uchar,int> m_codeTable;
	std::vector<uchar> m_buffer;
	std::vector<uchar> m_tmp;
	std::vector<uchar> m_flushBuffer;
	typedef std::vector<uchar> ByteArray;
	std::vector<ByteArray> m_decompressTable;

	int m_cachSize;
	int m_maxLen;
	int m_lastCode;
	int m_usedBits;
	uchar m_lastByte;
	uchar m_remaingBits;

	void cachBuffer(OS::IStream* input);
	void resetCompress();
	void resetDecompress();
	void flush(OS::IStream* output);
	void writeToBuffer(int v,int bits);
public:
	LZWCompress();
	virtual~LZWCompress();

	void setCachSize(int s);

	virtual bool isLossless();

	virtual int compress(OS::IStream* input,OS::IStream* output);
	virtual int decompress(OS::IStream* input,OS::IStream* output);
};

}
}


#endif //___LZWCompress___
