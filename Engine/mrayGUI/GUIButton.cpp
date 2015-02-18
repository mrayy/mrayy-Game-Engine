
#include "Stdafx.h"

#include "GUIButton.h"
#include "IGUITheme.h"

#include "GUIButtonComponent.h"
#include "IGUIManager.h"

namespace mray
{
namespace GUI
{


	IMPLEMENT_ELEMENT_FACTORY(GUIButton);

GUIButton::GUIButton(IGUIManager* creator)
:IGUIButton(creator)
{
	fillProperties();

	m_component=new GUIButtonComponent();
	m_component->owner=this;
}

void GUIButton::fillProperties()
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
	}
}

GUIButton::~GUIButton()
{
	delete m_component;
}


bool GUIButton::SetText(const core::UTFString&t)
{
	m_component->text=t;
	return true;
}
const core::UTFString& GUIButton::GetText()const
{
	return m_component->text;
}
	

void GUIButton::Draw(const math::rectf*vp)
{
	if(!IsVisible())return;
	m_component->rc=GetDefaultRegion()->GetClippedRect();
	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	GetCreator()->GetDevice()->setScissorRect(m_component->rc);
	m_component->Draw();
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);

/*

	if(guiMngr && guiMngr->hasFocus(this))
	{
		if(!bkTexture){
			math::rectf rc=tRect;
			rc.BRPoint-=4;
			rc.ULPoint+=4;
			if(pressed){
				rc.BRPoint+=1;
				rc.ULPoint+=1;
			}
			guiMngr->getDevice()->draw2DRectangle(rc,80,0,&clipRect);
		}
	}
	if(ShowCaption){
		IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
		if(!font)
			return;

		video::SColor clr,oldClr;
		clr=oldClr=m_FontAttributs.fontColor;
		clr.A=AlphaEffect ? Alpha:clr.A;
		if(!Enable)
			clr=video::SColor(128,128,128,clr.A);
		float charHeight=font->getCharDimension('A',m_FontAttributs.fontSize).y;
		tRect.ULPoint.y += (tRect.getHeight()-charHeight)*0.5f;
		m_FontAttributs.fontColor=clr;
		if(pressed==0)
			font->print(tRect,&m_FontAttributs,&clipRect,getRenderString(),guiMngr->GetRenderQueue());
		else
			font->print(tRect+1,&m_FontAttributs,&(clipRect+1),getRenderString(),guiMngr->GetRenderQueue());
		m_FontAttributs.fontColor=oldClr;
		
	}
	GUIElement::draw(dt);*/
}



