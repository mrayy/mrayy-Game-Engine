

#ifndef ___ISerializable___
#define ___ISerializable___

#include "IXMLParser.h"
#include "XMLElement.h"
#include "GCPtr.h"

namespace mray{

class MRAY_DLL ISerializable{
public:
	
	virtual~ISerializable(){
	}

	virtual void loadXMLSettings(xml::XMLElement* elem)=0;
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem)=0;
};
}

#endif

