
#include "stdafx.h"
#include "GUIWindow.h"
#include "TextureUnit.h"

#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "IEventReciver.h"
#include "FontResourceManager.h"

#include "IGUITheme.h"

namespace mray
{
namespace GUI
{

const GUID GUIWindow::ElementType("Window");
IMPLEMENT_ELEMENT_FACTORY(GUIWindow);

GUIWindow::GUIWindow(IGUIManager*mngr)
:IGUIPanelElement(ElementType,mngr),m_selectedIcon(ESI_None),m_mouseState(EMS_None),m_titleHeight(30)
{

	m_titleTexture=new video::TextureUnit();
	m_titleRegion=new GUIElementRegion(this);
	m_frameRegion=new GUIElementRegion(this);

	fillProperties();

}
GUIWindow::GUIWindow(const GUID& type,IGUIManager*mngr)
:IGUIPanelElement(type,mngr),m_selectedIcon(ESI_None),m_mouseState(EMS_None),m_titleHeight(30)
{

	m_titleTexture=new video::TextureUnit();
	m_titleRegion=new GUIElementRegion(this);
	m_frameRegion=new GUIElementRegion(this);

	fillProperties();

}

GUIWindow::~GUIWindow()
{
	m_titleTexture;
	delete m_titleRegion;
	delete m_frameRegion;
}

void GUIWindow::fillProperties()
{

	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
// 		dic->addPropertie(&PropertyTypeText::instance,mT("Apperance"));
// 		dic->addPropertie(&PropertyTypeIsMovable::instance,mT("Apperance"));
// 		dic->addPropertie(&PropertyTypeTitleThickness::instance,mT("Apperance"));
// 		dic->addPropertie(&PropertyTypeCanClose::instance,mT("Apperance"));
	}
}
void GUIWindow::_GetRects(const math::rectf& rc,math::rectf& title,math::rectf& frame)
{
	title.ULPoint=rc.ULPoint;
	title.BRPoint.x=rc.BRPoint.x;
	title.BRPoint.y=title.ULPoint.y+math::Min(m_titleHeight,rc.getHeight());
	
	frame.ULPoint.x=title.ULPoint.x;
	frame.ULPoint.y=title.BRPoint.y;
	frame.BRPoint=rc.BRPoint;
}
bool GUIWindow::_UpdateRegion(const math::rectf*vp)
{
	if(IGUIElement::_UpdateRegion(vp))
	{
		math::rectf title;
		_GetRects(GetDefaultRegion()->GetRect(),title,m_innerRect);
		IGUITheme* skin=GetCreator()->GetActiveTheme();
		if(skin)
		{
			m_frameRegion->SetRect(skin->getSizableRect(4,m_innerRect,0,mT("WindowFrame")));
		}else
			m_frameRegion->SetRect(m_innerRect);

		m_titleRegion->SetRect(title);
		m_innerRect.clipRect(GetDefaultRegion()->GetClippedRect());
		m_titleRegion->SetClipRect(GetDefaultRegion()->GetClippedRect());
		m_frameRegion->SetClipRect(GetDefaultRegion()->GetClippedRect());
		return true;
	}
	return false;
}
/*
const math::rectf& GUIWindow::GetUnclippedRect()
{
	if(m_unclippedRectDirt)
	{
		_GetRects(IGUIElement::GetUnclippedRect(),m_titleRect,m_innerRect);
		IGUITheme* skin=GetCreator()->GetActiveTheme();
		if(skin)
		{
			m_frameRect=skin->getSizableRect(4,m_innerRect,0,"WindowFrame");
		}else
			m_frameRect=m_innerRect;
	}
	return m_frameRect;
}*/

void GUIWindow::AddElement(IGUIElement* e)
{
	IGUIPanelElement::AddElement(e,m_frameRegion);
}

void GUIWindow::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	GUI::IFont* font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);

	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
//	GetUnclippedRect();//just to update rects
	_UpdateRegion(vp);
