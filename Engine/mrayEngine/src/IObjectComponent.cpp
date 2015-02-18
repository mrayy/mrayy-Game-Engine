

#include "stdafx.h"
#include "IObjectComponent.h"
#include "ILogManager.h"



namespace mray
{

IObjectComponent::IObjectComponent():m_ownerComponent(0)
{
}

IObjectComponent::~IObjectComponent()
{
	ClearComponents();
}
bool IObjectComponent::CanAddComponent(IObjectComponent* comp)
{
	if(!comp)
		return false;
	ComponentsMap::iterator it= m_components.find(comp->getObjectRTTI());
	if(it==m_components.end())
	{
		return true;
	}else if(comp->IsUnique())
		return false;
	else
	{
		return true;
	}
}

bool IObjectComponent::AddComponent(IObjectComponent* comp)
{
	if(!comp)
		return false;
	ComponentsMap::iterator it= m_components.find(comp->getObjectRTTI());
	if(it==m_components.end())
	{
		m_components[comp->getObjectRTTI()].push_back(comp);
		m_componentList.push_back(comp);
		comp->OnAttachedToComponent(this);
		return true;
	}else if(comp->IsUnique())
		return false;
	else
	{
		it->second.push_back(comp);
		comp->OnAttachedToComponent(this);
		m_componentList.push_back(comp);
		return true;
	}

}


void IObjectComponent::RemoveComponent(IObjectComponent* comp)
{
	if(!comp)
		return;
	ComponentsMap::iterator it= m_components.find(comp->getObjectRTTI());
	if(it==m_components.end())
		return;

	std::list<IObjectComponent*>& lst=it->second;
	std::list<IObjectComponent*>::iterator cit= lst.begin();
	for(;cit!=lst.end();++cit)
	{
		if(*cit==comp)
		{

			m_componentList.remove(comp);
			lst.erase(cit);
			comp->OnRemovedFromComponent(this);

			return;
		}
	}
}


const IObjectComponent::ComponentsMap& IObjectComponent::GetComponents()const
{
	return m_components;
}


void IObjectComponent::ClearComponents()
{
	ComponentsMap::iterator it= m_components.begin();
	for (;it!=m_components.end();++it)
	{
		std::list<IObjectComponent*>& lst=it->second;
		std::list<IObjectComponent*>::iterator cit= lst.begin();
		for(;cit!=lst.end();++cit)
		{
			delete *cit;
		}
	}
	m_componentList.clear();
	m_components.clear();
}


void IObjectComponent::OnAttachedToComponent(IObjectComponent* owner)
{
	m_ownerComponent=owner;
}
void IObjectComponent::OnRemovedFromComponent(IObjectComponent* owner)
{
	m_ownerComponent=0;
}

IObjectComponent* IObjectComponent::GetOwnerComponent()const
{
	return m_ownerComponent;
}


const std::list<IObjectComponent*>& IObjectComponent::GetComponent(const RTTI* type)const
{
	static std::list<IObjectComponent*> NullList;

	ComponentsMap::const_iterator it= m_components.find(type);
	if(it==m_components.end())
		return NullList;

	return it->second;
}

const std::list<IObjectComponent*>& IObjectComponent::GetComponent(const core::string& type)const
{
	static std::list<IObjectComponent*> NullList;
	ComponentsMap::const_iterator it= m_components.begin();
	for (;it!=m_components.end();++it)
	{
		if(!it->first->getTypeName().equals_ignore_case(type))
			continue;
		return it->second;
	}
	return NullList;
}

IObjectComponent* IObjectComponent::GetFirstComponent(const RTTI* type)const
{
	ComponentsMap::const_iterator it= m_components.find(type);
	if(it==m_components.end())
		return 0;
	if(it->second.empty())
		return 0;
	return *it->second.begin();
}
IObjectComponent* IObjectComponent::GetFirstComponent(const core::string& type)const
{
	ComponentsMap::const_iterator it= m_components.begin();
	for (;it!=m_components.end();++it)
	{
		if(!it->first->getTypeName().equals_ignore_case(type))
			continue;
		if(it->second.empty())
			return 0;
		return *it->second.begin();
	}
	return 0;
}

void IObjectComponent::_SearchForComponentByType(const RTTI* type,std::list<IObjectComponent*>& outlst)const
{
	ComponentsMap::const_iterator it= m_components.find(type);
	if(it==m_components.end())
		return;

	outlst.insert(outlst.begin(),it->second.begin(),it->second.end());

	it= m_components.begin();
	for (;it!=m_components.end();++it)
	{
		const std::list<IObjectComponent*>& lst=it->second;
		std::list<IObjectComponent*>::const_iterator cit= lst.begin();
		for(;cit!=lst.end();++cit)
		{
			(*cit)->_SearchForComponentByType(type,outlst);
		}
	}
}

std::list<IObjectComponent*> IObjectComponent::SearchForComponentByType(const RTTI* type)const
{
	std::list<IObjectComponent*> ret;
	_SearchForComponentByType(type,ret);
	return ret;
}

xml::XMLElement* IObjectComponent::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IObject::loadXMLSettings(elem);


	return ret;
}

xml::XMLElement*  IObjectComponent::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IObject::exportXMLSettings(elem);

	if(m_components.size())
	{
		xml::XMLElement* comps=new xml::XMLElement(mT("Components"));
		ret->addSubElement(comps);
		std::list<IObjectComponent*>::iterator it= m_componentList.begin();
		for(;it!=m_componentList.end();++it)
		{
			(*it)->exportXMLSettings(comps);
		}
	}

	return ret;
}

}
