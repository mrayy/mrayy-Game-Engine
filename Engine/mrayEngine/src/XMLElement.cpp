#include "stdafx.h"

#include "XMLElement.h"
#include "StringConverter.h"
#include "XMLTextNode.h"



namespace mray{
namespace xml{


XMLElement::XMLElement(const core::string&name)
		:m_prev(0),m_next(0),m_lastElem(0)
{
	setName(name);
	//m_name.make_lower();
	m_parent=0;
}
XMLElement::~XMLElement()
{
	Clear();
}

void XMLElement::Clear()
{
	xmlSubElementsMapIT it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		delete *it;
	}
	m_elements.clear();
	m_Attributes.clear();
	m_lastElem=0;
	m_prev=0;
}
void XMLElement::setName(const core::string&n)
{
	m_name = n;
	m_name.replaceChar('&', '_');
	m_name.replaceChar('<', '_');
	m_name.replaceChar('>', '_');
}
const core::string& XMLElement::getName()const{
	return m_name;
}


ENodeType XMLElement::GetType()const
{
	return ENT_Element;
}

int XMLElement::getSubElementsCount(){
	return m_elements.size();
}
void XMLElement::addSubElement(IXMLNode*elem)
{
	if(!elem)
		return;
	if(elem->GetType()==ENT_Element)
	{
		XMLElement*e=dynamic_cast<XMLElement*>(elem);
		if(m_lastElem)
		{
			m_lastElem->m_next=e;
			e->m_prev=m_lastElem;
		}
		m_lastElem=e;
	}
	m_elements.push_back(elem);
	elem->m_parent=this;
}
void XMLElement::removeSubElement(const core::string&name)
{
	core::string str=name;
	//str.make_lower();
	xmlSubElementsMapIT it= m_elements.begin();
	for(;it!=m_elements.end();++it)
	{
		IXMLNode *n=*it;
		if(n->GetType()!=ENT_Element)continue;
		XMLElement*e=dynamic_cast<XMLElement*>(n);
		if(e->getName().equals_ignore_case(str))
		{
			if(m_lastElem==e)
				m_lastElem=m_lastElem->m_prev;

			if(e->m_next)
				e->m_next->m_prev=e->m_prev;
			if(e->m_prev)
				e->m_prev->m_next=e->m_next;
			e->m_parent=0;
			m_elements.erase(it);
			return;
		}
	}
}
XMLElement*XMLElement::getSubElement(const core::string&name){
	core::string str=name;
	str.make_lower();
	xmlSubElementsMapIT it= m_elements.begin();
	for(;it!=m_elements.end();++it)
	{
		IXMLNode *n=*it;
		if(n->GetType()!=ENT_Element)continue;
		XMLElement*e=dynamic_cast<XMLElement*>(n);

		if(e->getName().equals_ignore_case(str)){
			return e;
		}
	}
	return 0;
}
XMLElement* XMLElement::nextSiblingElement(const core::string&name)
{
	XMLElement* e=m_next;
	while(e)
	{
		if(e->getName()==name)
			return e;
		e=e->m_next;
	}
	return 0;
}


int XMLElement::getAttributesCount(){
	return m_Attributes.size();
}
void XMLElement::addAttribute(const core::string&name,const core::string&value){
	core::string str=name;
	str.make_lower();
	str.replaceChar('&', '_');
	str.replaceChar('<', '_');
	str.replaceChar('>', '_');
	m_Attributes.insert(xmlAttributesMap::value_type(str,XMLAttribute(name,value)));
}
void XMLElement::removeAttribute(const core::string&name){
	core::string str=name;
	str.make_lower();
	m_Attributes.erase(str);
}


XMLAttribute* XMLElement::getAttribute(int index){
	xmlAttributesMapIT it=m_Attributes.begin();
	std::advance(it,index);
	if(it!=m_Attributes.end()){
		return &it->second;
	}
	return 0;
}
XMLAttribute*XMLElement::getAttribute(const core::string&name){
	core::string str=name;
	str.make_lower();
	xmlAttributesMapIT it=m_Attributes.find(str);
	if(it!=m_Attributes.end()){
		return &it->second;
	}
	return 0;
}

const  core::string& XMLElement::getValueString(const core::string&name){
	core::string str=name;
	str.make_lower();
	xmlAttributesMapIT it=m_Attributes.find(str);
	if(it!=m_Attributes.end()){
		return it->second.value;
	}
	return core::string::Empty
		;
}
int XMLElement::getValueInt(const core::string&name){
	core::string str=name;
	str.make_lower();
	return core::StringConverter::toInt(getValueString(str));
}
float XMLElement::getValueFloat(const core::string&name){
	core::string str=name;
	str.make_lower();
	return core::StringConverter::toFloat(getValueString(str));
}
bool XMLElement::getValueBool(const core::string&name){
	core::string str=name;
	str.make_lower();
	return core::StringConverter::toBool(getValueString(str));
}


xmlAttributesMapIT XMLElement::getAttributesBegin(){
	return m_Attributes.begin();
}
xmlAttributesMapIT XMLElement::getAttributesEnd(){
	return m_Attributes.end();
}

xmlSubElementsMapIT XMLElement::getElementsBegin(){
	return m_elements.begin();
}
xmlSubElementsMapIT XMLElement::getElementsEnd(){
	return m_elements.end();
}

xml::XMLTextNode* XMLElement::GetTextNode()
{

	xmlSubElementsMapIT it= m_elements.begin();
	for(;it!=m_elements.end();++it)
	{
		IXMLNode *n=*it;
		if(n->GetType()!=ENT_Text)continue;
		return dynamic_cast<XMLTextNode*>(n);
	}

	return 0;
}


}
}

