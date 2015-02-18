#include "stdafx.h"


#include "GUIOverlay.h"
#include "GUILoggerSystem.h"
#include "IGUIElement.h"
#include "IGUIManager.h"
#include "XMLTextNode.h"
#include "IGUIPanelElement.h"
#include "IGUISchemeBase.h"

 

namespace mray{
namespace GUI{


GUIOverlay::GUIOverlay()
{
}
GUIOverlay::~GUIOverlay()
{
	xmlElementList::iterator it=m_elements.begin();
	for(;it!=m_elements.end();++it)
		delete *it;
}

void GUIOverlay::SetName(const core::string&name)
{
	m_name=name;
}
const core::string& GUIOverlay::GetName(){
	return m_name;
}
xml::IXMLNode* GUIOverlay::DuplicateElement(xml::IXMLNode* ee)
{
	xml::IXMLNode* ret=0;
	if(ee->GetType()==xml::ENT_Element)
	{
		xml::XMLElement* elem=(xml::XMLElement*)ee;
		xml::XMLElement* e=new xml::XMLElement(elem->getName());
		
		{
			xml::xmlAttributesMapIT it= elem->getAttributesBegin();
			for(;it!=elem->getAttributesEnd();++it)
				e->addAttribute((*it).second.name,(*it).second.value);
		}
		{
			xml::xmlSubElementsMapIT it= elem->getElementsBegin();
			for(;it!=elem->getElementsEnd();++it)
				e->addSubElement(DuplicateElement(*it));
		}
		ret=e;
	}else
	if(ee->GetType()==xml::ENT_Text)
	{
		xml::XMLTextNode* elem=(xml::XMLTextNode*)ee;
		xml::XMLTextNode* e=new xml::XMLTextNode(elem->GetValue());
		ret=e;
	}
	return ret;
}

void GUIOverlay::AddElement(xml::XMLElement* elem){
	xml::XMLElement* e=(xml::XMLElement*)DuplicateElement(elem);
	if(!e)
		return;
	m_elements.push_back(e);
}
void GUIOverlay::TraverseElements(IGUIElement* elems, IGUISchemeBase* scheme,IDelegateContainer* eventListener)
{
	scheme->_ElementDeclared(elems);
	if(eventListener)
		elems->AddEventContainer(eventListener);
	if(!elems->IsPanelElement())
		return;
	IGUIPanelElement* panel=(IGUIPanelElement* )elems;
	const GUI::IGUIPanelElement::ElementList& subElemes= panel->GetElements();
	for(IGUIPanelElement::ElementList::const_iterator it=subElemes.begin();it!=subElemes.end();++it)
	{
		TraverseElements(*it,scheme,eventListener);
	}
}

ElementList GUIOverlay::CreateElements(IGUIManager* manager,IGUIPanelElement* parent,IDelegateContainer* eventListener,IGUISchemeBase* scheme){

	ElementList m_tmpOutList;
	xmlElementList::iterator it=m_elements.begin();
	for(;it!=m_elements.end();++it){
		xml::XMLElement*xe=*it;
		IGUIElement* e=_CreateElement(xe,manager);
		if(!e)
			continue;
		if(parent)
			parent->AddElement(e);
		m_tmpOutList.push_back(e);
	}
	if(scheme)
	{
		for(ElementList::iterator it=m_tmpOutList.begin();it!=m_tmpOutList.end();++it)
			TraverseElements(*it,scheme,eventListener);
		scheme->DoneLoadingElements();
	}

	return m_tmpOutList;//core::IteratorPair<ElementList>(m_elements);
}


IGUIElement* GUIOverlay::_CreateElement(xml::XMLElement* attrs,IGUIManager* manager){
	IGUIElement* e=0;
	core::stringc guidStr;
	core::string_to_char(attrs->getName(),guidStr);
	e=manager->CreateElement(guidStr);
	if(!e){
		gGUILoggerSystem.log(mT("Element Type Not Valid: \"")+attrs->getName()+mT("\""),ELL_WARNING,EVL_Normal);
		return 0;
	}
	e->LoadXMLSettings(attrs);
	/*
	{
		xml::xmlSubElementsMapIT it= attrs->getElementsBegin();
		xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
		for (;it!=end;++it)
		{
			xml::XMLElement*elem=*it;
			if(elem->getName().equals_ignore_case(mT("Element"))){
				GCPtr<GUIElement> chld=createElement(elem);
				if(chld){
					e->addChild(chld);
				}
			}else if(elem->getName().equals_ignore_case(mT("effector"))){
				GCPtr<GUIEffector> eff=createEffector(elem);
				if(eff){
					e->addEffector(eff);
				}
			}
		}
	}*/
	return e;
}
/*
GCPtr<GUIEffector> GUIOverlay::createEffector(xml::XMLElement*attrs){

	GCPtr<GUIEffector> e;
	xml::XMLAttribute*a =attrs->getAttribute(mT("type"));
	if(!a){
		gLogManager.log(mT("Effector Type Not Found: \"")+core::string(m_name)+mT("\""),ELL_WARNING);
		return 0;
	}
	e=GUIEffectorFactory::getInstance().createEffector(a->value);
	if(!e){
		gLogManager.log(mT("Effector Type Not Valid: \"")+a->value+mT("\""),ELL_WARNING);
		return 0;
	}
	e->loadXMLSettings(attrs);
	return e;
}*/
}
}

