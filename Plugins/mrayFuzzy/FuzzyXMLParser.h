
/********************************************************************
	created:	2010/03/25
	created:	25:3:2010   20:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyXMLParser.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyXMLParser
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyXMLParser___
#define ___FuzzyXMLParser___

#include "FuzzyModule.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace xml{
	class XMLElement;
}
namespace AI{

class FuzzyXMLParser
{
private:
protected:
public:
	FuzzyXMLParser();
	virtual~FuzzyXMLParser();

	FuzzyModule* ParseXML(OS::IStream*stream);

	//FuzzyModule* ParseModuleElement(xml::XMLElement*element);
	bool ParseModuleElement(xml::XMLElement*element,FuzzyModule*module);
	IDefuzzifyOperation* ParseDefuzzyElement(xml::XMLElement*element);

	void ParseVariables(xml::XMLElement*element,FuzzyModule*module);
	void ParseRules(xml::XMLElement*element,FuzzyModule*module);

	FuzzyVariable* ParseVariable(xml::XMLElement*element);
	FuzzyRule* ParseRule(xml::XMLElement*element,FuzzyModule*module);
	FuzzySet* ParseSet(const core::string&setName,xml::XMLElement*element);
	FuzzyTerm* ParseTerm(xml::XMLElement*element,FuzzyModule*module);


};

}
}


#endif //___FuzzyXMLParser___
