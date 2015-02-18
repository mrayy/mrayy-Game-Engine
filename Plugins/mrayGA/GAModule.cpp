 
#include "stdafx.h"
#include "GAModule.h"
#include "GAPopulation.h"
#include "GAChromosome.h"


namespace mray
{
namespace AI
{

GAModule::GAModule():m_population(0),m_listner(0)
{
	
}
GAModule::~GAModule()
{
	Clear();
}

void GAModule::Clear()
{
	delete m_population;
}

GAPopulation* GAModule::Initialize(const GAAlgorithmParameters& params)
{
	Clear();
	if(!params.IsValid())
		return 0;

	m_params=params;

	m_population=new GAPopulation();

	m_population->Generate(m_params.populationSize,m_params.chromosomeGen);

	return m_population;
}
GAPopulation* GAModule::RunEpoch()
{
	if(!m_population)
		return 0;

	std::vector<GAChromosome*> parents;

	//Calc fitness
	m_population->CalcFitness(m_params.fitnessEval);
	m_population->Select(m_params.selectedParentsCnt,m_params.chromSelector,parents);

	if(m_listner){
		m_listner->SelectedParents(parents);
	}

	m_population->Generate(m_params.populationSize,parents,m_params.crossoverOp,m_params.mutationOp);

	for(int i=0;i<parents.size();++i){
		delete parents[i];
	}

	return m_population;
}

GAPopulation* GAModule::GetPopulation()
{
	return m_population;
}

void GAModule::SetListener(GAAlgorithmListener*l)
{
	m_listner=l;
}

GAAlgorithmParameters* GAModule::GetParams()
{
	return &m_params;
}


}
}