/*
	math::rectf innerRect(m_innerRect);
	math::rectf titleRect(m_titleRect);
	if(m_parent)
	{
		const math::rectf& clipRect=m_parent->GetClippedRect();
		innerRect.clipRect(clipRect);
		titleRect.clipRect(clipRect);
	}*/

	creator->GetDevice()->setScissorRect(GetDefaultRegion()->GetClippedRect());
	skin->drawSizableBox(creator->GetRenderQueue(),m_innerRect,0,mT("WindowFrame"));
	skin->drawSizableBox3PH(creator->GetRenderQueue(),m_titleRegion->GetClippedRect(),0,mT("TitleBar"));

	font->print(m_titleRegion->GetClippedRect(),&m_FontAttributes,0,GetText(),creator->GetRenderQueue());

	creator->GetRenderQueue()->Flush();
	creator->GetDevice()->setScissorRect(oldScissor);
	/*
	math::rectf tRect=getRect();
	//tRect.ULPoint.y-=TitleHeight;
	math::rectf titleRect=tRect;
	titleRect.ULPoint.y-=TitleHeight;

	titleRect.BRPoint.y=titleRect.ULPoint.y+TitleHeight;

	if(hideWindow)
	{
		if(MainSize().y>0)
		{
			MainSize=math::vector2d(MainSize().x,MainSize().y-1000*dt);
//			if(MainSize().y<=0)
//				getDevice()->decreaseDrawingObjects();
		}else {
			MainSize=math::vector2d(MainSize().x,0);
		}
		clipRect.BRPoint.y=MainPos().y+MainSize().y;
	}else
	{
		if(MainSize().y<winSize.y)
		{
			MainSize=math::vector2d(MainSize().x,MainSize().y+1000*dt);
//			if(MainSize().y>=winSize.y)
//				getDevice()->decreaseDrawingObjects();
		}else
			MainSize=math::vector2d(MainSize().x,winSize.y);
		clipRect.BRPoint.y=MainPos().y+MainSize().y;
	}
	this->updatePos();

	math::rectf sTitleRect=titleRect;
	if(!HideTitleBar()){
		if(!titleTexture->GetTexture()){
			skin->drawSizableBox3PH(getManager()->GetRenderQueue(),sTitleRect,0,mT("TitleBar"),255,0);
		}
		else{
			device->draw2DImage(sTitleRect,titleTexture,m_Color);
		}
	}

	math::vector2d start,end;
	start.x=sTitleRect.ULPoint.x;
	start.y=sTitleRect.BRPoint.y;

	end.x=sTitleRect.BRPoint.x;
	end.y=sTitleRect.BRPoint.y;

	sTitleRect.ULPoint.x+=TitleHeight;

//	skin->Draw3DSunkenPane(sTitleRect,&sTitleRect,0,m_color,
//		m_color,GR_FLAT);

	if(!bgTexture->GetTexture())
		skin->drawSizableBox(getManager()->GetRenderQueue(),tRect,0,mT("WindowFrame"),m_Color,&clipRect);
	else 
		device->draw2DImage(tRect,bgTexture,m_Color,&clipRect);

	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(font){
		math::vector2d charDim=font->getCharDimension('A',m_FontAttributs.fontSize);
		float tH=skin->getRect(1,sTitleRect,mT("TitleBar")).getHeight();
		sTitleRect.ULPoint.y=titleRect.ULPoint.y+(tH-charDim.y)*0.5f;
		sTitleRect.BRPoint.y=sTitleRect.ULPoint.y+charDim.y;

		if(!HideTitleBar())
			font->print(sTitleRect,&m_FontAttributs,&sTitleRect,getRenderString(),guiMngr->GetRenderQueue());
	}
	int iconOffset=0;

	
	math::rectf urRect;
	urRect.ULPoint.x=titleRect.BRPoint.x;
	urRect.ULPoint.y=titleRect.ULPoint.y;
	urRect.BRPoint=urRect.ULPoint;
	math::rectf iconSizeRC(math::vector2d(0),IconSize());

	if(Closable){
		math::rectf closeRc=skin->getRect(9,iconSizeRC,mT("TitleBar"));
		urRect.ULPoint.x-=closeRc.getWidth();
		urRect.BRPoint=urRect.ULPoint+closeRc.getSize();
		int index=9;
		if(m_selectedIcon==ESI_Close){
			switch(m_mouseState){
				case EMS_Hover:
					index=10;
				break;
				case EMS_Down:
					index=11;
				break;
			}
		}
		skin->drawBox(getManager()->GetRenderQueue(),urRect,mT("TitleBar"),index,255,0);
	}
	if(Collapse){
		math::rectf collapseRC=skin->getRect(3,iconSizeRC,mT("TitleBar"));
		urRect.ULPoint.x-=collapseRC.getWidth();
		urRect.BRPoint=urRect.ULPoint+collapseRC.getSize();
		int index=3;
		if(m_selectedIcon==ESI_Collapse){
			switch(m_mouseState){
				case EMS_Hover:
					index=4;
				break;
				case EMS_Down:
					index=5;
				break;
			}
		}
		if(hideWindow)
			index+=3;
		skin->drawBox(getManager()->GetRenderQueue(),urRect,mT("TitleBar"),index,255,0);
	} */
	IGUIPanelElement::Draw(vp);
}
bool GUIWindow::_OnKeyboardEvent(KeyboardEvent* e)
{
	return true;
}
bool GUIWindow::_OnMouseEvent(MouseEvent* e)
{
	if(e->event==MET_LEFTDOWN && m_titleRegion->GetClippedRect().IsPointInside(e->pos))
	{
		m_windowDrag.Enable(GetPosition(),e->pos);
	}
	if(e->event==MET_MOVED && m_windowDrag.IsEnabled())
	{
		SetPosition(m_windowDrag.OnMouseMove(e->pos));
	}
	if(e->event==MET_RIGHTDOWN && m_windowDrag.IsEnabled())
	{
		SetPosition(m_windowDrag.UndoDrag());
	}
	if(e->event==MET_LEFTUP && m_windowDrag.IsEnabled())
	{
		m_windowDrag.Disable();
	}
	return true;
}

