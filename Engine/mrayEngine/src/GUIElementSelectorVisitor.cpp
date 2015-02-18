#include "stdafx.h"

#include "GUIElementSelectorVisitor.h"
#include "IGUIElement.h"
#include "IGUIPanelElement.h"



namespace mray
{
namespace GUI
{

GUIElementSelectorVisitor::GUIElementSelectorVisitor()
{
	m_result=0;
}
GUIElementSelectorVisitor::~GUIElementSelectorVisitor()
{
}

IGUIElement* GUIElementSelectorVisitor::Visit(IGUIElement*e,const math::vector2d& p)
{
	m_pos=p;
	m_result=0;
	e->OnVisit(this);
	return m_result;
}

void GUIElementSelectorVisitor::OnVisitComponent(IGUIComponent*comp)
{
}

void GUIElementSelectorVisitor::OnVisitElement(IGUIElement* elem)
{
	if(elem->IsPointInside(m_pos))
		m_result=elem;
}
void GUIElementSelectorVisitor::OnVisitPanel(IGUIPanelElement* elem)
{
	const IGUIPanelElement::ElementList& lst=elem->GetElements();
	IGUIPanelElement::ElementList::const_iterator it=lst.begin();
	for(;it!=lst.end();++it)
	{
		(*it)->OnVisit(this);
	}
}


}
}

