


#include "stdafx.h"
#include "GUIComboList.h"

#include "IGUIManager.h"
#include "IGUITheme.h"
#include "IListItem.h"
#include "GUIComboListComponent.h"

namespace mray{
namespace GUI{

IMPLEMENT_ELEMENT_FACTORY(GUIComboList);


GUIComboList::GUIComboList(IGUIManager*mngr):IGUIComboList(mngr)
{
	m_component=new GUIComboListComponent(this);
	m_FontAttributes.fontAligment=EFA_MiddleLeft;

}
GUIComboList::~GUIComboList()
{
	ClearItems();
	delete m_component;
}


int GUIComboList::GetItemsCount()const
{
	return m_component->items.size();
}
IListItem* GUIComboList::GetItem(int index)const
{
	if(index<0 || index>=m_component->items.size())return 0;
	return m_component->items[index];
}
int GUIComboList::FindItem(const core::UTFString& txt)const
{
	for(int i=0;i<m_component->items.size();++i)
	{
		if(m_component->items[i]->toString()==txt)
			return i;
	}
	return -1;
}
int GUIComboList::AddItem(IListItem* item)
{
	m_component->items.push_back(item);
	return m_component->items.size()-1;
}
int GUIComboList::InsertItem(int index,IListItem* item)
{
	index=math::Min(index,(int)m_component->items.size());
	ItemList::iterator it=m_component->items.begin();
	std::advance(it,index);
	m_component->items.insert(it,item);
	return index;
}
void GUIComboList::DeleteItem(int index)
{
	if(index<0 || index>=m_component->items.size())return;
	ItemList::iterator it=m_component->items.begin();
	std::advance(it,index);
	delete *it;
	m_component->items.erase(it);
	int si=m_component->GetSelectedItem();
	m_component->SetSelectedItem(math::Min(si,(int)m_component->items.size()),m_component->items.size());
}
const ItemList& GUIComboList::GetItems()const
{
	return m_component->items;
}
void GUIComboList::SortItems()
{
	for(int i=0;i<m_component->items.size();++i)
	{
		for(int j=i+1;j<m_component->items.size();++j)
		{
			if(m_component->items[j]->toString()<m_component->items[i]->toString())
			{
				IListItem* itm=m_component->items[i];
				m_component->items[i]=m_component->items[j];
				m_component->items[j]=itm;
			}
		}
	}
}
void GUIComboList::ClearItems()
{
	for(int i=0;i<m_component->items.size();++i)
	{
		delete m_component->items[i];
	}
	m_component->items.clear();

	m_component->SetSelectedItem(-1,0);
	m_component->SetStartItem(0);
}

int GUIComboList::GetSelectedItem()
{
	return m_component->GetSelectedItem();
}
void GUIComboList::SetSelectedItem(int i)
{
	m_component->SetSelectedItem(i,m_component->items.size());
}

void GUIComboList::SetShowItemsCount(int c)
{
	m_component->SetMaxItems(c);
}
int GUIComboList::GetShowItemsCount()
{
	return m_component->GetMaxItems();
}

int GUIComboList::GetItemFromPos(const math::vector2d&pt)
{
	return m_component->GetItemFromPos(pt,GetDefaultRegion()->GetClippedRect());
}

void GUIComboList::SetSelectionColor(const video::SColor& clr)
{
	m_component->SetSelectionColor(clr);
}

const video::SColor& GUIComboList::GetSelectionColor()const
{
	return m_component->GetSelectionColor();
}
void GUIComboList::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;

	m_component->rc=GetDefaultRegion()->GetClippedRect();
	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	GetCreator()->GetDevice()->setScissorRect(m_component->rc);
	m_component->Draw();
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);
	IGUIElement::Draw(vp);
}
void GUIComboList::PostDraw(const math::rectf*vp)
{
	m_component->PostDraw();
}

IGUIElement* GUIComboList::Duplicate()
{
	return new GUIComboList(GetCreator());
}
FontAttributes* GUIComboList::GetFontAttributes()
{
	return &m_FontAttributes;
}



bool GUIComboList::_OnKeyboardEvent(KeyboardEvent* e)
{
	if(GetCreator()->HasFocus(this))
	{
		GUIComboListComponent::EResultEvent r=m_component->LBOnKeyboardEvent(e,m_component->rc);
		if(r==GUIComboListComponent::ESelectionChange)
		{
			int si=m_component->GetSelectedItem();
			OnSelectChange(this,&si);
		}
		return true;
	}
	return false;
}
bool GUIComboList::_OnMouseEvent(MouseEvent* e)
{
	const math::rectf& clipRect=GetDefaultRegion()->GetClippedRect();
	if(clipRect.IsPointInside(e->pos))
	{
		GetCreator()->SetMouseOn(this);
	}
	GUIComboListComponent::EResultEvent r=m_component->LBOnMouseEvent(e,m_component->rc);
	if(r==GUIComboListComponent::ENone)
		return false;
	if(e->event==MET_LEFTDOWN)
		GetCreator()->SetFocus(this);
	if(r==GUIComboListComponent::ESelectionChange)
	{
		int si=m_component->GetSelectedItem();
		OnSelectChange(this,&si);
	}

	return true;
}
void GUIComboList::_notifyLostFocus()
{
	IGUIElement::_notifyLostFocus();
	m_component->LostFocus();
}
/*


void GUIComboList::draw(float dt)
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


void GUIComboList::clear(){
selectedItem=-1;
itemList.clear();
startItem=0;
}
void GUIComboList::onEvent(const EventPtr& e)
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

int GUIComboList::getItemFromPos(float x,float y)
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

GUIComboList::listItem* GUIComboList::getItem(int index)
{
if(index>=itemList.size())return 0;

return &itemList[index];
}
int GUIComboList::getItemIndex(const core::UTFString& item)
{
for(int i=0;i<itemList.size();++i)
{
if(itemList[i].item==item)
return i;
}
return -1;
}
int GUIComboList::addItem(const core::UTFString& item,const video::ImageSetTexture& icon,void*userData)
{
itemList.push_back(listItem(item,icon,userData));
return itemList.size()-1;
}

void GUIComboList::setItem(int id,const core::UTFString& item,const video::ImageSetTexture& icon,void*userData)
{
listItem* li=getItem(id);
if(!li)return;
li->item=item;
li->icon=icon;
li->userData=userData;
}
void GUIComboList::deleteItem(int index)
{
if(index>=itemList.size())return;
itemList.erase(index);
}

void GUIComboList::deleteItem(const core::UTFString& elem)
{
int i=getItemIndex(elem);
if(i!=-1)
itemList.erase(i);
}

void GUIComboList::sortItems()
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



GCPtr<GUIElement> GUIComboList::getDuplicate()
{
GUIComboList*dup=guiMngr->addList(getPos(),getSize(),m_event,getParent(),Color(),getId());

dup->itemList.resize(itemList.size());
for(int i=0;i<itemList.size();++i)
dup->itemList[i]=itemList[i];

dup->drawBackgrnd=drawBackgrnd;
return dup;
}

FontAttributes* GUIComboList::getFontAttributes(){
return &m_FontAttributs;
}

*/

}
}
