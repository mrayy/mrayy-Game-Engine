

/********************************************************************
	created:	2010/05/12
	created:	12:5:2010   16:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAMultiLimitsValues.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAMultiLimitsValues
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAMultiLimitsValues_h__
#define GAMultiLimitsValues_h__

#include <vector>

namespace mray
{
namespace AI
{

template <class T>
class GAMultiLimitsValues:public IGAChromValues<T>
{
private:
protected:
	struct LimitsStruct
	{
		T lower;
		T upper;
		bool use;

		void SetLimits(const T&l,const T&u)
		{
			lower=l;
			upper=u;
			use=true;
		}

		LimitsStruct():lower(0),upper(0),use(false)
		{
		}
		LimitsStruct(const T&l,const T&u):lower(l),upper(u),use(true)
		{
		}

		T GetValue(const T&v)
		{
			if(!use)
				return v;
			if(v<lower)
				return lower;
			else if(v>upper)
				return upper;
			return v;
		}
	};

	std::vector<LimitsStruct> m_limits;

public:
	GAMultiLimitsValues()
	{
	}
	GAMultiLimitsValues(int memCount)
	{
		m_limits.resize(memCount);
	}
	virtual ~GAMultiLimitsValues(){}

	void SetMembersCount(int c)
	{
		m_limits.resize(c);
	}

	//set the limits for chrom member 
	void SetLimits(uint m,const T& lower,const T& upper)
	{
		m_limits[m].SetLimits(lower,upper);
	}

	virtual const T& GetNearestVal(GAChromosome*chrom,uint member,const T& val)
	{
		if(member>=m_limits.size())return val;
		return m_limits[member].GetValue(val);
	}
};

}
}

#endif // GAMultiLimitsValues_h__
