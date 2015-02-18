
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   13:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzySet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzySet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzySet___
#define ___FuzzySet___

#include "FuzzyTerm.h"
#include "mString.h"

namespace mray{
namespace AI{

class FuzzySet:public FuzzyTerm
{
private:
protected:
	float m_DOM;
	float m_RepVal;

	core::string m_name;

public:
	FuzzySet(const core::string &name);
	FuzzySet(const core::string &name,float repVal);
	virtual~FuzzySet();

	const core::string&GetName(){return m_name;}

	//calculate degree of membership for a given value
	virtual float CalculateDOM(float v)=0;
	
	virtual float GetRepresentativeValue(){return m_RepVal;}

	void SetDOM(float v);
	virtual float GetDOM();
	virtual void ClearDOM();

	virtual float GetMinBound()=0;
	virtual float GetMaxBound()=0;

	virtual void ORWithDOM(float dom){if(dom>m_DOM)m_DOM=dom;}

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzySet___
