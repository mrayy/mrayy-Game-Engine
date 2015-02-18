#include "stdafx.h"


#include "CRC.h"
#include "IStream.h"

namespace mray{

	
namespace OS{

bool CRC::m_inited=false;

uint CRC::m_crcTable[256];


void CRC::initCRCTable()
{
	m_inited=true;
	uint val=0;
	for(int i=0;i<256;++i)
	{
		val=i;
		for(int j=0;j<8;j++)
		{
			if(val & 0x01)
			{
				val=0xedb88320 ^ (val >> 1);
			}else
				val=val>>1;
		}
		m_crcTable[i]=val;
	}
}

uint CRC::calcCRC(const char*buff,int len,uint crcVal)
{
	if(!m_inited)
		initCRCTable();
	for(;*buff;++buff){
		crcVal=m_crcTable[(crcVal^*buff)&0xff]^(crcVal>>8);
	}
	return crcVal;
}
uint CRC::calcCRC(IStream* stream,uint crcVal){

	stream->seek(0,ESeek_Set);

	int size=stream->length();
	char buff[4096];
	int count=(size+4095)/4096;

	for(int i=0;i<count;++i){
		int len=stream->read(buff,4096);
		crcVal=calcCRC(buff,len,crcVal);
	}
	stream->seek(0,ESeek_Set);
	return crcVal;
}

}
}