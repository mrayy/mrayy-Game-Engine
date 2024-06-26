
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyVeryHedge.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyVeryHedge
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyVeryHedge___
#define ___FuzzyVeryHedge___

#include "FuzzyTerm.h"

namespace mray{
namespace AI{

class FuzzyVeryHedge:public FuzzyTerm
{
private:
protected:

	FuzzyTerm* m_term;

public:
	FuzzyVeryHedge(FuzzyTerm*term);
	virtual~FuzzyVeryHedge();

	virtual float GetDOM();
	virtual void ClearDOM();
	virtual void ORWithDOM(float dom);

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzyVeryHedge___