/*
void GUIWindow::onEvent(const EventPtr& e)
{
	if(!Visible || !Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf winRect=getRect();
	math::rectf titleRect=winRect;
	titleRect.BRPoint.y=titleRect.ULPoint.y;
	titleRect.ULPoint.y-=TitleHeight;


	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme();

	math::rectf iconSizeRC(math::vector2d(0),IconSize());

	math::rectf urRect;
	urRect.ULPoint.x=titleRect.BRPoint.x;
	urRect.ULPoint.y=titleRect.ULPoint.y;
	urRect.BRPoint=urRect.ULPoint;

	titleRect.BRPoint.x-=TitleHeight;
	switch(e->getType())
	{
	case ET_Keyboard:
		break;
	case ET_Mouse:
		{
			GCPtr<MouseEvent>event=e;
			if(Closable )
			{
				math::rectf closeRc=skin->getRect(9,iconSizeRC,mT("TitleBar"));
				urRect.ULPoint.x-=closeRc.getWidth();
				urRect.BRPoint=urRect.ULPoint+closeRc.getSize();
				if(urRect.IsPointInside(math::vector2d(event->x,event->y))&&
					!event->isRecived())
				{
					m_selectedIcon=ESI_Close;
					if(event->event==MET_LEFTUP){
						this->setVisible(0);
						EventType=GET_Mouse_UP;
					}else if(event->event==MET_LEFTDOWN){
						m_mouseState=EMS_Down;
						EventType=GET_Mouse_DOWN;
					}else {
						m_mouseState=EMS_Hover;
						EventType=GET_Mouse_MOVE;
					}
					ok=1;
				}
			}
			if(Collapse){
				math::rectf collapseRC=skin->getRect(3,iconSizeRC,mT("TitleBar"));
				urRect.ULPoint.x-=collapseRC.getWidth();
				urRect.BRPoint=urRect.ULPoint+collapseRC.getSize();
				if(urRect.IsPointInside(math::vector2d(event->x,event->y))&&
				!event->isRecived() )
				{
					m_selectedIcon=ESI_Collapse;
					if(event->event==MET_LEFTUP){

						if(parent)
							parent->bringToFront(this);
						if(!hideWindow && MainSize().y==winSize.y)
							winSize=MainSize();
						hideWindow=!hideWindow;
						if(Transform){
//							if(MainSize().y==0 || MainSize().y==winSize.y)
//								getDevice()->increaseDrawingObjects();
						}
						else
						{
							if(hideWindow)
								MainSize=math::vector2d(MainSize().x,0);
							else
								MainSize=math::vector2d(MainSize().x,winSize.y);
						}
						EventType=GET_Mouse_UP;

					}else if(event->event==MET_LEFTDOWN){
						m_mouseState=EMS_Down;
						EventType=GET_Mouse_DOWN;
					}else {
						m_mouseState=EMS_Hover;
						EventType=GET_Mouse_MOVE;
					}

					ok=1;
					break;
				}
				urRect-=math::vector2d(TitleHeight,0);

				if(!ok){
					m_selectedIcon=ESI_None;
					m_mouseState=EMS_None;
				}
			}

			math::rectf ControlRC=skin->getRect(7,winRect,mT("WindowFrame"));
			if(ControlRC.IsPointInside(math::vector2d(event->x,event->y))&&
				!event->isRecived())
			{
				gCursorControl.setCursor(EC_SizeNS);
				ok=1;
			}else
			if(!event->isRecived())
			{
				if(titleRect.IsPointInside(math::vector2d(event->x,event->y))&&
					event->event==MET_LEFTDOWN && IsMovable)
				{
					if(parent)
						parent->bringToFront(this);
					if(Enable){
					mouseOn=1;
					lastPos=getPos()+getParentPos();
					mousePos=math::vector2d(event->x,event->y);
					guiMngr->setFocus(this);
					EventType=GET_Mouse_DOWN;
					}
					ok=1;
				}else 
					if(event->event==MET_MOVED && mouseOn)
				{
					EventType=GET_Mouse_MOVE;
					math::vector2d currPos=math::vector2d(event->x,event->y);
					this->MainPos=lastPos+(currPos-mousePos);
				}
				else if(winRect.IsPointInside(math::vector2d(event->x,event->y))&&(
					event->event==MET_LEFTDOWN) && !hideWindow)
				{
					if(hideWindow)return;
					if(parent)
						parent->bringToFront(this);
					EventType=GET_Mouse_DOWN;
					ok=1;
				}
				if(!titleRect.IsPointInside(math::vector2d(event->x,event->y))&&(
					event->event==MET_LEFTDOWN) || event->event==MET_LEFTUP)
				{
					mouseOn=0;
				}
			}
			if(!ok){

			//	CursorControl::getInstance().setCursor(EC_Arrow);
			}
		}break;
	};

	clipRect=getRect();

	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->event=EventType;
		getManager()->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
}
*/



void GUIWindow::SetTitleTexture(video::ITextureCRef  tex)
{
	m_titleTexture->SetTexture(tex);
}
video::TextureUnit* GUIWindow::GetTitleTexture()
{
	return m_titleTexture;
}

IGUIElement* GUIWindow::Duplicate()
{
	GUIWindow* dup=new GUIWindow(GetCreator());

	dup->SetTitleTexture(m_titleTexture->GetTexture());

	dup->SetText(GetText());

	dup->setCanClose(getCanClose());
	dup->setTitleHeight(getTitleHeight());
	dup->setIsMovable(getIsMovable());

	return dup;
}
FontAttributes* GUIWindow::GetFontAttributes()
{
	return &m_FontAttributes;
}



}
}
