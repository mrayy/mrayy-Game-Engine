#include "stdafx.h"


#include "ObjectSerializer.h"
#include "IObject.h"

#include "XMLTextNode.h"

namespace mray
{

ObjectSerializer::ObjectSerializer()
{
	m_currElement=0;
	m_currObject=0;
}
ObjectSerializer::~ObjectSerializer()
{
}

xml::XMLElement* ObjectSerializer::Serialize(IObject* object)
{
	m_currElement=0;
	xml::XMLElement* e=WriteElement(object->getObjectType());
	Visit(object);
	return e;
}

xml::XMLElement* ObjectSerializer::WriteElement(const core::string&name)
{
	xml::XMLElement*r= new xml::XMLElement(name);
	if(!m_currElement)
		m_currElement=r;
	else
		m_currElement->addSubElement(r);
	m_currElement=r;
	return r;
}
void ObjectSerializer::Visit(IObject*prop)
{

	Visit((CPropertieSet*)prop);
}

void ObjectSerializer::Visit(CPropertieSet*prop)
{
	m_currObject=prop;
	if(m_currObject->GetDictionary())
		m_currObject->GetDictionary()->OnVisit(this);


	{
		const CPropertieSet::SubsetMap& lst=prop->GetSubSets();
		CPropertieSet::SubsetMap::const_iterator it=lst.begin();
		for(;it!=lst.end();++it)
		{
			m_currElement=WriteElement(it->first);
			it->second->OnVisit(this);
			m_currElement=m_currElement->getParent();
		}
	}
}
void ObjectSerializer::Visit(CPropertieDictionary*prop)
{
	{
		const TPropertieList& lst=prop->getProperties();
		TPropertieList::const_iterator it=lst.begin();
		for(;it!=lst.end();++it)
		{
			it->second->OnVisit(this);
		}
	}

}
void ObjectSerializer::Visit(IProperty*prop)
{
	if(!prop->getFlag(EPF_Serialize)/* ||
		!prop->getFlag(EPF_Read)*/)
		return;
	xml::XMLElement*e=new xml::XMLElement(prop->getName());
	e->addSubElement(new xml::XMLTextNode(prop->toString(m_currObject)));
	m_currElement->addSubElement(e);
}

}

