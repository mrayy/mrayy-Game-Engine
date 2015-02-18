
#include "stdafx.h"
#include "GUICheckBox.h"
#include "GUIThemeManager.h"
#include "IGUITheme.h"
#include "GUIManager.h"
#include "KeyCode.h"
#include "Event.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"

#include "GUICheckBoxComponent.h"


namespace mray{
namespace GUI{


	IMPLEMENT_ELEMENT_FACTORY(GUICheckBox);

GUICheckBox::GUICheckBox(IGUIManager* creator)
	:IGUICheckBox(creator)
{
	m_component=new GUICheckBoxComponent();
	m_component->owner=this;

	SetText(core::string(mT("CheckBox")));

	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeText::instance);
	}
}

GUICheckBox::~GUICheckBox()
{
	delete m_component;
}

void GUICheckBox::ChangeState()
{
	m_component->ChangeState();
}

const bool& GUICheckBox::IsChecked() { return m_component->IsChecked(); }

bool GUICheckBox::SetChecked(const bool& c)
{
	bool tmp=c;
	OnChangeCheck(this,&tmp);
	return m_component->SetChecked(c);
}

const bool& GUICheckBox::IsBackgroundOpaque() 
{ 
	return m_component->IsBackgroundOpaque();
}
bool GUICheckBox::SetBGOpaque(const bool& val)
{  
	return m_component->SetBGOpaque(val);
}

const bool& GUICheckBox::IsPushButton() 
{ 
	return m_component->IsPushButton();
}
bool GUICheckBox::SetPushButton(const bool&val) 
{
	return m_component->SetPushButton(val);
}

bool GUICheckBox::_HandleMouseEvent(MouseEvent*e)
{
	return m_component->OnMouseEvent(e);
}
bool GUICheckBox::_HandleKeyboardEvent(KeyboardEvent*e)
{
	IGUIManager* creator=GetCreator();
	if(!creator->HasFocus(this))
		return false;
	return m_component->OnKeyboardEvent(e);
}


bool GUICheckBox::OnEvent(Event* e)
{
	if(!IsEnabled() || !IsVisible())
		return false;
	if (e->getType()==ET_Mouse)
	{
		return _HandleMouseEvent(dynamic_cast<MouseEvent*>(e));
	}else if(e->getType()==ET_Keyboard)
	{
		return _HandleKeyboardEvent(dynamic_cast<KeyboardEvent*>(e));
	}
	return false;
/*

	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf btnRect=getRect();
	switch(e->getType())
	{
	case ET_Keyboard:
		{
			GCPtr<KeyboardEvent> event=e;
		if(guiMngr->hasFocus(this) && event->key==KEY_SPACE)
		{
			if(Enable){
				if(event->press){
					Checked=!Checked();
					m_state=Checked() ? EC_Checked : EC_UnChecked;
				}
			}
			ok=1;
		}
		}
		break;
	case ET_Mouse:
		{
			GCPtr<MouseEvent> event=e;
		if(btnRect.IsPointInside(math::vector2d(event->x,event->y))&&
			!event->isRecived())
		{
			if(event->event==MET_MOVED)
			{
				if(!guiMngr->hasMouseOn(this))
				{
					EventType=GET_Mouse_MOVE;
					guiMngr->setMouseOn(this);
					showHint(1);
					setHintPos(math::vector2d(event->x,event->y));
					if(!Checked())
						m_state=EC_MouseOn;
				}
				ok=1;
			}else
			if(event->event==MET_LEFTDOWN)
			{
				if(Enable){
				Checked=!Checked();
				EventType=GET_Mouse_DOWN;
				guiMngr->setFocus(this);
				guiMngr->setMouseOn(this);
				m_state=EC_MouseDown;
				}
				ok=1;
			}else
			if(event->event==MET_LEFTUP)
			{
				if(Enable){
					EventType=GET_Mouse_UP;
					m_state=Checked() ? EC_Checked : EC_UnChecked;
				}
				ok=1;
			}
		}else{
			m_state=Checked() ? EC_Checked : EC_UnChecked;
			showHint(0);
		}
		}break;
	};
	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->event=EventType;

		guiMngr->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}else{
	//	m_state=Checked() ? EC_Checked : EC_UnChecked;
	}*/
}


