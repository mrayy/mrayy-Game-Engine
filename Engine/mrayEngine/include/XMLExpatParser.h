

#ifndef ___XMLExpatParser___
#define ___XMLExpatParser___

#include "IXMLParser.h"

namespace mray{
namespace xml{


class XMLExpatParser:public IXMLParser
{
public:

	XMLExpatParser();
	virtual~XMLExpatParser();

	virtual bool parserXML(OS::IStream*stream,XMLTree*tree);
	virtual bool parserXML(const core::string& xml, XMLTree*tree);

	virtual core::string getParserName();

};

}
}



#endif

