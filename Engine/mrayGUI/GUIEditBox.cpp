

#include "Stdafx.h"
#include "GUIEditBox.h"

#include "IGUIManager.h"
#include "IGUITheme.h"
#include "IOSClipboard.h"

#include "GUIEditBoxComponent.h"

#include <Regex/Pattern.h>

namespace mray{
namespace GUI{


	IMPLEMENT_ELEMENT_FACTORY(GUIEditBox);


GUIEditBox::GUIEditBox(IGUIManager*mngr):IGUIEditBox(mngr)
{
	m_component=new GUIEditBoxComponent();
	m_component->owner=this;

	m_component->m_HideText=PropertyTypeHideText::instance.GetDefaultValue();
	m_component->m_HideChar=PropertyTypeHideChar::instance.GetDefaultValue();
	m_component->m_background=PropertyTypeBackground::instance.GetDefaultValue();
	m_component->m_border=PropertyTypeBorder::instance.GetDefaultValue();
	SetText(PropertyTypeText::instance.GetDefaultValue());

	m_FontAttributes.fontAligment=EFA_MiddleLeft;

	m_component->listener = this;

	fillProperties();
}

GUIEditBox::~GUIEditBox()
{
	delete m_component;
}

const bool& GUIEditBox::getBackground() { return m_component->m_background; }
bool GUIEditBox::setBackground(const bool &val) {m_component-> m_background = val;  return true;}

const bool& GUIEditBox::getBorder() { return m_component->m_border; }
bool GUIEditBox::setBorder(const bool &val) { m_component->m_border = val; return true; }

const core::string& GUIEditBox::getHideChar() { return m_component->m_HideChar; }
bool GUIEditBox::setHideChar(const core::string &val) {  m_component->m_HideChar = val; return true;}

const bool& GUIEditBox::getHideText() { return m_component->m_HideText; }
bool GUIEditBox::setHideText(const bool &val) { m_component->m_HideText = val; return true;}

void GUIEditBox::fillProperties()
{
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{

		dic->addPropertie(&PropertyTypeRegularExpression::instance,mT("Behavior"));
		dic->addPropertie(&PropertyTypeText::instance,mT("Behavior"));

		dic->addPropertie(&PropertyTypeHideText::instance,mT("Apperance"));
		dic->addPropertie(&PropertyTypeHideChar::instance,mT("Apperance"));
		dic->addPropertie(&PropertyTypeBorder::instance,mT("Apperance"));
		dic->addPropertie(&PropertyTypeBackground::instance,mT("Apperance"));
	}
}

void GUIEditBox::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;

	m_component->rc=GetDefaultRegion()->GetClippedRect();
	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	//GetCreator()->GetDevice()->setScissorRect(m_component->rc);
	m_component->Draw();
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);
	IGUIElement::Draw(vp);
}

void GUIEditBox::Update(float dt)
{
	m_component->Update(dt);

	IGUIElement::Update(dt);

}

void GUIEditBox::OnTextChangeCallback(GUIEditBoxComponent*caller)
{
	OnTextChange(this, &m_component->text);
}



bool GUIEditBox::SetText(const core::UTFString& cap)
{
	if(!m_component->SetText(cap))
		return false;
	//m_CursorPos=m_text.Length();
	OnTextChange(this,&m_text);
	return true;
}
const core::UTFString& GUIEditBox::GetText()const 
{
	return m_component->text;
}
bool GUIEditBox::CheckIsValideText(const core::UTFString&t)
{
	return m_component->CheckIsValideText(t);
}
core::string GUIEditBox::getSelectedText()
{
	return m_component->getSelectedText();
}



IGUIElement* GUIEditBox::Duplicate()
{
	GUIEditBox*dup=new GUIEditBox(GetCreator());

	dup->SetText(GetText());

	return dup;
}


FontAttributes* GUIEditBox::GetFontAttributes()
{
	return &m_FontAttributes;
}
void GUIEditBox::_RemoveSelectedText()
{
	m_component->_RemoveSelectedText();
}

bool GUIEditBox::_OnKeyboardEvent(KeyboardEvent* event)
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
bool GUIEditBox::_OnMouseEvent(MouseEvent* event)
{
	m_component->rc=GetDefaultRegion()->GetClippedRect();

	return m_component->OnMouseEvent(event);
	/*
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme(); 	

	GUI_EVENT_TYPE EventType;
	bool ok=false;
	math::rectf rc=getRect();
	rc.clipRect(clipRect);
	rc=skin->getSizableRect(4,rc,0,mT("EditBox"));
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);
	if(!font)
		return false;
	switch(event->event)
	{
	case MET_LEFTDOWN:
		if(getRect().IsPointInside(math::vector2d(event->x,event->y)))
		{
			CursorPos=font->getCharFromPos(math::vector2d(event->x,event->y),&m_FontAttributes,clipRect,getRenderString())+startPos;
			if(m_Caption.Length()<=CursorPos)
				CursorPos=m_Caption.Length();
			MouseMark=true;
			MarkEnd=MarkBegin=CursorPos;
			guiMngr->setFocus(this);
			EventType=GET_Mouse_DOWN;
			ok=1;
		}
		break;
	case MET_LEFTUP:
		if(guiMngr->hasFocus(this))
		{
			if(CursorPos<MarkBegin)
			{
				MarkEnd=MarkBegin;
				MarkBegin=CursorPos;
			}
			EventType=GET_Mouse_UP;
			MouseMark=0;
			ok=1;
		}
		break;
	case MET_MOVED:
		if(guiMngr->hasFocus(this))
		{
			if(MouseMark){
				CursorPos=font->getCharFromPos(math::vector2d(event->x,event->y),&m_FontAttributes,clipRect,getRenderString())+startPos;
				if(m_Caption.Length()<CursorPos)
					CursorPos=m_Caption.Length();
				MarkEnd=CursorPos;
			}
			showHint(1);
			setHintPos(math::vector2d(event->x,event->y));
		}
		if(getRect().IsPointInside(math::vector2d(event->x,event->y))){
			guiMngr->setMouseOn(this);
			EventType=GET_Mouse_MOVE;
			ok=1;
		}
		break;
	}
	if(!getRect().IsPointInside(math::vector2d(event->x,event->y)))
		showHint(0);

	if(ok)
	{
		event->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->event=EventType;
		guiMngr->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
	return ok;*/
}


const core::string& GUIEditBox::getRegex()
{
	return m_component->m_regexExpStr;
}
bool GUIEditBox::setRegex(const core::string&val) 
{
	m_component->m_regexExpStr=val;
	if(m_component->m_regexExpression)
		delete m_component->m_regexExpression;
	if(val==mT(""))
	{
		m_component->m_regexExpression=0;
	}else
	{
		core::stringc str;
		core::string_to_char(val.c_str(),str);
		m_component->m_regexExpression=regex::Pattern::compile(str.c_str());
		if(!m_component->m_regexExpression)
			return false;
	}
	return true;

}


}
}
