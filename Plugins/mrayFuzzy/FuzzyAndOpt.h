
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyAndOpt.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyAndOpt
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyAndOpt___
#define ___FuzzyAndOpt___

#include "FuzzyTerm.h"
#include <mArray.h>

namespace mray{
namespace AI{

class FuzzyAndOpt:public FuzzyTerm
{
private:
protected:
	core::array<FuzzyTerm*> m_terms;
public:
	FuzzyAndOpt();
	FuzzyAndOpt(FuzzyTerm*t1,FuzzyTerm*t2);
	virtual~FuzzyAndOpt();

	void AddTerm(FuzzyTerm*t);
	int getTermsCount();

	virtual float GetDOM();
	virtual void ClearDOM();
	virtual void ORWithDOM(float dom);

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzyAndOpt___
