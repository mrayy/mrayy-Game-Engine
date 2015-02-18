



#include "GUIElement.h"
#include "GUIList.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "KeyCode.h"
#include "Event.h"
#include "KeyboardEvent.h"
#include "IEventReciver.h"
#include "GUIEvent.h"
#include "MouseEvent.h"
#include "EventMemoryManager.h"
#include "FontResourceManager.h"


namespace mray{
namespace GUI{

GUIList::GUIList():GUIElement(0),selectedItem(-1),
	drawBackgrnd(1)
{
	startItem=0;
	Selectable=true;

	m_FontAttributs.fontAligment=EFA_MiddleLeft;

	m_texture=MRAY_new video::TextureUnit();

	setTypeStr(mT("List"));
}
GUIList::GUIList(GUIManager* manager,const  core::UTFString& _text,const math::Point2df& pos,const math::Point2df& size,IEventReciver* event,
				 GUIElement* parent,const video::SColor& color,int id)
:GUIElement(manager,parent,pos,size,event,id),selectedItem(-1),
drawBackgrnd(1)
{

	Color=color;
	Caption=_text;
	startItem=0;
	Selectable=true;

	m_FontAttributs.fontAligment=EFA_MiddleLeft;
	m_texture=MRAY_new video::TextureUnit();

	setTypeStr(mT("List"));
}

GUIList::~GUIList()
{
	itemList.clear();
}


void GUIList::clear(){
	selectedItem=-1;
	itemList.clear();
	startItem=0;
}
void GUIList::onEvent(const EventPtr& e)
{
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e); 	
	if(e->isRecived())return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme();
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)return;
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::vector2d charDim=font->getCharDimension('A',m_FontAttributs.fontSize);
	math::rectf rc=getRect();
	rc.clipRect(clipRect);
	rc=skin->getSizableRect(4,rc,0,mT("Frame"));
	int shownItems=rc.getHeight()/charDim.y;
	shownItems-=1;
	switch(e->getType())
	{
	case ET_Keyboard:
		if(guiMngr->hasFocus(this))
		{
			GCPtr<KeyboardEvent>event=e;
			switch(event->key)
			{
			case KEY_RETURN:
				if(selectedItem!=-1){
					EventType=GET_Mouse_DOWN_AGAIN;//just like that!!
					ok=1;
				}
				break;
			case KEY_DOWN:
				selectedItem++;
				if(selectedItem>=itemList.size())
					selectedItem=itemList.size()-1;
				if(selectedItem>=shownItems+startItem)
					startItem=selectedItem-shownItems+1;
				EventType=GET_SELECT_ITEM;
				ok=1;
				break;
			case KEY_UP:
				selectedItem--;
				if(selectedItem<0)
					selectedItem=0;
				if(selectedItem<=startItem)
					startItem=selectedItem;
				EventType=GET_SELECT_ITEM;
				ok=1;
				break;
			default:
				if(isalpha((int)event->key))
				{
					bool flag=0;
					if(selectedItem!=-1){
						if(tolower(itemList[selectedItem].item[0])==tolower((int)event->key))
						{
							if(selectedItem<itemList.size()-1){
								if(tolower(itemList[selectedItem+1].item[0])==tolower((int)event->key)){
									selectedItem++;
									flag=1;
								}
							}
						}
					}
					if(!flag)
					{
						for(int i=0;i<itemList.size() && !flag;++i)
						{
							if(tolower(itemList[i].item[0])==tolower((int)event->key)){
								selectedItem=i;
								flag=1;
							}
						}
					}
					if(selectedItem>=shownItems+startItem)
						startItem=selectedItem-shownItems+1;
					if(selectedItem<=startItem)
						startItem=selectedItem;
				}
			}
		}
		break;
	case ET_Mouse:
		{
			GCPtr<MouseEvent>event=e;
		if(rc.IsPointInside(math::vector2d(event->x,event->y))&&
			!event->isRecived())
		{
			switch(event->event)
			{
			case MET_MOVED:
				EventType=GET_Mouse_MOVE;
				if(!guiMngr->hasMouseOn(this))
				{
					guiMngr->setMouseOn(this);
				}
				ok=1;
				break;
			case MET_LEFTDOWN:{
				EventType=GET_SELECT_ITEM;
				guiMngr->setFocus(this);
				//find selected item
				int sel=getItemFromPos(event->x,event->y);
				if(sel==selectedItem && sel!=-1)
					EventType=GET_Mouse_DOWN_AGAIN;
				if(sel!=-1)
					selectedItem=sel;
					
				ok=1;
				}
				break;
			case MET_LEFTUP:
				EventType=GET_Mouse_UP;
				ok=1;
				break;
			}
		}else
		{
			//showHint(0);
		}
		
		if(event->event==MET_WHEEL && guiMngr->hasMouseOn(this)){
			startItem+=-event->MouseWheel;
			if(startItem<0)startItem=0;
			if(startItem>itemList.size()-shownItems)
				startItem=itemList.size()-shownItems;
			}
		}
	};
	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->event=EventType;
		guiMngr->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
}

