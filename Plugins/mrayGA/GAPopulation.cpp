
#include "stdafx.h" 
#include "GAPopulation.h"
#include "GAChromOperation.h"
#include "GAChromosome.h"

#include <Randomizer.h>
#include <assert.h>
#include <list>

namespace mray
{
namespace AI
{

GAPopulation::GAPopulation():m_averageFitness(0)
{
}
GAPopulation::~GAPopulation()
{
	Clear();
}

void GAPopulation::Clear()
{
	for(uint i=0;i<m_chromosomes.size();++i)
	{
		delete m_chromosomes[i].chrom;
	}
	m_chromosomes.clear();
}

void GAPopulation::Generate(int count,GAChromosomeGenerator*generator)
{
	Clear();
	m_chromosomes.resize(count);
	for(int i=0;i<count;++i)
	{
		m_chromosomes[i].chrom=generator->GenerateChromosome();
		m_chromosomes[i].chrom->SetID(i);
	}
}

void GAPopulation::Generate(int count,const std::vector<GAChromosome*> &chroms,
			  GACrossoverOperation*crossover,GAMutationOperation*mutation)
{
	if(chroms.size()<2)
		return;
	//Clear();
	if(count<m_chromosomes.size())
	{
		for(int i=count;i<m_chromosomes.size();++i)
		{
			delete m_chromosomes[i].chrom;
		}
	}
	m_chromosomes.resize(count);
	for(int i=0;i<count;++i)
	{
		int c1ID=math::Randomizer::rand(chroms.size());
		int c2ID=math::Randomizer::rand(chroms.size());
		if(c1ID==c2ID)
		{
			c2ID=(c1ID+1)%chroms.size();
		}

		GAChromosome*c1=chroms[c1ID];
		GAChromosome*c2=chroms[c2ID];

		if(m_chromosomes[i].chrom==0){
			m_chromosomes[i].chrom=c1->CreateCopy();
		}
		m_chromosomes[i].fitness=0;
		GAChromosome* result=m_chromosomes[i].chrom;
		crossover->Apply(c1,c2,result);
		mutation->Apply(result);
		//m_chromosomes[i].chrom=result;
		m_chromosomes[i].chrom->SetID(i);
	}
}

void GAPopulation::CalcFitness(GAFitnessEvaluator*eval)
{
	m_averageFitness=0;
	for(uint i=0;i<m_chromosomes.size();++i)
	{
		m_chromosomes[i].fitness=eval->Evaluate(m_chromosomes[i].chrom);
		m_averageFitness+=m_chromosomes[i].fitness;
	}
	m_averageFitness/=(float)m_chromosomes.size();
}
void GAPopulation::Select(int count,GAChromosomeSelector*selector,std::vector<GAChromosome*> &chroms)
{
	std::vector<GAChromosome*>  chromsTmp;
	selector->SelectChromosomes(count,m_chromosomes,chromsTmp);
	for(int i=0;i<chromsTmp.size();++i)
	{
		chroms.push_back(chromsTmp[i]->CreateCopy());
	}
}

uint GAPopulation::GetPopulationSize()
{
	return m_chromosomes.size();
}
const GAChromosomeInfo& GAPopulation::GetChromosomeInfo(uint i)
{
	assert(i<m_chromosomes.size());
		
	return m_chromosomes[i];
}

const GAChromosomeInfoList& GAPopulation::GetChromosomes()
{
	return m_chromosomes;
}

float GAPopulation::GetAverageFitness()
{
	return m_averageFitness;
}

}
}
