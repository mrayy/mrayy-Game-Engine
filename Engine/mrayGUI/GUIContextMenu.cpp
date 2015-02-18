
#include "GUIContextMenu.h"
#include "GUIManager.h"
#include "SColor.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"

namespace mray{
namespace GUI{

GUIContextMenu::GUIContextMenu():
	GUIElement(0),selectedItem(-1)
{
	setVisible(0);

	setTypeStr(mT("ContextMenu"));
}

GUIContextMenu::GUIContextMenu(GUIManager*mngr,IEventReciver* event,
	GCPtr<GUIElement>  parent,video::SColor color,int id):
	GUIElement(mngr,parent,0,0,event,id),selectedItem(-1)
{
	setVisible(0);
	SetManager(mngr);
	Color=color;
	recalcSize();
	
	setTypeStr(mT("ContextMenu"));
}

GUIContextMenu::~GUIContextMenu(){
	removeAllItems();
}

void GUIContextMenu::recalcSize(){
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);
	if(!font)return;
	math::vector2d charDim=font->getCharDimension('A',m_FontAttributes.fontSize);;
	charDim.x+=1;
	charDim.y+=4;
	float height=0;
	float maxWidth=0;
	for(int i=0;i<items.size();++i){
		items[i].posY=height;
		items[i].size.x=items[i].Caption.length()*charDim.x;
		if(items[i].size.x>maxWidth)
			maxWidth=items[i].size.x;
		if(!items[i].isSeperator){
			height+=charDim.y;
			items[i].size.y=charDim.y;
		}
		else{
			height+=SEPERATOR_HEIGHT+5;
			items[i].size.y=SEPERATOR_HEIGHT+5;
		}
	}
	for(int i=0;i<items.size();++i){
		if(items[i].subMenu)
		{
			math::vector2d p(maxWidth,items[i].posY);
			items[i].subMenu->MainPos=p;
		}
	}

	MainSize=(math::vector2d(maxWidth+10,height));
}


void GUIContextMenu::draw(float dt){
	if(!Visible)return;
	if(!guiMngr)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 
	if(!skin)return;
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);
	if(!font)return;

	math::rectf tRect=getRect();
	math::rectf clipRC=tRect;//getClipRect();

	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	video::SColor clr;

	skin->drawBox(getManager()->GetRenderQueue(),tRect,bclr,0,&clipRC,false);


	math::vector2d pos=tRect.ULPoint;
	for(int i=0;i<items.size();++i){
		math::rectf irc;
		irc.ULPoint=(math::vector2d(pos.x,pos.y+items[i].posY));
		irc.BRPoint=irc.ULPoint+items[i].size;
		if(items[i].isSeperator){
			irc.ULPoint.x+=5;
			irc.ULPoint.y+=(SEPERATOR_HEIGHT+5)/2.0f;
			irc.BRPoint.x=irc.ULPoint.x+getSize().x-10;
			irc.BRPoint.y=irc.ULPoint.y+SEPERATOR_HEIGHT;
			skin->drawBox(getManager()->GetRenderQueue(),irc,video::SColor(180,180,180,bclr.A),0,&clipRC,false);
		}else{
			clr=m_FontAttributes.fontColor;
			if(selectedItem==i){
				video::SColor selectedClr(64,64,128,AlphaEffect ? Alpha:Color().A);
				if(items[i].enabled){
					clr=video::SColor(255)-m_FontAttributes.fontColor;
					m_FontAttributes.fontColor.A=AlphaEffect ? Alpha:clr.A;
				}
				math::rectf r2(irc);
				r2.ULPoint.x=tRect.ULPoint.x;
				r2.BRPoint.x=tRect.BRPoint.x;
				skin->drawBox(getManager()->GetRenderQueue(),irc,selectedClr,selectedClr*0.5,&clipRC,true);
				
		//		if(items[i].isSubMenu && items[i].subMenu && selectedItem==i){
		//			items[i].subMenu->draw();
		//		}
			}
			if(!items[i].enabled)
				m_FontAttributes.fontColor.Set(128,128,128,m_FontAttributes.fontColor.A);
//			font->print(irc,&m_FontAttributes,&clipRC,items[i].Caption,guiMngr->GetRenderQueue());
			m_FontAttributes.fontColor=clr;
		}
	}
	GUIElement::draw(dt);
}

