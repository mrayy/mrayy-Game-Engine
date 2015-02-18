#include "stdafx.h"


#include "IGUIPanelElement.h"
#include "IGUIElementVisitor.h"
#include "XMLElement.h"
#include "IGUIManager.h"
#include "GUIElementRegion.h"

namespace mray
{
namespace GUI
{

IGUIPanelElement::IGUIPanelElement(const GUID& type,IGUIManager* creator):
IGUIElement(type,creator)
{
}
IGUIPanelElement::~IGUIPanelElement()
{
	ClearElements();
}

bool IGUIPanelElement::IsPanelElement()
{
	return true;
}

void IGUIPanelElement::_UpdateDerivedAlpha()
{
	IGUIElement::_UpdateDerivedAlpha();
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->_UpdateDerivedAlpha();
	}
}


void IGUIPanelElement::AddElement(IGUIElementCRef elem,GUIElementRegion* r)
{
	if(elem->GetParent()==this)
		return;
	if(elem->GetParent())
	{
		elem->GetParent()->RemoveElement(elem);
	}
	m_elements.push_back(elem);
	if(r)
		elem->AttachToRegion(r);
	else
		elem->AttachToRegion(m_defaultRegion);
}
void IGUIPanelElement::AddElement(IGUIElementCRef elem)
{
	AddElement(elem,m_defaultRegion);	
}

void IGUIPanelElement::RemoveElement(IGUIElement* elem)
{
	if(elem->GetParent()!=this)
		return;

	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it).pointer()==elem)
		{
			m_elements.erase(it);
			break;
		}
	}
	elem->AttachToRegion(0);
}

IGUIElementCRef IGUIPanelElement::GetElementByID(uint id)const
{
	ElementList::const_iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it)->GetID()==id)
			return *it;
	}
	return IGUIElementPtr::Null;
}

void IGUIPanelElement::ClearElements()
{
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->AttachToRegion(0);
		//delete *it;
	}
	m_elements.clear();
}


bool IGUIPanelElement::IsChild(IGUIElement*elem,bool recursive)
{
	if(!elem)
		return false;
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it).pointer()==elem)
			return true;
		if(recursive && (*it)->IsPanelElement())
		{
			if(((IGUIPanelElement*)(*it).pointer())->IsChild(elem,true))
				return true;
		}
	}
	return false;
}

bool IGUIPanelElement::OnEvent(Event* e)
{
	if(!IsEnabled() || !IsVisible())return false;
	bool res=false;
	if(!m_elements.empty()){
		ElementList::iterator it=m_elements.end();
		for (--it;;--it)
		{
			res=(*it)->OnEvent(e);
			if(res)
				break;
			if(it==m_elements.begin())
				break;
		}
	}
	if(!res)
		res=IGUIElement::OnEvent(e);
	return res;
}

void IGUIPanelElement::Update(float dt)
{
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->Update(dt);
	}

}

void IGUIPanelElement::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;
	_UpdateRegion(vp);
	if(!GetDefaultRegion()->GetClippedRect().isValid())
		return;
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->Draw(vp);
	}
}
bool IGUIPanelElement::MoveElementToTop(IGUIElement* elem)
{
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it).pointer()==elem)
		{
			IGUIElementPtr e = *it;
			m_elements.erase(it);
			m_elements.push_back(e);
			return true;
		}
	}
	return false;
}
bool IGUIPanelElement::MoveElementToBottom(IGUIElement* elem)
{
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it).pointer()==elem)
		{
			IGUIElementPtr e = *it;
			m_elements.erase(it);
			m_elements.push_front(e);
			return true;
		}
	}
	return false;
}

void IGUIPanelElement::_RectsDirt()
{
	IGUIElement::_RectsDirt();
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->_RectsDirt();
	}
}

void IGUIPanelElement::OnVisit(IGUIElementVisitor*v)
{
	v->OnVisitPanel(this);
}

void IGUIPanelElement::LoadXMLSettings(xml::XMLElement*elem)
{
	xml::XMLElement* e= elem->getSubElement(mT("Children"));
	if(e)
	{
		xml::xmlSubElementsMapIT it= e->getElementsBegin();
		xml::xmlSubElementsMapIT end= e->getElementsEnd();
		for(;it!=end;++it)
		{
			if((*it)->GetType()!=xml::ENT_Element)
				continue;
			xml::XMLElement* elem=(xml::XMLElement*)*it;
			GUI::IGUIElement* guiElem= GetCreator()->CreateElement(core::string_to_char(elem->getName()));
			if(!guiElem)
				continue;
			guiElem->LoadXMLSettings(elem);
			AddElement(guiElem);
		}
	}
	IGUIElement::LoadXMLSettings(elem);

}

bool IGUIPanelElement::_OnMouseEvent(MouseEvent*evt)
{
	return false;
}
bool IGUIPanelElement::_OnKeyboardEvent(KeyboardEvent*evt)
{
	return false;
}

void IGUIPanelElement::OnMoved(const GUIElementEvent& e)
{
	IGUIElement::OnMoved(e);
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->OnMoved(e);
	}
}
void IGUIPanelElement::OnResized(const GUIElementEvent& e)
{
	IGUIElement::OnResized(e);
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->OnResized(e);
	}
}
void IGUIPanelElement::OnChangeVisible(const GUIElementEvent& e)
{
	IGUIElement::OnChangeVisible(e);
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->OnChangeVisible(e);
	}
}
void IGUIPanelElement::OnChangeEnable(const GUIElementEvent& e)
{
	IGUIElement::OnChangeEnable(e);
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->OnChangeEnable(e);
	}
}
void IGUIPanelElement::OnChangeParent(const GUIElementEvent& e)
{
	IGUIElement::OnChangeParent(e);
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->OnChangeParent(e);
	}
}
void IGUIPanelElement::OnSetCreator(const GUIElementEvent& e)
{
	IGUIElement::OnSetCreator(e);
	ElementList::iterator it=m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		(*it)->OnSetCreator(e);
	}
}

}
}
