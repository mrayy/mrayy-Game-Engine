


#include "GUIElement.h"
#include "GUIRadioButton.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "KeyCode.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"
#include "IEventReciver.h"
#include "CPropertie.h"

#include "TextureResourceManager.h"
#include "FontResourceManager.h"

namespace mray{
namespace GUI{


GUIRadioButton::GUIRadioButton():GUIElement(0),
	Checked(this,&GUIRadioButton::setChecked,&GUIRadioButton::getChecked,EPT_BOOLEAN,mT("isChecked"),mT("status of the Radio Button")),
	BGOpaque(this,&GUIRadioButton::setBGOpaque,&GUIRadioButton::getBGOpaque,EPT_BOOLEAN,mT("BGOpaque"),mT("is back ground opaque")),
	PushButton(this,&GUIRadioButton::setPushButton,&GUIRadioButton::getPushButton,EPT_BOOLEAN,mT("PushButton"),mT("make it as Button"))
{
	m_checked=0;
	BGOpaque=0;
	PushButton=0;
	m_state=ER_Unchecked;
	bkTexture=new video::TextureUnit();
	Selectable=true;

	setTypeStr(mT("RadioButton"));
}

GUIRadioButton::GUIRadioButton(GUIManager* manager,const  core::UTFString& _text,math::vector2d pos,math::vector2d size,IEventReciver* event,
						 GCPtr<GUIElement>  parent,video::SColor color,int id)
:GUIElement(manager,parent,pos,size,event,id),
Checked(this,&GUIRadioButton::setChecked,&GUIRadioButton::getChecked,EPT_BOOLEAN,mT("isChecked"),mT("status of the Radio Button")),
BGOpaque(this,&GUIRadioButton::setBGOpaque,&GUIRadioButton::getBGOpaque,EPT_BOOLEAN,mT("BGOpaque"),mT("is back ground opaque")),
PushButton(this,&GUIRadioButton::setPushButton,&GUIRadioButton::getPushButton,EPT_BOOLEAN,mT("PushButton"),mT("make it as Button"))
{

	m_checked=0;
	BGOpaque=0;
	PushButton=0;

	Color=color;
	Caption=_text;
	bkTexture=new video::TextureUnit();
	Selectable=true;
	m_state=ER_Unchecked;
	
	setTypeStr(mT("RadioButton"));
}



GUIRadioButton::~GUIRadioButton()
{
}


void GUIRadioButton::fillProperties(){
	addPropertie(&Checked,mT("Apperance"));
	addPropertie(&BGOpaque,mT("Apperance"));
	addPropertie(&PushButton,mT("Apperance"));
}
void GUIRadioButton::loadTextures()
{
	return;
// 	if(!device)return;
// 	video::ITexturePtr  tex =gTextureResourceManager.loadTexture2D(core::UTFString(core::UTFString(getManager()->getDataPath())+mT("radioBtn.png")).c_str());
// 	if(tex && tex->getType()!=video::ETT_2DTex)
// 		tex=0;
// 	bkTexture=video::ITexturePtr (tex);
}

void GUIRadioButton::onEvent(const EventPtr& e)
{
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
					setChecked(1);
					m_state=Checked ? ER_Checked : ER_Unchecked;
				}
			
				EventType=GET_Mouse_DOWN;
			}
			ok=1;
		}
		break;
	}
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
					if(!Checked)
						m_state=ER_MouseOn;
				}
				ok=1;
			}else
			if(event->event==MET_LEFTDOWN)
			{
				if(Enable){
				setChecked(1);
				EventType=GET_Mouse_DOWN;
				guiMngr->setFocus(this);
				guiMngr->setMouseOn(this);
				m_state=ER_MouseDown;
				}
				ok=1;
			}else if(event->event==MET_LEFTUP)
			{
				if(Enable){
				EventType=GET_Mouse_UP;
				m_state=Checked ? ER_Checked : ER_Unchecked;
				}
				ok=1;
				break;
			}
		}else{
			m_state=Checked ? ER_Checked : ER_Unchecked;
			showHint(0);
		}
		}break;
	};

	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->event=EventType;
			
		getManager()->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}else{
	}
}


