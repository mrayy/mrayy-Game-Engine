

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   17:39
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAPopulation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAPopulation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAPopulation_h__
#define GAPopulation_h__

#include "CompileConfig.h"
#include "GAChromosomeInfo.h"
#include <mtypes.h>

namespace mray
{
namespace AI
{

	class GAChromosome;
	class GAChromosomeGenerator;
	class GAFitnessEvaluator;
	class GAChromosomeSelector;
	class GAMutationOperation;
	class GACrossoverOperation;



class MRAYGA_API GAPopulation
{
private:
protected:


	GAChromosomeInfoList m_chromosomes;
	
	float m_averageFitness;

public:
	GAPopulation();
	virtual ~GAPopulation();

	void Clear();
	void Generate(int count,GAChromosomeGenerator*generator);

	//Generate population starting from chromosomes
	void Generate(int count,const std::vector<GAChromosome*> &chroms,
		GACrossoverOperation*crossover,GAMutationOperation*mutation);

	void CalcFitness(GAFitnessEvaluator*eval);
	void Select(int count,GAChromosomeSelector*selector,std::vector<GAChromosome*> &chroms);

	uint GetPopulationSize();
	const GAChromosomeInfo& GetChromosomeInfo(uint i);
	const GAChromosomeInfoList& GetChromosomes();

	float GetAverageFitness();

};

}
}

#endif // GAPopulation_h__