void GUIList::draw(float dt)
{
	if(!Visible)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	

	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)return;
	math::vector2d charSize=font->getCharDimension('A',m_FontAttributs.fontSize);
	
	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	
	video::SColor clr;

	if(drawBackgrnd){
		//Draw BackGround
		//skin->drawPanel(getRect(),bclr,&clipRect);
		skin->drawSizableBox(getManager()->GetRenderQueue(),getRect(),0,mT("Frame"),bclr,&clipRect);
	}
	math::rectf rc=getRect();
	rc.clipRect(getClipRect());
	rc=skin->getSizableRect(4,rc,0,mT("Frame"));
	math::rectf pos=rc;
	pos.BRPoint.y=pos.ULPoint.y+charSize.y;
	pos+=math::vector2d(charSize.y+2,0);//translate on x axis for icon

	clr=m_FontAttributs.fontColor;
	m_FontAttributs.fontColor.A=bclr.A;		
	for(int i=startItem;i<itemList.size() && pos.BRPoint.y<clipRect.BRPoint.y;++i)
	{
		const listItem& item=itemList[i];
		if(item.icon.GetImage()!=0){
			/*if(bank->getTexture(itemList[i].icon).pointer()!=0)*/
			{
				math::rectf pic=pos;
				pic.ULPoint-=math::vector2d(charSize.y+2,0);
				pic.BRPoint=pic.ULPoint+charSize.y;
				m_texture->SetTexture(item.icon.GetImageSet()->GetTexture()->GetTexture());
				device->draw2DImage(pic,m_texture,video::SColor(255,255,255,bclr.A),&clipRect,&item.icon.GetImage()->GetTextureRect());
			}
		}
//		font->print(pos,&m_FontAttributs,&clipRect,item.item,guiMngr->GetRenderQueue());
		pos.ULPoint.y+=charSize.y;
		pos.BRPoint.y+=charSize.y;
	}
	if(selectedItem!=-1){
		pos=rc;
		float charH=font->getCharDimension('A',m_FontAttributs.fontSize).y;
		pos.ULPoint.y+=(selectedItem-startItem)*charH;
		pos.BRPoint.y=pos.ULPoint.y+charH;
		guiMngr->getDevice()->draw2DRectangle(pos,video::SColor(128,128,255,128),1,&clipRect);
	}
	m_FontAttributs.fontColor=clr;		


	GUIElement::draw(dt);
}


int GUIList::getItemFromPos(float x,float y)
{
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)return -1;
	math::rectf rc=getRect();
	rc.clipRect(getClipRect());
	rc=skin->getSizableRect(4,rc,0,mT("Frame"));

	x-=rc.ULPoint.x;
	y-=rc.ULPoint.y;
	
	if(x<0 || x>rc.getWidth() || y<0 || y>rc.getHeight())
		return -1;

	int item;
	math::vector2d charDim=font->getCharDimension('A',m_FontAttributs.fontSize).y;
	item=y/charDim.y;

	if(startItem+item>=itemList.size())
		return -1;
	return startItem+item;
}

GUIList::listItem* GUIList::getItem(int index)
{
	if(index>=itemList.size())return 0;

	return &itemList[index];
}
int GUIList::getItemIndex(const core::UTFString& item)
{
	for(int i=0;i<itemList.size();++i)
	{
		if(itemList[i].item==item)
			return i;
	}
	return -1;
}
int GUIList::addItem(const core::UTFString& item,const video::ImageSetTexture& icon,void*userData)
{
	itemList.push_back(listItem(item,icon,userData));
	return itemList.size()-1;
}

void GUIList::setItem(int id,const core::UTFString& item,const video::ImageSetTexture& icon,void*userData)
{
	listItem* li=getItem(id);
	if(!li)return;
	li->item=item;
	li->icon=icon;
	li->userData=userData;
}
void GUIList::deleteItem(int index)
{
	if(index>=itemList.size())return;
	itemList.erase(index);
}

void GUIList::deleteItem(const core::UTFString& elem)
{
	int i=getItemIndex(elem);
	if(i!=-1)
		itemList.erase(i);
}

void GUIList::sortItems()
{
	core::UTFString str1,str2;
	//using bubble sort
	for(int i=0;i<itemList.size();++i){
		for(int j=i;j<itemList.size();j++)
		{
			str1=itemList[i].item;
//			str1.make_lower();
			str2=itemList[j].item;
//			str2.make_lower();
			if(str2<str1)
			{
				math::Swap<listItem>(itemList[i],itemList[j]);
			}
		}
	}
}



GCPtr<GUIElement> GUIList::getDuplicate()
{
	GUIList*dup=guiMngr->addList(getPos(),getSize(),m_event,getParent(),Color(),getId());
	
	dup->itemList.set_used(itemList.size());
	for(int i=0;i<itemList.size();++i)
		dup->itemList[i]=itemList[i];

	dup->drawBackgrnd=drawBackgrnd;
	return dup;
}



/*
void GUIList::setTextureBank(GCPtr<video::TextureBank>texBank)
{
	bank=texBank;
}
GCPtr<video::TextureBank>GUIList::getTextureBank(){
	return bank;
}
void GUIList::setIconTexture(video::ITexturePtr  tex,int index)
{
	if(!bank)bank=MRAY_new video::TextureBank();
	bank->insertTexture(tex,index);
}*/

FontAttributes* GUIList::getFontAttributes(){
	return &m_FontAttributs;
}



}
}
