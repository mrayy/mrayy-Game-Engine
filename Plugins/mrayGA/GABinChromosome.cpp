
#include "stdafx.h"
#include "GABinChromosome.h"
#include <memory>


namespace mray
{
namespace AI
{

GABinChromosome::GABinChromosome():m_bits(0),m_length(0)
{
}
GABinChromosome::GABinChromosome(const bool* bits,uint len):m_length(len),m_bits(new bool[len])
{
	memcpy(m_bits,bits,len);
}

GABinChromosome::~GABinChromosome()
{
	delete [] m_bits;
}

uint GABinChromosome::GetCodeLength()
{
	return m_length;
}

void GABinChromosome::CreateBuffer(GACodeBuffer*buff)
{
	buff->Resize(m_length);
}
void GABinChromosome::FillBuffer(GACodeBuffer* buff)
{
	FillBuffer(buff,0,m_length);
}
void GABinChromosome::FillBuffer(GACodeBuffer* buff,int start,int len)
{
	bool*b=((bool*)buff->GetBuffer());
	memcpy(b+start,m_bits+start,len*sizeof(bool));
}
void GABinChromosome::FromBuffer(GACodeBuffer* buff)
{
	bool*b=(bool*)buff->GetBuffer();
	memcpy(m_bits,b,m_length);
}

void GABinChromosome::Flip(int start,int len)
{
	for(int i=start;i<start+len;++i)
	{
		m_bits[i]=!m_bits[i];
	}
}

void GABinChromosome::SetBit(uint b,bool v)
{
	m_bits[b]=v;
}
bool GABinChromosome::GetBit(uint b)
{
	return m_bits[b];
}


GAChromosome* GABinChromosome::CreateCopy()
{
	return new GABinChromosome(m_bits,m_length);
}



}
}
