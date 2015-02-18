

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   17:12
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAChromOperation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAChromOperation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAChromOperation_h__
#define GAChromOperation_h__

#include "GAChromosomeInfo.h"

namespace mray
{
namespace AI
{

	class GAChromosome;


class GAChromosomeGenerator
{
public:
	GAChromosomeGenerator(){}
	virtual ~GAChromosomeGenerator(){}

	virtual GAChromosome* GenerateChromosome()=0;
};
class GAChromosomeSelector
{
public:
	GAChromosomeSelector(){}
	virtual ~GAChromosomeSelector(){}

	virtual void SelectChromosomes(int cnt,const GAChromosomeInfoList& chroms,std::vector<GAChromosome*> &selectedChroms)=0;
};

class GACrossoverOperation
{
private:
protected:
public:
	GACrossoverOperation(){}
	virtual ~GACrossoverOperation(){}

	virtual void Apply(GAChromosome* c1,GAChromosome* c2,GAChromosome*&result)=0;
};


class GAMutationOperation
{
private:
protected:
public:
	GAMutationOperation(){}
	virtual ~GAMutationOperation(){}

	virtual void Apply(GAChromosome* c)=0;
};


class GAFitnessEvaluator
{
private:
protected:
public:
	GAFitnessEvaluator(){}
	virtual ~GAFitnessEvaluator(){}

	virtual float Evaluate(GAChromosome* c)=0;
};

class GAFitnessComparator
{
private:
protected:
public:
	GAFitnessComparator(){}
	virtual ~GAFitnessComparator(){}

	virtual int Compare(float f1,float f2)=0;
};



}
}

#endif // GAChromOperation_h__
