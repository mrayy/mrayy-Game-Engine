
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyFairlyHedge.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyFairlyHedge
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyFairlyHedge___
#define ___FuzzyFairlyHedge___

#include "FuzzyTerm.h"

namespace mray{
namespace AI{

class FuzzyFairlyHedge:public FuzzyTerm
{
private:
protected:
	FuzzyTerm* m_term;
public:
	FuzzyFairlyHedge(FuzzyTerm*term);
	virtual~FuzzyFairlyHedge();

	virtual float GetDOM();
	virtual void ClearDOM();
	virtual void ORWithDOM(float dom);

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzyFairlyHedge___
