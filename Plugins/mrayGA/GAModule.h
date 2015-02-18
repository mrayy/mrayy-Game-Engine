


/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   16:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAAlgorithm.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAAlgorithm
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAAlgorithm_h__
#define GAAlgorithm_h__

#include "CompileConfig.h"
#include "GAAlgorithmParameters.h"
#include <mTypes.h>
#include <vector>

namespace mray
{
namespace AI
{

	class GACrossoverOperation;
	class GAMutationOperation;
	class GAFitnessEvaluator;
	class GAChromosomeSelector;
	class GAChromosomeGenerator;

	class GAChromosome;
	class GAPopulation;


class GAAlgorithmListener
{
public:
	virtual void SelectedParents(const std::vector<GAChromosome*> &chroms){}
};

class MRAYGA_API GAModule
{
private:
protected:

	GAPopulation* m_population;
	GAAlgorithmParameters m_params;

	GAAlgorithmListener* m_listner;

	void Clear();

public:
	GAModule();
	virtual ~GAModule();

	GAPopulation* Initialize(const GAAlgorithmParameters& params);
	GAPopulation* RunEpoch();

	GAPopulation* GetPopulation();

	void SetListener(GAAlgorithmListener*l);

	GAAlgorithmParameters* GetParams();
	
};

}
}


#endif // GAAlgorithm_h__
