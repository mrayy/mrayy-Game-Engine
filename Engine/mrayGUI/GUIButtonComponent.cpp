
#include "Stdafx.h"
#include "GUIButtonComponent.h"
#include "IGUIElement.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "IGUIManager.h"
#include "IGUITheme.h"

namespace mray
{
namespace GUI
{

GUIButtonComponent::GUIButtonComponent():m_isPressed(false),m_mouseOver(false)
{
	text=core::string(mT("Button"));
}

GUIButtonComponent::~GUIButtonComponent()
{
}

bool GUIButtonComponent::OnMouseEvent(MouseEvent* e)
{
	IGUIManager* creator=owner->GetCreator();
	if(!rc.IsPointInside(e->pos))
	{
		if(m_mouseOver)
		{
			creator->SetMouseOn(0);
			m_mouseOver=false;
			m_isPressed=false;
		}
		return false;
	}

	// set the creator's mouse on element to be this
	creator->SetMouseOn(owner);
	m_mouseOver=true;

	switch (e->event)
	{
	case MET_LEFTDOWN:
		m_isPressed=true;
		creator->SetFocus(owner);
		//owner->OnMouseDown(m_ownerElement,&e->pos);
	

		break;
	case MET_LEFTUP:
	//	m_ownerElement->OnMouseUp(m_ownerElement,&e->pos);
		if(m_isPressed)
		{
			m_isPressed=false;
		}
		break;
	case MET_MOVED:
		//owner->OnMouseMove(m_ownerElement,&e->pos);
		break;
	}
	e->setRecived();

	return true;
}
bool GUIButtonComponent::OnKeyboardEvent(KeyboardEvent* e)
{
/*	IGUIManager* creator=m_ownerElement->GetCreator();
	if(!creator->HasFocus(m_ownerElement))
		return false;
*/


	IGUIManager* creator=owner->GetCreator();
	if(e->key==KEY_SPACE)
	{
		if(e->press)
		{
			m_isPressed=true;
// 			if(m_listener)
// 				m_listener->OnPressed();
		}else
		{
			m_isPressed=false;
		}
		e->setRecived();
		return true;
	}
	return false;
}
void GUIButtonComponent::Draw()
{
	IGUIManager* creator=owner->GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	if(!skin)
		return;
	int state=0;
	if(!m_isPressed)
	{
		if(!m_mouseOver)
			state=0;
		else state=1;
	}else
		state=2;

	skin->drawSizableBox(creator->GetRenderQueue(),rc,state,mT("Button"));
	FontAttributes*attrs= owner->GetFontAttributes();
	if(attrs)
	{
		IFont* f= gFontResourceManager.getFontByName(attrs->fontName);
		if(f)
		{
			f->print(rc,attrs,0,text,creator->GetRenderQueue());
		}
	}
}
/*
void GUIButtonComponent::SetText(const core::UTFString&t)
{
	m_renderText=t;
}*/
void GUIButtonComponent::LostMouseOn()
{
	m_mouseOver=false;
}
void GUIButtonComponent::LostFocus()
{
	m_mouseOver=false;
}


}
}