#include "stdafx.h"

#include "IGameAction.h"

#include "XMLElement.h"
#include "IGameVisitor.h"
#include "GameEntity.h"

namespace mray
{
namespace game
{

IGameAction::IGameAction()
{
}
IGameAction::~IGameAction()
{
	/*
	ValuesMap::iterator it=m_parameters.begin();
	for (;it!=m_parameters.end();++it)
	{
		delete it->second;
	}*/
	
}
void IGameAction::AddAttribute(IValue*v)
{
	m_parameters[v->getName()]=v;
}

const ValuesMap& IGameAction::GetAttributes()
{
	return m_parameters;
}

IValue* IGameAction::GetAttribute(const core::string&name)
{
	ValuesMap::iterator it= m_parameters.find(name);
	if(it==m_parameters.end())
		return 0;
	return it->second;
}

xml::XMLElement* IGameAction::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret= IGameComponent::loadXMLSettings(elem);
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Value")))
		{
			xml::XMLAttribute*attr= e->getAttribute(mT("Name"));
			if(!attr)
				continue;
			IValue*v= GetAttribute(attr->value);
			if(v)
			{
				v->loadXMLSettings(e);
			}
		}
	}
	return ret;
}
xml::XMLElement*  IGameAction::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e=IGameComponent::exportXMLSettings(elem);
	elem->addSubElement(e);

	//e->addAttribute(mT("Name"),GetName());

	ValuesMap::iterator it=m_parameters.begin();
	for (;it!=m_parameters.end();++it)
	{
		it->second->exportXMLSettings(e);
	}

	return e;
}

void IGameAction::Visit(IGameVisitor* visitor)
{
	visitor->OnVisit(this);
}

}
}
