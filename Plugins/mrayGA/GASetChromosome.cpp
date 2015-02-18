
#include "stdafx.h"
#include "GASetChromosome.h"
#include "GAAlgorithmParameters.h"
#include "IGAChromValues.h"
#include <Randomizer.h>
#include <memory>

namespace mray
{
namespace AI
{

GASetChromosome::GASetChromosome(int length,int maxIndex,GAAlgorithmParameters* params)
:GAValuedChrom(params),m_length(length),m_maxIndex(maxIndex),m_indicies(new uint[length])
{
}
GASetChromosome::GASetChromosome(uint *indicies,int length,int maxIndex,GAAlgorithmParameters* params)
:GAValuedChrom(params),m_length(length),m_maxIndex(maxIndex),m_indicies(new uint[length])
{
	for (int i=0;i<length;++i)
	{
		m_indicies[i]=GetNearestValue(i,indicies[i]);
	}
}
GASetChromosome::~GASetChromosome()
{
	delete [] m_indicies;
}

uint GASetChromosome::GetCodeLength()
{
	return m_length;
}

void GASetChromosome::CreateBuffer(GACodeBuffer*buff)
{
	buff->Resize(m_length*sizeof(uint));
}
void GASetChromosome::FillBuffer(GACodeBuffer* buff)
{
	FillBuffer(buff,0,m_length);
}
void GASetChromosome::FillBuffer(GACodeBuffer* buff,int start,int len)
{
	uint* ind=((uint*)buff->GetBuffer());
	memcpy(ind+start,m_indicies+start,len*sizeof(uint));
}
void GASetChromosome::FromBuffer(GACodeBuffer* buff)
{
	uint* ind=((uint*)buff->GetBuffer());
	for(int i=0;i<m_length;++i)
	{
		m_indicies[i]=GetNearestValue(i,ind[i]);
	}
}

void GASetChromosome::Flip(int start,int len)
{
	for(int i=start;i<start+len;++i)
	{
		m_indicies[i]=GetNearestValue(i, math::Randomizer::rand(m_maxIndex));
	}
}

void GASetChromosome::SetAt(uint b,uint v)
{
	m_indicies[b]=GetNearestValue(b, v);;
}
uint GASetChromosome::GetAt(uint b)
{
	return m_indicies[b];
}

GAChromosome* GASetChromosome::CreateCopy()
{
	return new GASetChromosome(m_indicies,m_length,m_maxIndex,m_params);
}

}
}
