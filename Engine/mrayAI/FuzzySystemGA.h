


/********************************************************************
	created:	2010/05/10
	created:	10:5:2010   18:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\FuzzySystemGA.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	FuzzySystemGA
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	using Genetic Algorithm,optimize and find good rules depending on the fitness values provided
				please notice,the fitness values should be provided before each RunEpoch()
*********************************************************************/

#ifndef FuzzySystemGA_h__
#define FuzzySystemGA_h__

#include "CompileConfig.h"
#include <vector>
#include <mString.h>

namespace mray
{
namespace AI
{

	class FuzzyModule;
	class FuzzyRule;
	class FuzzyVariable;
	class GAModule;
	class GAFitnessEvaluator;
	class GAPopulation;
	class GACrossoverOperation;
	class GAMutationOperation;
	class GAFitnessComparator;
	class GAChromosomeGenerator;
	class GAChromosomeSelector;
	class GAChromosome;

	template <class T>
	class GAMultiLimitsValues;


class MRAY_AI_DLL FuzzySystemGA
{
private:
protected:
	int m_numberOfRules;
	FuzzyModule* m_fuzzyModule;
	GAModule* m_GAAlgo;
	GAFitnessEvaluator* m_eval;

	GACrossoverOperation* m_crossoverOP;
	GAMutationOperation* m_mutationOP;

	GAFitnessComparator* m_comp;
	GAChromosomeGenerator* m_gen;
	GAChromosomeSelector* m_selector;

	GAMultiLimitsValues<uint>* m_limitValues;

	std::vector<core::string> m_varsNames;

	std::vector<FuzzyVariable*> m_inputs;
	std::vector<FuzzyVariable*> m_outputs;

	int m_populationSize;

	int GetVarID(const core::string&v);
public:
	FuzzySystemGA();
	virtual ~FuzzySystemGA();
	
	void SetNumberOfRules(int c);
	void SetFuzzyModule(FuzzyModule*module);
	//void SetFitnessEvaluator(GAFitnessEvaluator*eval);

	void SetPopulationSize(int sz);
	int  GetPopulationSize();

	GAPopulation* Initialize(const std::vector<core::string>& input,const std::vector<core::string> &output);
	GAPopulation* RunEpoch();

	void SetFitnessValue(int chrom,float fitness);

	GAModule* GetGAModule();

	bool ConvertToRules(GAChromosome*chrom,std::vector<FuzzyRule*>&rules);
};

}
}

#endif // FuzzySystemGA_h__