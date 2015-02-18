
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzySetProxy.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzySetProxy
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this class used as a proxy to FuzzySet,main use to it for cases you don't want 
				the main Set to be removed, so you use this wraper as shield for the set
*********************************************************************/

#ifndef ___FuzzySetProxy___
#define ___FuzzySetProxy___

#include "FuzzySet.h"

namespace mray{
namespace AI{

class FuzzySetProxy:public FuzzySet
{
private:
protected:
	FuzzySet* m_set;
	core::string m_varName;
public:
	FuzzySetProxy(const core::string &varName,FuzzySet*set);
	virtual~FuzzySetProxy();

	//calculate degree of membership for a given value
	virtual float CalculateDOM(float v);

	virtual float GetRepresentativeValue();

	void SetDOM(float v);
	virtual float GetDOM();
	virtual void ClearDOM();

	virtual float GetMinBound();
	virtual float GetMaxBound();

	virtual void ORWithDOM(float dom);

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}


#endif //___FuzzySetProxy___
