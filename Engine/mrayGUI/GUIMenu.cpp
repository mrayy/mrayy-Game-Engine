
#include "GUIMenu.h"
#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"


namespace mray{
namespace GUI{

GUIMenu::GUIMenu():
	GUIElement(0),selectedMenu(-1),selecting(0)
{


	setTypeStr(mT("Menu"));

}
GUIMenu::GUIMenu(GUIManager* mngr,IEventReciver*  event,
				 GCPtr<GUIElement>  parent,video::SColor color,int id):
	GUIElement(mngr,parent,0,0,event,id),selectedMenu(-1),selecting(0)
{
	Color=color;
	recalcSize();

	
	setTypeStr(mT("Menu"));

}
GUIMenu::~GUIMenu(){
	clearAllMenus();
}

void GUIMenu::recalcSize(){
	if(!guiMngr)return;
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)
		return;
	math::vector2d charDim=font->getCharDimension('A',m_FontAttributs.fontSize);
	
	math::vector2d sz;
	if(parent)
		sz=parent->getSize();
	else
		sz=getDevice()->getViewportRect().getSize();
	
	charDim.x+=1;
	charDim.y+=14;

	MainPos=0;
	sz.y=charDim.y;

	MainSize=sz;

	float posX=0;
	for(int i=0;i<menus.size();++i){
		menus[i].posX=posX;
		menus[i].size.y=charDim.y;
		if(!menus[i].isSeperator){
			menus[i].size.x=menus[i].Caption.length()*charDim.x+10;
			posX+=menus[i].size.x;
			if(menus[i].menu){
				menus[i].menu->MainPos=(math::vector2d(menus[i].posX,menus[i].size.y));
			}
		}
		else{
			posX+=SEPERATOR_WIDTH+5;
			menus[i].size.x=SEPERATOR_WIDTH+5;
		}
	}
}


void GUIMenu::draw(float dt){
	if(!Visible)return;
	if(!guiMngr)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	
	if(!skin)return;
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);

	math::rectf tRect=getRect();
	math::rectf clipRC=getClipRect();

	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	
	video::SColor clr;

	skin->drawBox(getManager()->GetRenderQueue(),tRect,bclr,0,&clipRC,false);

	math::vector2d pos=tRect.ULPoint;
	for(int i=0;i<menus.size();++i){
		math::rectf irc;
		irc.ULPoint=math::vector2d(pos.x+menus[i].posX,pos.y);
		irc.BRPoint=irc.ULPoint+menus[i].size;
		if(menus[i].isSeperator){
			irc.ULPoint.x+=(SEPERATOR_WIDTH+5)/2.0f;
			irc.ULPoint.y+=2;
			irc.BRPoint.x=irc.ULPoint.x+SEPERATOR_WIDTH;
			irc.BRPoint.y=irc.ULPoint.y+getSize().y-4;
			skin->drawBox(getManager()->GetRenderQueue(),irc,video::SColor(50,50,50,bclr.A),0,&clipRC,false);
		}else{
			clr=m_FontAttributs.fontColor;
			if(selectedMenu==i){
				video::SColor selectedClr(64,64,128,128);
				if(menus[i].enabled){
					m_FontAttributs.fontColor=video::SColor(255)-m_FontAttributs.fontColor;
				}
				math::rectf r2(irc);
		//		r2.ULPoint.x=tRect.ULPoint.x;
		//		r2.BRPoint.x=tRect.BRPoint.x;
				skin->drawBox(getManager()->GetRenderQueue(),r2,selectedClr,selectedClr*0.5,&clipRC);
				if(menus[i].menu && selecting ){
					menus[i].menu->Visible=1;
					menus[i].menu->draw(dt);
				}
			}
			if(!menus[i].enabled)
				m_FontAttributs.fontColor=128;
			m_FontAttributs.fontColor.A=AlphaEffect ? Alpha:m_FontAttributs.fontColor.A;
//			if(font)
//				font->print(irc,&m_FontAttributs,&clipRC,menus[i].Caption,guiMngr->GetRenderQueue());
			m_FontAttributs.fontColor=clr;
		}
	}
	GUIElement::draw(dt);
}



