
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   15:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyTriangleSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyTriangleSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyTriangleSet___
#define ___FuzzyTriangleSet___


#include "FuzzySet.h"

namespace mray{
namespace AI{

class FuzzyTriangleSet:public FuzzySet
{
private:
protected:
	float m_peak;
	float m_minBound;
	float m_maxBound;
public:
	FuzzyTriangleSet(const core::string &name,float minBound,float peak,float maxBound);
	virtual~FuzzyTriangleSet();

	virtual float CalculateDOM(float v);

	virtual float GetMinBound();
	virtual float GetMaxBound();

	xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzyTriangleSet___