void GUICheckBox::Draw(const math::rectf*vp)
{
	if(!IsVisible())return;

	m_component->rc=GetDefaultRegion()->GetClippedRect();
	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	GetCreator()->GetDevice()->setScissorRect(m_component->rc);
	m_component->Draw();
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);
/*
	math::rectf tChBox=tRect;
	tChBox.BRPoint=tChBox.ULPoint+skin->getRect(0,math::rectf(0,0,24,24),mT("CheckBox")).getSize();


	tRect.ULPoint.x+=tChBox.getWidth();
	float yCoord=0;

	int state;
	int butState;
	switch (m_state){
		case EC_UnChecked:
			yCoord=0.0f;
			state=0;
			butState=0;
			break;
		case EC_Checked:
			yCoord=0.25f;
			state=1;
			butState=2;
			break;
		case EC_MouseOn:
			yCoord=0.5f;
			state=2;
			butState=1;
			break;
		case EC_MouseDown:
			yCoord=0.75f;
			state=3;
			butState=2;
			break;
	}

	tRect.clipRect(clipRect);
	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	math::rectf coordsRect(0,yCoord,1,yCoord+0.25f);
	
	video::SColor clr;
	if(!PushButton()){
		if(BGOpaque() && !bkTexture->GetTexture())
			skin->drawBox(getManager()->GetRenderQueue(),tRect,bclr,bclr*0.5,&clipRect);
		
		if(bkTexture->GetTexture())
			device->draw2DImage(tChBox,bkTexture,video::SColor(255,255,255,bclr.A),0,&coordsRect);
		else{
			if(Checked())
			{
				skin->drawBox(getManager()->GetRenderQueue(),tChBox,mT("CheckBox"),state,bclr,&(clipRect+1),true);
			}else
				skin->drawBox(getManager()->GetRenderQueue(),tChBox,mT("CheckBox"),state,bclr,&(clipRect+1),false);
		}

		if(tChBox.isValid()){
			tChBox.ULPoint+=2;
			tChBox.BRPoint-=2;
		}
			
	}else
	{
		if(Checked()){
			clr=Color();
			clr.A=AlphaEffect ? Alpha:bclr.A;
			skin->drawSizableBox(getManager()->GetRenderQueue(),tRect,butState,mT("PushButton"),bclr,&(clipRect+1));
		}else
		{
			clr=Color();
			clr.A=AlphaEffect ? Alpha:bclr.A;
			skin->drawSizableBox(getManager()->GetRenderQueue(),tRect,butState,mT("PushButton"),bclr,&clipRect);
		}
	}


	if(guiMngr->hasFocus(this))
	{
		math::rectf rc=tRect;
		rc.BRPoint-=4;
		rc.ULPoint+=4;
		guiMngr->getDevice()->draw2DRectangle(rc,80,0,&clipRect);
	}
	if(ShowCaption){
		IFont*font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);
		if(!font)
			return;
		video::SColor clr,oldClr;
		clr=oldClr=m_FontAttributes.fontColor;
		clr.A=AlphaEffect ? Alpha:oldClr.A;
		m_FontAttributes.fontColor=clr;
		if(Enable)
			font->print(tRect,&m_FontAttributes,&clipRect,getRenderString(),guiMngr->GetRenderQueue());
		else{
			m_FontAttributes.fontColor=video::SColor(128,128,128,clr.A);
			font->print(tRect,&m_FontAttributes,&clipRect,getRenderString(),guiMngr->GetRenderQueue());
		}
		m_FontAttributes.fontColor=oldClr;

	}
		
	GUIElement::draw(dt);*/
}






IGUIElement* GUICheckBox::Duplicate()
{
	GUICheckBox*dup=new GUICheckBox(m_creator);
	dup->CopyProperties(this);
	dup->m_FontAttributes=m_FontAttributes;
	return dup;
}
FontAttributes* GUICheckBox::GetFontAttributes(){
	return &m_FontAttributes;
}


//////////////////////////////////////////////////////////////////////////



}
}
