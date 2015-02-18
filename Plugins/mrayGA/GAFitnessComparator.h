

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   19:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAFitnessComparator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAFitnessComparator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAFitnessComparator_h__
#define GAFitnessComparator_h__

#include "CompileConfig.h"
#include "GAChromOperation.h"

namespace mray
{
namespace AI
{
 
class GAMinFitnessComparator:public GAFitnessComparator
{
private:
protected:
public:
	GAMinFitnessComparator(){}
	virtual ~GAMinFitnessComparator(){}

	virtual int Compare(float f1,float f2)
	{
		if(f1<f2)
			return -1;
		else if(f2>f1)
			return 1;
		return 0;
	}
	
};
class GAMaxFitnessComparator:public GAFitnessComparator

{
private:
protected:
public:
	GAMaxFitnessComparator(){}
	virtual ~GAMaxFitnessComparator(){}

	virtual int Compare(float f1,float f2)
	{
		if(f1>f2)
			return -1;
		else if(f2<f1)
			return 1;
		return 0;
	}

};

}
}
#endif // GAFitnessComparator_h__
