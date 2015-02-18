#include "stdafx.h"

#include "FCMClustering.h"
#include <Randomizer.h>
#include <mMath.h>

namespace mray{
namespace AI{

FCMClustering::FCMClustering()
{
}
FCMClustering::~FCMClustering()
{
}


void FCMClustering::initWeightsMatrix()
{
	float**m=m_weights.getMat();
	int r=m_weights.getRowsCount();
	int c=m_weights.getColsCount();

	int i,j;
	float len;

	for (i=0;i<r;++i)
	{
		len=0;
		for (j=0;j<c;++j)
		{
			m[i][j]=math::Randomizer::rand01()*0.99+0.01;//make sure it's > 0
			len+=m[i][j];
		}
		//normalize weight vector
		len=1.0f/len;
		for (j=0;j<c;++j)
		{
			m[i][j]*=len;
		}
	}
}

bool FCMClustering::DoClustering(const math::GenericMatrix &Z,int c,float tolerance,float m)
{
	if(m<=1)
		m=1.1f;
	float invM=1.0f/(m-1);
	int samples=Z.getColsCount();
	int attrs=Z.getRowsCount();

	if(c>=samples)
		c=samples-1;
	if(c<=1)
		c=2;

	if(m_NormInducingMat.getRowsCount()!=attrs)
		return false;

	m_weights.setRowsColsCount(c,samples);
	m_lastWeights.setRowsColsCount(c,samples);
	m_Centers.setRowsColsCount(attrs,c);
	m_Distances.setRowsColsCount(c,samples);

	math::GenericMatrix tempVector;
	math::GenericMatrix diffVector;
	tempVector.setRowsColsCount(1,attrs);
	diffVector.setRowsColsCount(1,attrs);

	float**weightsPtr=m_weights.getMat();
	float**lastWeightsPtr=m_lastWeights.getMat();
	float**centersPtr=m_Centers.getMat();
	float**distPtr=m_Distances.getMat();
	float**tmpVecPtr=tempVector.getMat();
	float**diffVecPtr=diffVector.getMat();
	const float**samplesPtr=Z.getMat();

	core::array<int> ZeroDistSamples;
	ZeroDistSamples.resize(c);

	//iteration counters
	int l=0;
	int i,j,k,it1;

	float len=0;

	//init weights matrix to random values
	initWeightsMatrix();

	float error=0;

	do 
	{
		error=0;
		//Compute the cluster prototypes (means)
		for (i=0;i<c;++i)
		{
			float bottom=0;
			for (j=0;j<attrs;++j)
			{
				centersPtr[j][i]=0;
			}
			for (k=0;k<samples;++k)
			{
				//multiply weight with the sample
				float w=pow(weightsPtr[i][k],m);
				for (j=0;j<attrs;++j)
				{
					centersPtr[j][i]+=w*samplesPtr[j][k];
				}
				bottom+=w;
			}
			if(bottom>0){
				bottom=1.0f/bottom;
				//normalize center(i)
				for (j=0;j<attrs;++j)
				{
					centersPtr[j][i]*=bottom;
				}
			}
		}
		//compute the distances
		for (i=0;i<c;++i)
		{
			for (k=0;k<samples;++k)
			{
				for (j=0;j<attrs;++j)
				{
					tmpVecPtr[0][j]=samplesPtr[j][k]-centersPtr[j][i];
				}
				math::GenericMatrix::MultiplyMatricies(tempVector,m_NormInducingMat,diffVector);
				//calculate vector length
				distPtr[i][k]=0;
				for (j=0;j<attrs;++j)
				{
					distPtr[i][k]+=diffVecPtr[0][j]*diffVecPtr[0][j];
				}
			}
		}

		//Update the partition matrix
		for (k=0;k<samples;++k)
		{
			ZeroDistSamples.resize(0);
			for (i=0;i<c;++i)
			{
				if(distPtr[i][k]==0)
					ZeroDistSamples.push_back(i);
			}
			if(ZeroDistSamples.size()==0)//no zero distance for this cluster
			{
				for (i=0;i<c;++i)
				{
					float bottom=0;
					for (it1=0;it1<c;++it1)
					{
						bottom+=pow(distPtr[i][k]/distPtr[it1][k],invM);
					}
					lastWeightsPtr[i][k]=weightsPtr[i][k];
					weightsPtr[i][k]=1.0f/bottom;
				}
			}else
			{
				//zero all weights for other clusters
				float invSize=1.0f/ZeroDistSamples.size();
				for (it1=0;it1<c;++it1)
				{
					lastWeightsPtr[it1][k]=weightsPtr[it1][k];
					weightsPtr[it1][k]=0;
				}
				for(it1=0;it1<ZeroDistSamples.size();++it1)
				{
					int idx=ZeroDistSamples[it1];
					float lastW=weightsPtr[idx][k];
					lastWeightsPtr[it1][k]=weightsPtr[it1][k];
					weightsPtr[idx][k]=invSize;
				}
			}
			for (i=0;i<c;++i)
			{
				error=math::Max(error,abs(lastWeightsPtr[i][k]-weightsPtr[i][k]));
			}
		}

		++l;
	} while (error>tolerance);
	return true;
}
void FCMClustering::SetNormInducingMatrix(const math::GenericMatrix&A)
{
	m_NormInducingMat=A;
}

//Get the result weights
const math::GenericMatrix& FCMClustering::GetWeights()const
{
	return m_weights;
}
const math::GenericMatrix& FCMClustering::GetCenters()const
{
	return m_Centers;
}

}
}