void GUIRadioButton::draw(float dt)
{
	if(!Visible)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme();
	math::rectf tRect=getRect();
	math::rectf tChBox=tRect;
	tChBox.BRPoint=tChBox.ULPoint+skin->getRect(0,math::rectf(0,0,24,24),mT("RadioButton")).getSize();
/*
	tChBox.BRPoint=tChBox.ULPoint;
	tChBox.ULPoint.y+=(tRect.getHeight()-24)/2;
	tChBox.BRPoint.x+=24;
	tChBox.BRPoint.y=tChBox.ULPoint.y+24;
	tChBox.clipRect(clipRect);
*/
	tRect.ULPoint.x+=tChBox.getWidth();
	
	float yCoord=0;
	int butState;
	int state;

	switch (m_state){
		case ER_Unchecked:
			yCoord=0.0f;
			state=0;
			butState=0;
			break;
		case ER_Checked:
			yCoord=0.25f;
			state=1;
			butState=2;
			break;
		case ER_MouseOn:
			yCoord=0.5f;
			state=2;
			butState=1;
			break;
		case ER_MouseDown:
			yCoord=0.75f;
			state=3;
			butState=2;
			break;
	}

	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	
	video::SColor clr;

	math::rectf coordsRect(0,yCoord,1,yCoord+0.25f);

	if(!PushButton){
/*		if(BGOpaque() || !bkTexture)
			skin->drawBox(tRect,mT("RadioButton"),state,bclr,&clipRect);
		
		if(bkTexture)
			device->draw2DImage(tChBox,bkTexture,video::SColor(255,255,255,bclr.A),0,&coordsRect);
*/
		if(BGOpaque && !bkTexture->GetTexture())
			skin->drawBox(getManager()->GetRenderQueue(),tRect,bclr,bclr*0.5,&clipRect);
		
		if(bkTexture->GetTexture())
			device->draw2DImage(tChBox,bkTexture,video::SColor(255,255,255,bclr.A),0,&coordsRect);
		else{
			if(Checked)
			{
				skin->drawBox(getManager()->GetRenderQueue(),tChBox,mT("RadioButton"),state,bclr,&(clipRect+1),true);
			}else
				skin->drawBox(getManager()->GetRenderQueue(),tChBox,mT("RadioButton"),state,bclr,&(clipRect+1),false);
		}

		if(tChBox.isValid()){
			tChBox.ULPoint+=2;
			tChBox.BRPoint-=2;
		}
	}else{
		if(Checked){
			clr=Color();
			clr.A=AlphaEffect ? Alpha:bclr.A;
			skin->drawSizableBox(getManager()->GetRenderQueue(),tRect,butState,mT("PushButton"),bclr,&(clipRect+1));
		}else
		{
			clr=Color();
			clr.A=AlphaEffect ? Alpha:bclr.A;
			skin->drawSizableBox(getManager()->GetRenderQueue(),tRect,butState,mT("PushButton"),bclr,&(clipRect+1));
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
		IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
		if(!font)
			return;
		clr=m_FontAttributs.fontColor;
		m_FontAttributs.fontColor.A=AlphaEffect ? Alpha:m_FontAttributs.fontColor.A;
		if(!Enable)
			m_FontAttributs.fontColor.Set(128,128,128,m_FontAttributs.fontColor.A);
			
		font->print(tRect,&m_FontAttributs,&clipRect,getRenderString(),guiMngr->GetRenderQueue());
	}
		
	GUIElement::draw(dt);
}

void GUIRadioButton::setChecked(const bool& c)
{
	if(c){
		std::list<GCPtr<GUIElement> >* pChld=parent->getChildren();
		std::list<GCPtr<GUIElement> >::iterator it=pChld->begin();
		for(;it!=pChld->end();++it)
		{
			GCPtr<GUIRadioButton>btn=GCPtr<GUIRadioButton>((*it));
			if(btn)
			{
				btn->Checked=0;
				btn->m_state=ER_Unchecked;
			}
		}
	}
	m_checked=c;
}


GCPtr<GUIElement> GUIRadioButton::getDuplicate()
{
	GCPtr<GUIRadioButton>dup=guiMngr->addRadioButton(Caption().c_str(),getPos(),getSize(),m_event,getParent(),Color(),Id());

	dup->PushButton=PushButton;
	dup->BGOpaque=BGOpaque();


	dup->bkTexture=bkTexture;


	return dup;
}

FontAttributes* GUIRadioButton::getFontAttributes(){
	return &m_FontAttributs;
}







}
}

