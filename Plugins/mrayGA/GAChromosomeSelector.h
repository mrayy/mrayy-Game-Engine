

/********************************************************************
	created:	2010/05/10
	created:	10:5:2010   19:11
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAChromosomeSelector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAChromosomeSelector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAChromosomeSelector_h__
#define GAChromosomeSelector_h__

#include "CompileConfig.h"
#include "GAChromOperation.h"

namespace mray
{
namespace AI
{

class MRAYGA_API GABestFitChromSelector:public GAChromosomeSelector
{
private:
protected:
	GAFitnessComparator* m_comp;
public:
	GABestFitChromSelector(GAFitnessComparator*comp);
	virtual ~GABestFitChromSelector();

	virtual void SelectChromosomes(int cnt,const GAChromosomeInfoList& chroms,std::vector<GAChromosome*> &selectedChroms);
};

}
}

#endif // GAChromosomeSelector_h__
