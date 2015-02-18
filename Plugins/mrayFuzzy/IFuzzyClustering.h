
/********************************************************************
	created:	2010/03/24
	created:	24:3:2010   13:11
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\IFuzzyClustering.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	IFuzzyClustering
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	giving data set,distribute the data on clusters with fuzziness factors
				(member ship values) assigned to each datum for each cluster
*********************************************************************/

#ifndef ___IFuzzyClustering___
#define ___IFuzzyClustering___


#include <mArray.h>
#include <GenericMatrix.h>

namespace mray{
namespace AI{

class IFuzzyClustering
{
private:
protected:
public:
	IFuzzyClustering(){}
	virtual~IFuzzyClustering(){}


	//Z:data vectors (NxM),where N is the number of data and M is the vector length
	//C:Number of clusters,1< C <N
	//tolerance >0 : stop factor
	//A:Norm Inducing Matrix
	//m: fuzziness factor , 1< m 
	//	 notice when m->1 , there would be no fuzziness
	virtual bool DoClustering(const math::GenericMatrix &Z,int c,float tolerance=0.01,float m=2)=0;
	virtual void SetNormInducingMatrix(const math::GenericMatrix &A)=0;

	//Get the result weights
	//Rows are clusters,and Columns are the weights for each (data,cluster)
	virtual const math::GenericMatrix & GetWeights()const=0;
	
	//Get clusters center
	virtual const math::GenericMatrix & GetCenters()const=0;

};

}
}


#endif //___IFuzzyClustering___