void GUIMenu::onEvent(const EventPtr& e){
	if(!Enable)return;
	if(!Visible)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	if(selectedMenu>-1)
		menus[selectedMenu].menu->onEvent(e);
	if(e->isRecived())return;

	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf rc=getRect();
	switch(e->getType())
	{
	case ET_Keyboard:
		if(guiMngr->hasFocus(this))
		{
		}
		break;
	case ET_Mouse:
		{
			GCPtr<MouseEvent> event=e;
		if(rc.IsPointInside(math::vector2d(event->x,event->y))&&
			!event->isRecived())
		{
			switch(event->Event){
			case MET_MOVED:
			{
				EventType=GET_Mouse_MOVE;
				guiMngr->setMouseOn(this);

				selectedMenu=getMenuFromPos(
					math::vector2d(event->x,event->y));

				ok=1;
			}break;
			case MET_LEFTDOWN:
			{
				selecting=!selecting;
				EventType=GET_Mouse_DOWN;
				guiMngr->setFocus(this);
				ok=1;
			}break;
			case MET_LEFTUP:
			{
				EventType=GET_Mouse_UP;
				guiMngr->setFocus(this);
				ok=1;
			}
			case MET_RIGHTDOWN:
				break;
			}
		}else
		{
			if(event->Event==MET_LEFTDOWN){
				selectedMenu=-1;
				selecting=0;
			}
		}break;
		}
	};
	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->Event=EventType;
		guiMngr->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
}

int GUIMenu::addMenu(const  mchar*Caption,bool seperator,bool enabled,
							GUIContextMenu*subMenu){
		SMenu menu;
		menu.Caption=Caption;
		menu.enabled=enabled;
		menu.isSeperator=seperator;
		menu.posX=0;
		menu.size=0;
		if(!subMenu){
			menu.menu=new GUIContextMenu(getManager(),m_event,0);
			menu.menu->MainSize=(math::vector2d(80,30));
			menu.menu->Visible=0;
		}else
			menu.menu=subMenu;
		menus.push_back(menu);

		recalcSize();
		return menus.size()-1;
	
}



void GUIMenu::removeMenu(int i){
	if(i<0 || i>=menus.size())return;
	menus.erase(i);
}
void GUIMenu::clearAllMenus(){

	menus.clear();
}


int GUIMenu::getMenuFromPos(math::vector2d p){
	math::rectf rc=getRect();
	p-=rc.ULPoint;
	rc.clipRect(getClipRect());
	rc-=rc.ULPoint;
	if(!rc.IsPointInside(p))return -1;
	for(int i=0;i<menus.size();++i){
		if(p.x>menus[i].posX && p.x<menus[i].posX+menus[i].size.x)
			if(menus[i].enabled==0)return -1;
			else return i;
	}
	return -1;
}


int GUIMenu::getMenusCount(){
	return menus.size();
}

int GUIMenu::getSelectedMenu(){
	return selectedMenu;
}

void GUIMenu::setMenuCaption(int i,const mchar*Caption){
	if(i<0 || i>=menus.size())return;
	menus[i].Caption=Caption;
}

void GUIMenu::setSubMenu(int i,GCPtr<GUIContextMenu>subMenu){
	if(i<0 || i>=menus.size())return;
	menus[i].menu=subMenu;
}

GCPtr<GUIContextMenu>GUIMenu::getSubMenu(int i){
	if(i<0 || i>=menus.size())return 0;
	return menus[i].menu;
}

void GUIMenu::setMenuEnable(int i,bool enabled){
	if(i<0 || i>=menus.size())return;
	menus[i].enabled=enabled;
}

bool GUIMenu::isMenuEnable(int i){
	if(i<0 || i>=menus.size())return 0;
	return menus[i].enabled;
}


FontAttributes* GUIMenu::getFontAttributes(){
	return &m_FontAttributs;
}

}
}

