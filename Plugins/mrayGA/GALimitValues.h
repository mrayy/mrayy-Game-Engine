

/********************************************************************
	created:	2010/05/12
	created:	12:5:2010   15:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GALimitConstraint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GALimitConstraint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GALimitConstraint_h__
#define GALimitConstraint_h__

#include "IGAChromValues.h"

namespace mray
{
namespace AI
{


template <class T>
class GALimitConstraint:public IGAChromValues<T>
{
private:
protected:
	T m_lower;
	T m_upper;
	bool m_clamp;

	uint m_tmpResult;
public:
	GALimitConstraint(T lower,T upper,bool clamp=false)
		:m_lower(lower),m_upper(upper),m_clamp(clamp)
	{
	}
	virtual ~GALimitConstraint()
	{
	}

	virtual const T& GetNearestVal(GAChromosome*chrom,uint member,const T& val)
	{
		m_tmpResult=val;
		if(val<m_lower)
			m_tmpResult=m_lower;
		else if(val>m_upper)
			m_tmpResult=m_upper;
		return m_tmpResult;
	}
};

}
}

#endif // GALimitConstraint_h__
