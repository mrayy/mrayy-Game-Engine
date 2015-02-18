
/********************************************************************
	created:	2010/03/24
	created:	24:3:2010   13:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FCMClustering.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FCMClustering
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	basic Fuzzy C-Means Clustering Algorithm
*********************************************************************/

#ifndef ___FCMClustering___
#define ___FCMClustering___

#include "IFuzzyClustering.h"

namespace mray{
namespace AI{

class FCMClustering:public IFuzzyClustering
{
private:
protected:

	math::GenericMatrix m_weights;
	math::GenericMatrix m_lastWeights;

	math::GenericMatrix m_NormInducingMat;
	math::GenericMatrix m_Distances;

	math::GenericMatrix m_Centers;//Clusters centers

	void initWeightsMatrix();

public:
	FCMClustering();
	virtual~FCMClustering();

	virtual bool DoClustering(const math::GenericMatrix &Z,int c,float tolerance=0.01,float m=2);
	virtual void SetNormInducingMatrix(const math::GenericMatrix&A);

	//Get the result weights
	virtual const math::GenericMatrix& GetWeights()const;
	virtual const math::GenericMatrix & GetCenters()const;
};

}
}


#endif //___FCMClustering___
