#include "stdafx.h"

#include "LZWCompress.h"
#include "IStream.h"
#include <math.h>


namespace mray{
namespace Compress{

LZWCompress::LZWCompress():ICompressor(mT("LZW"))
{
	setCachSize(4096);
	m_maxLen=20;
	m_tmp.reserve(m_maxLen);
	m_lastCode=0;
	m_lastByte=0;
	m_remaingBits=0;
}
LZWCompress::~LZWCompress(){
}

void LZWCompress::setCachSize(int s){
	m_cachSize=s;
	m_buffer.reserve(s); // allocate a block for reading from the file
}

void LZWCompress::flush(OS::IStream* output){
	output->write(&m_flushBuffer[0],m_flushBuffer.size());
	m_flushBuffer.clear();
}
void LZWCompress::writeToBuffer(int v,int bits){
	int bytes=0;
	if(bits % 8 ==0){
		bytes=bits/8;
	}else{
		bytes=(bits/8) +1;
	}
	int tmp=m_lastByte;
	for(int i=bits-1,x=1;i>=0;--i,++x){
		tmp |= (v >> (i)) & 1;
		if((x+ m_remaingBits) % 8 == 0){
			m_flushBuffer.push_back(tmp);
			tmp=0;
		}else{
			tmp <<=1;
		}
	}
	m_remaingBits=(bits+m_remaingBits)%8;
	m_lastByte=tmp;
	m_usedBits+=bits;
}


void LZWCompress::resetCompress(){
	m_codeTable.clear();
	m_lastCode=0;
	for (int i=0;i<256;i++)
	{
		m_codeTable.Add((uchar)i,m_lastCode++);
	}
	m_buffer.resize(0);
}
void LZWCompress::resetDecompress(){
	m_decompressTable.resize(256);
	m_lastCode=0;
	for (int i=0;i<256;i++)
	{
		m_decompressTable[i].resize(1);
		m_decompressTable[i][0]=(uchar)i;
	}
	m_lastCode=256;
	m_buffer.resize(0);
}


void LZWCompress::cachBuffer(OS::IStream* input){
	int r=input->read(&m_buffer[0],m_cachSize);
	m_buffer.resize(r);
}

bool LZWCompress::isLossless(){
	return true;
}

int LZWCompress::compress(OS::IStream* input,OS::IStream* output){
	
	resetCompress();
	input->seek(0,OS::ESeek_Set);
	m_tmp.resize(0);
	int lastCode=0;
	int *entry=0;

	static const float invLog2=1.0f/log(2.0f);

	int totalBits=0;

	int lastBitC=0;
	while(!input->eof())
	{
		cachBuffer(input);
		for (int i=0;i<m_buffer.size();i++)
		{
			uchar k=m_buffer[i];
			m_tmp.push_back(k);
			entry=m_codeTable.getValue(&m_tmp[0],m_tmp.size());
			if(!entry){
				int bitsCount=ceil(log((float)m_lastCode)*invLog2);
				if(bitsCount!=lastBitC){
					lastBitC=bitsCount;
				}
				m_codeTable.Add(&m_tmp[0],m_tmp.size(),m_lastCode++);
				m_tmp.resize(0);
				writeToBuffer(lastCode,bitsCount);
				totalBits+=bitsCount;
				
				if(m_flushBuffer.size()>m_cachSize){
					flush(output);
				}
				m_tmp.push_back(k);
				lastCode=*m_codeTable.getValue(&k,1);
			}else{
				lastCode=*entry;
			}
		}
	}
	int bitsCount=ceil(log((float)m_lastCode)*invLog2);
	writeToBuffer(lastCode,bitsCount);
	totalBits+=bitsCount;
	if(m_remaingBits>0){
		writeToBuffer(m_lastByte,m_remaingBits);
	}
	flush(output);

	return ceil(totalBits/8.0f);
}
int LZWCompress::decompress(OS::IStream* input,OS::IStream* output){
	resetDecompress();
	input->seek(0,OS::ESeek_Set);
	ByteArray lastCode;
	ByteArray tmp;
	ByteArray* entry=0;
	static const float invLog2=1.0f/log(2.0f);

	int lastBit=0;
	int code=0;
	m_tmp.clear();

	int totalBytes=0;

	int lastBitC=0;

	while(!input->eof())
	{
		cachBuffer(input);
		for (int k=0;k<m_buffer.size()*8;)
		{
			int bitsCount=ceil(log((float)m_lastCode)*invLog2);
			if(bitsCount!=lastBitC){
				lastBitC=bitsCount;
			}
			code=0;
			int startShift=k%8;
			for (int j=0;j<bitsCount;++j,++k)
			{
				if(k>=m_buffer.size()*8){
					cachBuffer(input);
					if(m_buffer.size()==0){
						//end of stream
						if(m_tmp.size()>0){
							output->write(&m_tmp[0],m_tmp.size());
							m_tmp.resize(0);
						}
						return totalBytes;
					}
					k=0;
				}
				int currByte=k/8;
				int shift=j+startShift;
				if(shift>=8)
					shift=8-shift%8 -1;
				else shift=8-shift-1;

				code<<=1;
				code |= (m_buffer[currByte]  >> shift) & 1;
			}
			m_lastCode++;
			entry=0;
			if(code>=m_decompressTable.size()){
				tmp.resize(lastCode.size());
				for (int j=0;j<lastCode.size();j++)
				{
					tmp[j]=lastCode[j];
				}
				tmp.push_back(lastCode[0]);
				entry=&tmp;

				//lastCode.push_back();
			}else{
				entry=&m_decompressTable[code];
			}

			totalBytes+=entry->size();
			for(int i=0;i<entry->size();i++){
				m_tmp.push_back((*entry)[i]);
			}
			if(m_tmp.size()>m_cachSize){
				output->write(&m_tmp[0],m_tmp.size());
				m_tmp.resize(0);
			}

			if(lastCode.size()!=0 )
			{
				lastCode.push_back((*entry)[0]);
				m_decompressTable.push_back(lastCode);
				if(m_decompressTable.size()>code)
					entry=&m_decompressTable[code];
			}

			if(entry){
				lastCode.resize(entry->size());
				for(int i=0;i<entry->size();i++)
					lastCode[i]=(*entry)[i];
			}
		}
	}

	if(m_tmp.size()>0){
		output->write(&m_tmp[0],m_tmp.size());
		m_tmp.resize(0);
	}
	return totalBytes;
}

}
}
