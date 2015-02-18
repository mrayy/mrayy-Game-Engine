
#include "stdafx.h"
#include "GAChromosomeSelector.h"
#include <mtypes.h>
#include <list>

namespace mray
{
namespace AI
{

GABestFitChromSelector::GABestFitChromSelector(GAFitnessComparator*comp):m_comp(comp)
{}
GABestFitChromSelector::~GABestFitChromSelector()
{}


void GABestFitChromSelector::SelectChromosomes(int cnt,const GAChromosomeInfoList& chroms,std::vector<GAChromosome*> &selectedChroms)
{
	selectedChroms.clear();
	std::list<float> fitness;
	std::list<GAChromosome*> chromsTmp;
	std::list<GAChromosome*>::iterator it;
	std::list<GAChromosome*>::iterator end=chromsTmp.end();


	std::list<float>::iterator fIt;

	for(uint i=0;i<chroms.size();++i)
	{
		const GAChromosomeInfo& info=chroms[i];
		bool add=true;
		fIt=fitness.begin();
		it=chromsTmp.begin();
		for(;it!=chromsTmp.end();++fIt,++it)
		{
			if(m_comp->Compare(info.fitness,(*fIt))==-1){
				chromsTmp.insert(it,info.chrom);
				fitness.insert(fIt,info.fitness);
				add=false;
				break;
			}
		}
		if(add)
		{
			chromsTmp.push_back(info.chrom);
			fitness.push_back(info.fitness);
		}
		if(chromsTmp.size()>cnt)
		{
			fitness.pop_back();
			chromsTmp.pop_back();
		}
	}
	it=chromsTmp.begin();
	for(;it!=chromsTmp.end();++it)
	{
		selectedChroms.push_back((*it));
	}
}


}
}
