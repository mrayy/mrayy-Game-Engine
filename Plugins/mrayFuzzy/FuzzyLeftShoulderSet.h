
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   15:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyLeftShoulderSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyLeftShoulderSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyLeftShoulderSet___
#define ___FuzzyLeftShoulderSet___

#include "FuzzySet.h"

namespace mray{
namespace AI{

class FuzzyLeftShoulderSet:public FuzzySet
{
private:
protected:
	float m_peak;
	float m_minBound;
	float m_maxBound;
public:
	FuzzyLeftShoulderSet(const core::string &name,float minBound,float peak,float maxBound);
	virtual~FuzzyLeftShoulderSet();

	virtual float CalculateDOM(float v);
	virtual float GetMinBound();
	virtual float GetMaxBound();
	xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}

#endif //___FuzzyLeftShoulderSet___
