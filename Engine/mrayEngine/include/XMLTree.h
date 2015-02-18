
#ifndef ___XMLTree___
#define ___XMLTree___

#include "XMLElement.h"

namespace mray{
	namespace OS{
		class IStream;
	}
namespace xml{


class MRAY_DLL XMLTree:public XMLElement
{
public:
	XMLTree();
	virtual~XMLTree();

/*
	virtual void onStart(){}
	virtual void startElementHandler(XMLElement*attrs)=0;
	virtual void endElementHandler(const core::string&elementName)=0;
	virtual void texDataHandler(const core::string&text)=0;
	virtual void onDone(bool success){}
	*/

	bool load(OS::IStream* stream);
	bool load(const core::string& path);
};

}
}




#endif

