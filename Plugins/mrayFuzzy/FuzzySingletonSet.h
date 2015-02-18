
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   15:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzySingletonSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzySingletonSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzySingletonSet___
#define ___FuzzySingletonSet___


#include "FuzzySet.h"

namespace mray{
namespace AI{

class FuzzySingletonSet:public FuzzySet
{
private:
protected:
	float m_leftRange;
	float m_rightRange;
public:
	FuzzySingletonSet(const core::string &name,float leftRange,float rightRange);
	virtual~FuzzySingletonSet();

	virtual float CalculateDOM(float v);
	virtual float GetMinBound();
	virtual float GetMaxBound();
	xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzySingletonSet___
