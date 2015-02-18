#include "stdafx.h"
 
#include "IObject.h"
#include "IPropertyVistor.h"

namespace mray{


	
IObject::IObject(){
}
IObject::~IObject(){
}

core::string IObject::toString()const
{
	return this->getObjectType();
}
void IObject::parse(const core::string&str){
}

void IObject::fillProperties(){
}

xml::XMLElement* IObject::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e=elem;//elem->getSubElement(getObjectType());
	if(!e)return 0;
	CPropertieSet::loadFromXML(e);
	return e;
}
xml::XMLElement*  IObject::exportXMLSettings(xml::XMLElement* elem){
	xml::XMLElement* e=new xml::XMLElement(getObjectType());
	CPropertieSet::exportToXML(e);
	elem->addSubElement(e);
	return e;
}
void IObject::OnVisit(IPropertyVistor* visitor)
{
	visitor->Visit(this);
}


}