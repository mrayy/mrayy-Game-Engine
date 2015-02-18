
#ifndef ___IXMLParser___
#define ___IXMLParser___

#include "mString.h"
#include "ISingleton.h"

namespace mray
{
namespace OS
{
	class IStream;
}
namespace xml
{

	class XMLTree;

class MRAY_DLL IXMLParser:public ISingleton<IXMLParser>
{
public:

	IXMLParser(){}
	virtual~IXMLParser(){}

	virtual bool parserXML(OS::IStream*stream,XMLTree*tree)=0;

	virtual core::string getParserName()=0;

};

}
}




#endif

