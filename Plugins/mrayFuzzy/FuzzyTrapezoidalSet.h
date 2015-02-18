
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   15:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyTrapezoidalSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyTrapezoidalSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyTrapezoidalSet___
#define ___FuzzyTrapezoidalSet___

#include "FuzzySet.h"


namespace mray{
namespace AI{

class FuzzyTrapezoidalSet:public FuzzySet
{
private:
protected:
	float m_leftPeak;
	float m_rightPeak;
	float m_minBound;
	float m_maxBound;
public:
	FuzzyTrapezoidalSet(const core::string &name,float minBound,float leftPeak,float rightPeak,float maxBound);
	virtual~FuzzyTrapezoidalSet();

	virtual float CalculateDOM(float v);

	virtual float GetMinBound();
	virtual float GetMaxBound();
	xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}

#endif //___FuzzyTrapezoidalSet___
