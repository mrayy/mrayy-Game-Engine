
/********************************************************************
	created:	2010/04/02
	created:	2:4:2010   15:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayRBS\RBSParseXML.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayRBS
	file base:	RBSParseXML
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RBSParseXML___
#define ___RBSParseXML___

#include <vector>
#include <mstring.h>

namespace mray{
namespace OS{
	class IStream;
}
namespace xml{
	class XMLElement;
}
namespace AI{

	class RuleBasedSystem;
	struct RBSSymbol;

class RBSParseXML
{
private:
protected:

	void ParseSymbolsToArray(xml::XMLElement*elem,std::vector<RBSSymbol>&arr);

public:
	RBSParseXML();
	virtual~RBSParseXML();

	RuleBasedSystem* ParseXML(OS::IStream*stream);
	RuleBasedSystem* ParseRBS(xml::XMLElement*elem);
	bool ParseRBS(xml::XMLElement*elem,RuleBasedSystem*rbs);
	void ParseSymbols(xml::XMLElement*elem,RuleBasedSystem*rbs);
	void ParseAttachedRBSs(xml::XMLElement*elem,RuleBasedSystem*rbs);
	bool ParseSymbol(xml::XMLElement*elem,RBSSymbol&symbol);

	void ParseRules(xml::XMLElement*elem,RuleBasedSystem*rbs);
	void ParseRule(xml::XMLElement*elem,RuleBasedSystem*rbs);

};

}
}


#endif //___RBSParseXML___
