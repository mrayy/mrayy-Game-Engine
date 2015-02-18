

/********************************************************************
	created:	2011/09/19
	created:	19:9:2011   16:09
	filename: 	d:\Development\mrayEngine\Engine\mraySerialization\GenericTypeClassSerialize.h
	file path:	d:\Development\mrayEngine\Engine\mraySerialization
	file base:	GenericTypeClassSerialize
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GenericTypeClassSerialize__
#define __GenericTypeClassSerialize__

#include "IClassSerialize.h"

namespace mray
{
namespace serialize
{

template <class T>
class GenericTypeClassSerialize:public IClassSerialize
{
private:
protected:
public:
	GenericTypeClassSerialize(const core::string&className):IClassSerialize(className)
	{}
	virtual~GenericTypeClassSerialize()
	{}

	virtual void Serialize(void*obj,xml::XMLElement*elem)
	{
		 xml::IXMLNode*r= new xml::XMLTextNode(core::StringConverter::toString(*((T*)obj)));
		 elem->addSubElement(r);
	}
	virtual bool Deserialize(void*result,xml::XMLElement* elem)
	{
		if(elem->getElementsBegin()==elem->getElementsEnd())
			return false;
		xml::IXMLNode*node=*elem->getElementsBegin();
		if(node->GetType()!=xml::ENT_Text)
			return false;
		xml::XMLTextNode*tnode=dynamic_cast<xml::XMLTextNode*>(elem);
		core::StringConverter::parse(tnode->GetValue(),*((T*)result));

		return true;
	}
};

}
}

#endif