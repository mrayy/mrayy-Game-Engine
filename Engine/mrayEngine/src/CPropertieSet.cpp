#include "stdafx.h"


#include "CPropertieSet.h"
#include "GenericLoggerSystem.h"
#include "macros.h"
#include "DelegateManager.h"
#include "IPropertyVistor.h"

namespace mray
{

CPropertieDictionary::CPropertieDictionary(const RTTI*rtti):m_rtti(rtti)
{
}
CPropertieDictionary::~CPropertieDictionary(){
	m_properties.clear();
	m_groups.clear();
} 

void CPropertieDictionary::addPropertie(IProperty*prop,const core::string&group){
	core::string s1=prop->getName();
	s1.make_lower();
	m_properties.insert(std::make_pair(s1,prop));
	core::string s2=group;
	s2.make_lower();
	TPropertieGroupList::iterator it=m_groups.find(s2);
	if(it!=m_groups.end()){
		it->second.properties.insert(std::make_pair(s1,(IProperty*)prop));
	}else{
		SPropertyGroup g;
		g.name=s2;
		g.properties.insert(std::make_pair(s1,(IProperty*)prop));
		m_groups.insert(std::make_pair(s2,g));
	}
}


void CPropertieDictionary::addSubDictionary(const core::string&name, CPropertieDictionary* set)
{
	if(set)
		m_subDics[name]=set;
}

CPropertieDictionary* CPropertieDictionary::GetSubDictionary(const core::string&name)
{

	TSubPropertyDictionaryIT it=m_subDics.find(name);
	if(it!=m_subDics.end()){
		return (it->second);
	}
	return 0;
}

const TSubPropertyDictionary& CPropertieDictionary::getSubDictionaries()
{
	return m_subDics;
}


SPropertyGroup*CPropertieDictionary::getGroup(const core::string&name){
	core::string s=name;
	s.make_lower();
	TPropertieGroupList::iterator it=m_groups.find(s);
	if(it!=m_groups.end()){
		return &(it->second);
	}
	return 0;
}


const mray::GUID* CPropertieDictionary::getPropertyType(const core::string&name){
	IProperty *p=getPropertie(name);
	if(p==0)return 0;
	return &p->getPropertyType();
}

int CPropertieDictionary::getPropertiesCount(){
	return m_properties.size();
}

const TPropertieList& CPropertieDictionary::getProperties(){
	return m_properties;
}
const TPropertieGroupList& CPropertieDictionary::getGroups()
{
	return m_groups;
}
IProperty*CPropertieDictionary::getPropertie(const core::string&name){
	TPropertieListIT it;

	core::string s(name);
	s.make_lower();
	it=m_properties.find(s);

	if(it==m_properties.end())return 0;
	return it->second;
}


core::string CPropertieDictionary::getPropertieInfo(const core::string&name){
	IProperty*p=getPropertie(name);
	if(!p)return mT("");
	return p->getInfo();
}
void CPropertieDictionary::setPropertieInfo(const core::string&name,const  core::string&info){
	IProperty*p=getPropertie(name);
	if(!p)return ;
	p->setInfo(info);
}


void CPropertieDictionary::OnVisit(IPropertyVistor*v)
{
	v->Visit(this);
}



CPropertieSet::PropertyDictionaryMap CPropertieSet::m_dictionaries;

CPropertieSet::CPropertieSet()
:m_dictionary(0)
{
}
CPropertieSet::~CPropertieSet()
{
}
void CPropertieSet::_OnEventFired(ObjectEvent* event,IObject* p1,PVOID p2)
{
	EventDelegateMap::iterator it= m_eventDelegMap.find(event->getName());
	if(it==m_eventDelegMap.end())
		return;
	std::list<core::string>& events=it->second;
	std::list<core::string>::iterator eit=events.begin();
	for (;eit!=events.end();++eit)
	{
		EventContainerList::iterator it2= m_eventContainer.begin();
		EventContainerList::iterator e= m_eventContainer.end();
		for (;it2!=e;++it2)
		{
			(*it2)->CallDelegate(*eit,p1,p2);
		}
	}


}

bool CPropertieSet::CreateDictionary(const core::string&name,const RTTI*obj,CPropertieDictionary**dic)
{
	PropertyDictionaryMap::iterator it=m_dictionaries.find(name);
	if(it==m_dictionaries.end())
	{
		GCPtr<CPropertieDictionary> res=new CPropertieDictionary(obj);
		m_dictionaries[name]=res;
		*dic=res.pointer();
		return true;
	}else
	{
		*dic=it->second.pointer();
		return false;
	}
}
CPropertieDictionary* CPropertieSet::GetDictionary(const core::string&name)
{
	PropertyDictionaryMap::iterator it=m_dictionaries.find(name);
	if(it==m_dictionaries.end())return 0;
	return it->second.pointer();
}


bool CPropertieSet::CreateDictionary(CPropertieDictionary**dic)
{
	bool r=CreateDictionary(this->getObjectType(),this->getObjectRTTI(),dic);
	if(m_dictionary && r)
	{
		const TPropertieGroupList& gprops=m_dictionary->getGroups();
		TPropertieGroupList::const_iterator git=gprops.begin();
		for(;git!=gprops.end();++git)
		{
			TPropertieList::const_iterator it=git->second.properties.begin();
			for(;it!=git->second.properties.end();++it)
				(*dic)->addPropertie(it->second,git->second.name);
		}
	}
	m_dictionary=*dic;
	return r;
}
CPropertieDictionary* CPropertieSet::GetDictionary()
{
	if(!m_dictionary)
		m_dictionary=GetDictionary(this->getObjectType());
	return m_dictionary;
}
core::string CPropertieSet::GetPropertyValue(const core::string&name)
{
	if(!GetDictionary())
		return core::string::Empty;
	IProperty* p= m_dictionary->getPropertie(name);
	if(!p)
	{
		gGenericLoggerSystem.log(mT("Property Named: \"")+name+mT("\" Not Found"),ELL_WARNING,EVL_Normal);
		return core::string::Empty;
	}
	return p->toString(this);
}
void CPropertieSet::SetPropertyValue(const core::string&name,const core::string&v)
{
	if(!GetDictionary())
		return ;
	IProperty* p= m_dictionary-> getPropertie(name);
	if(!p)
	{
		gGenericLoggerSystem.log(mT("Property Named: \"")+name+mT("\" Not Found"),ELL_WARNING,EVL_Normal);
	}else
	{
		p->parse(this,v);
	}
}
void CPropertieSet::AddSubSet(const core::string&name,CPropertieSet* s)
{
	m_subSets[name]=s;
	if(GetDictionary() && s)
	{
		GetDictionary()->addSubDictionary(name,s->GetDictionary());
	}
}
CPropertieSet* CPropertieSet::GetSubSet(const core::string&name)
{
	SubsetMap::iterator it=m_subSets.find(name);
	if(it==m_subSets.end())return 0;
	return it->second;
}
const CPropertieSet::SubsetMap& CPropertieSet::GetSubSets()
{
	return m_subSets;
}


void CPropertieSet::AddEvent(ObjectEvent*e)
{
	core::string s1=e->getName();
	s1.make_lower();
	m_events[s1]=e;
}
int CPropertieSet::GetEventsCount()
{
	return m_events.size();
}
const TEventsList& CPropertieSet::GetEvents()
{
	return m_events;
}
ObjectEvent*CPropertieSet::GetEvent(const core::string&name)
{
	TEventsListIT it;

	core::string s(name);
	s.make_lower();
	it=m_events.find(s);

	if(it==m_events.end())return 0;
	return it->second;
}

void CPropertieSet::FireEvent(const core::string& name,IObject* p1,PVOID p2)
{
	ObjectEvent* e= GetEvent(name);
	if(!e)
		return;
	_OnEventFired(e,p1,p2);
}
void CPropertieSet::AddEventContainer(IDelegateContainer* d)
{
	if(!d)
		return;
	m_eventContainer.push_back(d);
}
void CPropertieSet::RemoveEventContainer(IDelegateContainer* listener)
{
	m_eventContainer.remove(listener);
}
void CPropertieSet::ClearEventContainers()
{
	m_eventContainer.clear();
}
void CPropertieSet::AddEventDelegatePair(const core::string& event,const core::string& target)
{
	EventDelegateMap::iterator it= m_eventDelegMap.begin();
	if(it==m_eventDelegMap.end())
	{
		m_eventDelegMap[event].push_back(target);
		return;
	}
	std::list<core::string>::iterator it2=it->second.begin();
	for (;it2!=it->second.end();++it2)
	{
		if(*it2==target)
			return;
	}
	it->second.push_back(target);
}
void CPropertieSet::RemoveEventDelegatePair(const core::string& event,const core::string& target)
{
	EventDelegateMap::iterator it= m_eventDelegMap.begin();
	if(it==m_eventDelegMap.end())
		return;
	std::list<core::string>::iterator it2=it->second.begin();
	for (;it2!=it->second.end();++it2)
	{
		if(*it2==target)
		{
			it->second.erase(it2);
			break;
		}
	}

}
void CPropertieSet::ClearDelegatesForEvent(const core::string& event)
{
	m_eventDelegMap.clear();
}
void CPropertieSet::loadFromXML(xml::XMLElement* elem)
{
	if(!elem)
		return;
	if(GetDictionary())
	{
		xml::XMLAttribute *attr;
		for(int i=0;i<elem->getAttributesCount();++i){
			attr=elem->getAttribute(i);
			IProperty*p= m_dictionary->getPropertie(attr->name);
			if(p /*&& p->getFlag(EPF_Write)*/){
				p->parse(this,attr->value);
			}
			if(!p){
				ObjectEvent*e= GetEvent(attr->name);
				if(e){
					ObjectDelegate*d= DelegateManager::getInstance().getDelegate(attr->value);
					if(d)
						(*e)+=d;
				}
			}
		}
	}
	xml::XMLElement* e= elem->getSubElement(mT("Events"));
	if(e)
	{
		core::string evnt,delg;
		xml::XMLAttribute*a;
		e=e->getSubElement(mT("Event"));
		while(e)
		{
			a=e->getAttribute(mT("Name"));
			if(!a)
				continue;
			evnt=a->value;
			a=e->getAttribute(mT("Delegate"));
			if(!a)
				continue;
			delg=a->value;
			AddEventDelegatePair(evnt,delg);
			e=e->nextSiblingElement(mT("Event"));
		}
	}
	
}

void  CPropertieSet::exportToXML(xml::XMLElement* elem){
	
	{
		const TPropertieList& props= m_dictionary->getProperties();
		TPropertieList::const_iterator it=props.begin();

		for(;it!=props.end();++it){
			IProperty* p=it->second;
			if(p->getFlag(EPF_Serialize) && p->getFlag(EPF_Read))
			{
				if(!p->isDefault(this))
					elem->addAttribute(p->getName(),p->toString(this));
			}
		}
	}
	{
		TEventsListIT it=m_events.begin();

		for(;it!=m_events.end();++it){
			if(it->second->getDelegates().begin()!=it->second->getDelegates().end())
				elem->addAttribute(it->second->getName(),(*it->second->getDelegates().begin())->getName());
		}
	}
	{
		SubsetMap::iterator it=m_subSets.begin();
		for(;it!=m_subSets.end();++it)
		{
			if(it->second)
			{
				xml::XMLElement* e=new xml::XMLElement(it->first);
				it->second->exportToXML(e);
				elem->addSubElement(e);
			}
		}
	}
}

void CPropertieSet::OnVisit(IPropertyVistor*visitor)
{
	visitor->Visit(this);
}

}