void GUIContextMenu::onEvent(const EventPtr& e){
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
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
				if(!Visible)
				{
					EventType=GET_Mouse_MOVE;
					guiMngr->setMouseOn(this);
					setHintPos(math::vector2d(event->x,event->y));
					showHint(1);
					if(selectedItem!=-1 && items[selectedItem].isSubMenu)
						items[selectedItem].subMenu->setVisible(0);

					selectedItem=getItemFromPos(
						math::vector2d(event->x,event->y));

					if(selectedItem!=-1 && items[selectedItem].isSubMenu)
						items[selectedItem].subMenu->setVisible(1);
					ok=1;
				}
			}break;
			case MET_LEFTDOWN:
			{
				if(!Visible)
				{
					if(Enable){
					EventType=GET_Mouse_DOWN;
					guiMngr->setFocus(this);
					}
					ok=1;
				}
			}break;
			case MET_LEFTUP:
			{
				if(!Visible)
				{
					if(Enable){
						if(selectedItem!=-1 && !items[selectedItem].isSubMenu){
							setVisible(0);
							EventType=GET_Mouse_UP;
							guiMngr->setFocus(this);
						}
					}
					ok=1;
				}
			}
			case MET_RIGHTDOWN:
				break;
			}
		}else
		{
			if(event->Event==MET_LEFTDOWN)
				setVisible(0);
			else
				if(parent && event->Event==MET_RIGHTUP)
				if(parent->getRect().IsPointInside(
						math::vector2d(event->x,event->y))&&
					!event->isRecived()){
						MainPos=(math::vector2d(event->x,event->y));
						setVisible(1);
						parent->bringToFront(this);
						ok=1;
					}
			selectedItem=-1;
			showHint(0);
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

int GUIContextMenu::getItemFromPos(math::vector2d p){
	math::rectf rc=getRect();
	p-=rc.ULPoint;
	rc.clipRect(getClipRect());
	rc-=rc.ULPoint;
	if(!rc.IsPointInside(p))return -1;
	for(int i=0;i<items.size();++i){
		if(p.y>items[i].posY && p.y<items[i].posY+items[i].size.y)
			if(items[i].enabled==0)return -1;
			else return i;
	}
	return -1;
}


int GUIContextMenu::addItem(const  mchar*Caption,int itemID,bool seperator,bool enabled,bool isSubMenu,
							GUIContextMenu*subMenu){
		SMenuItem item;
		item.Caption=Caption;
		item.enabled=enabled;
		item.isSeperator=seperator;
		item.isSubMenu=isSubMenu;
		item.itemId=itemID;
		item.posY=0;
		item.size=0;
		if(isSubMenu ){
			if(!subMenu){
				item.subMenu=new GUIContextMenu(getManager(),0,this);
				item.subMenu->MainSize=(math::vector2d(80,30));
				item.subMenu->setVisible(0);
			}else
				item.subMenu=subMenu;
		}else
			item.subMenu=0;
		items.push_back(item);

		recalcSize();
		return items.size()-1;
	
}
void GUIContextMenu::removeItem(int i){
	if(i<0 || i>=items.size())return;
	items.erase(i);
}
void GUIContextMenu::removeAllItems(){
	
	items.clear();
}
int GUIContextMenu::getItemCount(){
	return items.size();
}

int GUIContextMenu::getSelectedItem(){
	return selectedItem;
}

void GUIContextMenu::setItemCaption(int i,const mchar*Caption){
	if(i<0 || i>=items.size())return;
	items[i].Caption=Caption;
}

void GUIContextMenu::setSubMenu(int i,GUIContextMenu*subMenu){
	if(i<0 || i>=items.size())return;
	items[i].isSubMenu=1;
	items[i].subMenu=subMenu;
}

GUIContextMenu*GUIContextMenu::getSubMenu(int i){
	if(i<0 || i>=items.size())return 0;
	return items[i].subMenu;
}

const GUIContextMenu::SMenuItem *GUIContextMenu::getItem(int i){
	if(i<0 || i>=items.size())return 0;
	return &items[i];
}

void GUIContextMenu::setItemEnable(int i,bool enabled){
	if(i<0 || i>=items.size())return;
	items[i].enabled=enabled;
}
bool GUIContextMenu::isItemEnable(int i){
	if(i<0 || i>=items.size())return 0;
	return items[i].enabled;
}

int GUIContextMenu::getItemID(int i){
	if(i<0 || i>=items.size())return -1;
	return items[i].itemId;
}
void GUIContextMenu::setItemID(int i,int itemID){
	if(i<0 || i>=items.size())return;
	items[i].itemId=itemID;
}

FontAttributes* GUIContextMenu::getFontAttributes(){
	return &m_FontAttributes;
}

}
}
