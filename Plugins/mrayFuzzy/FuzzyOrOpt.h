
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:28
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyOrOpt.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyOrOpt
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyOrOpt___
#define ___FuzzyOrOpt___


#include "FuzzyTerm.h"
//#include <mArray.h>

namespace mray{
namespace AI{

class FuzzyOrOpt:public FuzzyTerm
{
private:
protected:
	std::vector<FuzzyTerm*> m_terms;
public:
	FuzzyOrOpt();
	FuzzyOrOpt(FuzzyTerm*t1,FuzzyTerm*t2);
	virtual~FuzzyOrOpt();

	void AddTerm(FuzzyTerm*t);
	int getTermsCount();

	virtual float GetDOM();
	virtual void ClearDOM();
	virtual void ORWithDOM(float dom);

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzyOrOpt___
