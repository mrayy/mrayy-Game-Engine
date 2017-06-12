

/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   20:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MinMaxPair.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MinMaxPair
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MinMaxPair___
#define ___MinMaxPair___

#include "randomizer.h"

namespace mray{
namespace math{

template <class T>
class MinMaxPair
{
public:
	T minVal,maxVal;
public:
	MinMaxPair():minVal(0),maxVal(0)
	{
	}
	MinMaxPair(const T& v):minVal(v),maxVal(v)
	{
	}
	MinMaxPair(const T& minV,const T& maxV):minVal(minV),maxVal(maxV)
	{
	}

	T getRandomVal()const
	{
		float t=Randomizer::rand01();
		return math::lerp(minVal,maxVal,t);
	}
	T lerp(float t)const
	{
		return math::lerp(minVal,maxVal,t);
	}

	MinMaxPair operator *(float t)const
	{
		return MinMaxPair(minVal*t,maxVal*t);
	}

	MinMaxPair operator +(const MinMaxPair&o)const
	{
		return MinMaxPair(minVal+o.minVal,maxVal+o.maxVal);
	}
};

}
}

#endif //___MinMaxPair___

