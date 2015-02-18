
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   15:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyRightShoulderSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyRightShoulderSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyRightShoulderSet___
#define ___FuzzyRightShoulderSet___

#include "FuzzySet.h"

namespace mray{
namespace AI{


class FuzzyRightShoulderSet:public FuzzySet
{
private:
protected:
	float m_peak;
	float m_minBound;
	float m_maxBound;
public:
	FuzzyRightShoulderSet(const core::string &name,float minBound,float peak,float maxBound);
	virtual~FuzzyRightShoulderSet();

	virtual float CalculateDOM(float v);
	virtual float GetMinBound();
	virtual float GetMaxBound();
	xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}

#endif //___FuzzyRightShoulderSet___