bool GUIButton::_OnKeyboardEvent(KeyboardEvent* event)
{
	IGUIManager* creator=GetCreator();
	if(!creator->HasFocus(this))
		return false;
	m_component->rc=GetDefaultRegion()->GetClippedRect();

	if(m_component->OnKeyboardEvent(event))
	{
		if(event->press)
		{
			OnKeyDown(this,event);
		}
		return true;
	}
	return false;
}
bool GUIButton::_OnMouseEvent(MouseEvent* e)
{
	math::rectf rc=GetDefaultRegion()->GetClippedRect();

	IGUIManager* creator=GetCreator();
	if(!rc.IsPointInside(e->pos))
	{
		if(m_component->m_mouseOver)
		{
			creator->SetMouseOn(0);
			m_component->m_mouseOver=false;
			m_component->m_isPressed=false;
		}
		return false;
	}

	// set the creator's mouse on element to be this
	creator->SetMouseOn(this);
	m_component->m_mouseOver=true;

	switch (e->event)
	{
	case MET_LEFTDOWN:
		m_component->m_isPressed=true;
		creator->SetFocus(this);
		OnMouseDown(this,&e->pos);
		_OnEventFired(&OnMouseDown,this,&e->pos);

		OnClick(this,&e->pos);
		_OnEventFired(&OnClick,this,&e->pos);

		break;
	case MET_LEFTUP:
		OnMouseUp(this,&e->pos);
		_OnEventFired(&OnMouseUp,this,&e->pos);
		if(m_component->m_isPressed)
		{
			m_component->m_isPressed=false;
		}
		break;
	case MET_MOVED:
		OnMouseMove(this,&e->pos);
		_OnEventFired(&OnMouseMove,this,&e->pos);
		break;
	}
	e->setRecived();

	return true;

}/*
bool GUIButton::OnEvent(Event*  event)
{
	
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf btnRect=getRect();
	btnRect.clipRect(clipRect);

	switch(e->getType())
	{
	case ET_Keyboard:
		{

		GCPtr<KeyboardEvent> event=e;
		if(guiMngr->hasFocus(this) &&
			(event->key==KEY_RETURN ||
			event->key==KEY_SPACE))
		{
			if(Enable){
				pressed=event->press;
				if(pressed){
					EventType=GET_Mouse_DOWN;
					OnKeyDown(this,&event->key);
				}
				else EventType=GET_Mouse_UP;
				ok=1;
			}
		}
		if(event->press==0 && Enable)
			pressed=0;
		m_state=pressed ? Pressed : Unpressed;
		}
		break;
	case ET_Mouse:
		{

			GCPtr<MouseEvent> event=e;
		if(btnRect.IsPointInside(math::vector2d(event->x,event->y))&&
			!event->isRecived())
		{
			gCursorControl.setCursor(EC_Hand);
			if(event->event==MET_MOVED)
			{
				if(!guiMngr->hasMouseOn(this))
				{
					EventType=GET_Mouse_MOVE;
					guiMngr->setMouseOn(this);
					setHintPos(math::vector2d(event->x,event->y));
					showHint(1);
				}
				math::vector2d t(event->x,event->y);
				OnMouseMove(this,&t);
				if(!pressed)
					m_state=MouseOn;
				ok=1;
			}else
			if(event->event==MET_LEFTDOWN)
			{
				if(Enable){
					pressed=1;
					EventType=GET_Mouse_DOWN;
					guiMngr->setFocus(this);
					m_state=Pressed;
					controllers::EMouseButton b= controllers::EMB_Left;
					OnClick(this,&b);
				}
				ok=1;
			}
			else if(event->event==MET_LEFTUP && pressed)
			{
				pressed=0;
				EventType=GET_Mouse_UP;
				m_state=MouseOn;
				ok=1;
			}
		}else
		{
			m_SndPlayMO=0;
			if(event->event==MET_LEFTUP)
				m_SndPlayMD=0;

			showHint(0);
			if(event->event==MET_LEFTDOWN || 
				event->event==MET_LEFTUP)
			{
				pressed=0;
				m_state=Unpressed;
			}else
				m_state=pressed ? Pressed : Unpressed;
		}
		}break;
	};


	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->event=EventType;
		switch(EventType)
		{
		case GET_Mouse_DOWN:
			if(!m_SndPlayMD && m_sndMouseDown){
				m_sndMouseDown->play();
				m_SndPlayMD=true;
		case MET_LEFTUP:
			m_SndPlayMD=0;
			break;
		case MET_MOVED:
			if(!m_SndPlayMO && m_sndMouseOn){
				m_sndMouseOn->play();
				m_SndPlayMO=true;
			}
			break;
			}
		}

		guiMngr->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
}*/

/*
void GUIButton::SetText(const core::UTFString&t)
{
	IGUIElement::SetText(t);
	m_buttonComp->SetText(t);
}*/

IGUIElement* GUIButton::Duplicate()
{
	GUIButton*btn=new GUIButton(m_creator);
	btn->CopyProperties(this);
	btn->m_FontAttributs=m_FontAttributs;

	return btn;
}

	
FontAttributes* GUIButton::GetFontAttributes(){
	return &m_FontAttributs;
}

void GUIButton::_notifyMouseEnter()
{
	IGUIElement::_notifyMouseEnter();
}
void GUIButton::_notifyMouseLeave()
{
	IGUIElement::_notifyMouseLeave();
	m_component->LostMouseOn();
}

void GUIButton::_notifyReceiveFocus()
{
	IGUIElement::_notifyReceiveFocus();
}
void GUIButton::_notifyLostFocus()
{
	IGUIElement::_notifyLostFocus();
	m_component->LostFocus();
}

//////////////////////////////////////////////////////////////////////////


}
}

