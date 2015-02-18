

/********************************************************************
	created:	2010/05/12
	created:	12:5:2010   16:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAAlgorithmParameters.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAAlgorithmParameters
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAAlgorithmParameters_h__
#define GAAlgorithmParameters_h__

#include "mTypes.h"

namespace mray
{
namespace AI
{
	class GACrossoverOperation;
	class GAMutationOperation;
	class GAFitnessEvaluator;
	class GAChromosomeSelector;
	class GAChromosomeGenerator;

class GAAlgorithmParameters
{
public:

	GAAlgorithmParameters():crossoverOp(0),mutationOp(0),fitnessEval(0),chromSelector(0),chromosomeGen(0),populationSize(20),selectedParentsCnt(4),chromValues(0)
	{
	}

	bool IsValid()const
	{
		return crossoverOp && mutationOp && fitnessEval && chromSelector && chromosomeGen && populationSize>0 && selectedParentsCnt>=2 ; 
	}

	GACrossoverOperation* crossoverOp;
	GAMutationOperation* mutationOp;

	GAFitnessEvaluator* fitnessEval;
	GAChromosomeSelector* chromSelector;

	GAChromosomeGenerator* chromosomeGen;

	void* chromValues;	//should contain a pointer to IGAChromValues or as null

	uint populationSize;
	uint selectedParentsCnt;
};

}
}

#endif // GAAlgorithmParameters_h__
