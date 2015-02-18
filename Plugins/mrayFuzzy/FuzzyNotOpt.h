
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   20:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyNotOpt.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyNotOpt
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyNotOpt___
#define ___FuzzyNotOpt___



#include "FuzzyTerm.h"
namespace mray{
namespace AI{

class FuzzyNotOpt:public FuzzyTerm
{
private:
protected:
	FuzzyTerm* m_term;
public:
	FuzzyNotOpt(FuzzyTerm*t);
	virtual~FuzzyNotOpt();

	virtual float GetDOM();
	virtual void ClearDOM();
	virtual void ORWithDOM(float dom);

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}

#endif //___FuzzyNotOpt___
