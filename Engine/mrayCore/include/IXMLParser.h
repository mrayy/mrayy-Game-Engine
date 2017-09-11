
#ifndef ___IXMLParser___
#define ___IXMLParser___

#include "mString.h"
#include "ISingleton.h"

namespace mray
{
namespace OS
{
	class MRAY_CORE_DLL IStream;
}
namespace xml
{

	class MRAY_CORE_DLL XMLTree;

class MRAY_CORE_DLL  IXMLParser:public ISingleton<IXMLParser>
{
public:

	IXMLParser(){}
	virtual~IXMLParser(){}

	virtual bool parserXML(OS::IStream*stream, XMLTree*tree) = 0;
	virtual bool parserXML(const core::string& xml, XMLTree*tree) = 0;

	virtual core::string getParserName()=0;

};

}
}




#endif

