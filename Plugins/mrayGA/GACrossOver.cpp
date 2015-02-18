

#include "stdafx.h"
#include "GACrossOver.h"
#include "GAChromosome.h"

#include <Randomizer.h>

namespace mray
{
namespace AI
{

GAMultiPointCrossover::GAMultiPointCrossover(int numberOfPoints):m_numberOfPoints(0),m_points(0)
{
	SetNumberOfPoints(numberOfPoints);
	m_buffer=new GACodeBuffer();
}
GAMultiPointCrossover::~GAMultiPointCrossover()
{
	delete [] m_points;
	delete m_buffer;
}

void GAMultiPointCrossover::SetNumberOfPoints(int c)
{
	if(c==m_numberOfPoints)
		return;
	delete [] m_points;
	m_numberOfPoints=c;
	m_points=new int[m_numberOfPoints+1];
}

void GAMultiPointCrossover::Apply(GAChromosome* c1,GAChromosome* c2,GAChromosome*&result)
{
	//GAChromosome* result=c1->CreateCopy();

	int codeLen=c1->GetCodeLength();
	c1->CreateBuffer(m_buffer);
	//int itemSize=m_buffer.GetSize()/codeLen;

	m_points[0]=0;
	for(int i=0;i<m_numberOfPoints;++i)
	{
		bool found;
		int c;
		do{
			found=false;
			c=math::Randomizer::rand(codeLen);
			for(int j=0;j<i;++j)
			{
				if(m_points[j+1]==c){
					found=true;
					break;
				}
			}
		}while(found);
		int j=i+1;
		for(;j>0 && m_points[j-1]>c;--j)
		{
			m_points[j]=m_points[j-1];
		}
		m_points[j]=c;
	}

	bool turn=math::Randomizer::rand(2)==1;

	for(int i=0;i<m_numberOfPoints+1;++i)
	{
		int p=m_points[i];
		int len=0;
		if(i==m_numberOfPoints)
			len=codeLen-p;
		else len=m_points[i+1]-p;
		if(turn)
			c1->FillBuffer(m_buffer,p,len);
		else
			c2->FillBuffer(m_buffer,p,len);
		turn=!turn;
	}

	result->FromBuffer(m_buffer);

	//return result;
}


}
}