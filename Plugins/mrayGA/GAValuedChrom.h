

/********************************************************************
	created:	2010/05/12
	created:	12:5:2010   16:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAValuedChrom.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAValuedChrom
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAValuedChrom_h__
#define GAValuedChrom_h__

#include "GAChromosome.h"
#include "IGAChromValues.h"

namespace mray
{
namespace AI
{
	class GAAlgorithmParameters;

template <class T>
class GAValuedChrom:public GAChromosome
{
private:
protected:
	GAAlgorithmParameters* m_params;
public:
	GAValuedChrom(GAAlgorithmParameters* params):m_params(params)
	{
	}
	virtual ~GAValuedChrom()
	{
	}
	void SetParams(GAAlgorithmParameters* params){m_params=params;}

	virtual const T& GetNearestValue(uint member,const T&v)
	{
		if(m_params)
		{
			IGAChromValues<uint>* values= (IGAChromValues<uint>*)m_params->chromValues;
			return values ? values->GetNearestVal(this,member,v):v;
		}
		else
			return v;
	}
};

}
}

#endif // GAValuedChrom_h__
