
#include "stdafx.h"
#include "GUIXMLElementVisitor.h"
#include "XMLElement.h"
#include "IGUIPanelElement.h"



namespace mray
{
namespace GUI
{

GUIXMLElementVisitor::GUIXMLElementVisitor()
{
	m_currElement=0;
}
GUIXMLElementVisitor::~GUIXMLElementVisitor()
{
}

xml::XMLElement* GUIXMLElementVisitor::WriteNode(IGUIElement* elem)
{
	xml::XMLElement*e= m_serializer.Serialize(elem);
	core::string name;
	core::char_to_string(elem->GetType().GetString().c_str(),name);
	e->setName(name);
	return e;
}
xml::XMLElement* GUIXMLElementVisitor::Visit(IGUIElement*e,const core::string&name)
{
	m_currElement=new xml::XMLElement(mT("Overlay"));
	m_currElement->addAttribute(mT("Name"),name);
	e->OnVisit(this);
	return m_currElement;
}
void GUIXMLElementVisitor::OnVisitComponent(IGUIComponent*comp)
{
}

void GUIXMLElementVisitor::OnVisitElement(IGUIElement* elem)
{
	xml::XMLElement* e=WriteNode(elem);
	m_currElement->addSubElement(e);
}
void GUIXMLElementVisitor::OnVisitPanel(IGUIPanelElement* elem)
{
	const IGUIPanelElement::ElementList& lst=elem->GetElements();
	xml::XMLElement* e1=WriteNode((IGUIElement*)elem);
	xml::XMLElement* tmp=m_currElement;
	m_currElement->addSubElement(e1);
	if(lst.size()>0)
	{
		xml::XMLElement*e=new xml::XMLElement(mT("Children"));
		e1->addSubElement(e);
		m_currElement=e;
		IGUIPanelElement::ElementList::const_iterator it=lst.begin();
		for(;it!=lst.end();++it)
		{
			(*it)->OnVisit(this);
		}
		m_currElement=tmp;
	}
}


}
}
