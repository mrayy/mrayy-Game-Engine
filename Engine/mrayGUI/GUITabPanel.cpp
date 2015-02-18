

#include "GUITabPanel.h"
#include "GUIElement.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "KeyCode.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"
#include "IEventReciver.h"
#include "FontResourceManager.h"


namespace mray{
namespace GUI{


GUITabPanel::GUITabPanel():
GUIElement(0),currTab(0)
{
	Selectable=1;
	setTabCount(1);

	setTypeStr(mT("TabPanel"));
}

GUITabPanel::GUITabPanel(GUIManager* manager,int tabCount,math::Point2df pos,math::Point2df size,IEventReciver* event,
	GUIElement*  parent,video::SColor color,int id):
GUIElement(manager,parent,pos,size,event,id),currTab(0)
{
	Color=color;
	Selectable=1;
	
	if(tabCount<1)tabCount=1;
	setTabCount(tabCount);
	
	setTypeStr(mT("TabPanel"));
}

GUITabPanel::~GUITabPanel()
{
	tabs.clear();
}

void GUITabPanel::draw(float dt)
{
	//tab pabel texture
	//0 : tab image

#define T_MIN_CHAR 1
	if(!Visible)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)
		return;

	float charSY=font->getCharDimension('A',m_FontAttributs.fontSize).y;
	math::rectf rc=getRect();
	rc.clipRect(clipRect);
	if(!rc.isValid())return;
	//first draw tabs
	float width=0;
	math::rectf rtab(0,0,0,charSY);
	rtab+=rc.ULPoint;
//	rtab-=math::Point2df(0,charSY);//move it to the top of panel
	rc.ULPoint.y+=charSY;
	bool flag=0;
	video::SColor fclr=m_FontAttributs.fontColor;
	if(!Enable)
		m_FontAttributs.fontColor=video::SColor(128,128,128,255);
	video::SColor clr=Color();
	for(int i=0;i<tabs.size() && !flag;++i)
	{
		
		width=0;//font->getTextDimension(tabs[i].Caption,&m_FontAttributs,&rtab).x;
		rtab.BRPoint.x=rtab.ULPoint.x+width;
		if(rtab.BRPoint.x>rc.BRPoint.x){
			rtab.BRPoint.x=rc.BRPoint.x;
			flag=1;
		}
		if(i==currTab && Enable)
			m_FontAttributs.fontColor=fclr;
		else
			m_FontAttributs.fontColor=video::SColor(128,128,128,255);
		skin->drawBox(getManager()->GetRenderQueue(),rtab,mT("TabPanel"),0,clr);
//		font->print(rtab,&m_FontAttributs,0,tabs[i].Caption,guiMngr->GetRenderQueue());
		rtab+=math::Point2df(width,0);
	}
	m_FontAttributs.fontColor=fclr;
//	rc.ULPoint.y+=charDim.y;//sub the tabs height
	skin->drawSizableBox(getManager()->GetRenderQueue(),rc,0,mT("Panel"),Color());

	for(int i=0;i<tabs[currTab].children.size();++i){
		tabs[currTab].children[i]->draw(dt);
	}
	
}


void GUITabPanel::onEvent(const EventPtr& e)
{
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	for(int i=0;i<tabs[currTab].children.size() && !e->isRecived();++i)
		tabs[currTab].children[i]->onEvent(e);

	if(e->isRecived())return;
	if(!Visible)return;
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf rc=getRect();
	rc.clipRect(clipRect);
	
	switch(e->getType())
	{
	case ET_Mouse:
	{
		GCPtr<MouseEvent>event=e;
		if(rc.IsPointInside(math::vector2d(event->x,event->y))&&
			!event->isRecived())
		{
			if(event->Event==MET_MOVED)
			{
				if(!guiMngr->hasMouseOn(this))
				{
					EventType=GET_Mouse_MOVE;
					guiMngr->setMouseOn(this);
				}
				ok=1;
			}else
			if(event->Event==MET_LEFTDOWN)
			{
				if(Enable){
					checkTabs(math::Point2df(event->x,event->y));
					EventType=GET_Mouse_DOWN;
				}
				ok=1;
			}
			else if(event->Event==MET_LEFTUP)
			{
				EventType=GET_Mouse_UP;
				ok=1;
			}
		}
	}break;
	}

	if(ok){
		e->setRecived();
		GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
		event1->Caller=this;
		event1->Event=EventType;
		getManager()->onEvent(event1);
		if(m_event)
			m_event->onEvent(event1);
	}
}

std::vector<GCPtr<GUIElement> >*GUITabPanel::getTabChildren(int index)
{
	if(index<tabs.size())
		return &tabs[index].children;
	return 0;
}

void GUITabPanel::setTabCount(int count)
{
	for(int i=count-1;i<tabs.size();++i)
	{
		tabs[i].children.clear();
	}
	tabs.resize(count);
}

void GUITabPanel::setTabCaption(const  mchar*str,int index)
{
	if(index>=tabs.size())return;
	tabs[index].Caption=str;
}
void GUITabPanel::addTab(const  mchar*str,int index)
{
	if(index==-1)index=tabs.size();
	if(index>tabs.size())return;
	S_TAB st;
	st.Caption=str;
	tabs.insert(st,index);
}
void GUITabPanel::delTab(int index)
{
	if(index==-1)index=tabs.size();
	if(index>=tabs.size())return;
	tabs.erase(index);
}
void GUITabPanel::addChildToTab(GCPtr<GUIElement> elem,int index)
{
	if(index>tabs.size())return;
	tabs[index].children.push_back(elem);
	GUIElement::addChild(elem);
}
void GUITabPanel::removeChild(GCPtr<GUIElement> elem)
{
	for(int i=0;i<tabs.size();++i)
	{
		for(int j=0;j<tabs[i].children.size();j++)
			if(tabs[i].children[j]==elem){
				tabs[i].children.erase(j);
				removeChild(elem);
			}
	}
}

void GUITabPanel::checkTabs(math::Point2df pos)
{
#define T_MAX_CHAR 7
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)
		return;
	float charSY=font->getCharDimension('A',m_FontAttributs.fontSize).y;
	math::rectf rc=getRect();
	rc.BRPoint.y=rc.ULPoint.y+charSY;
	rc.clipRect(clipRect);
	if(!rc.IsPointInside(pos))return;
	
	pos-=rc.ULPoint;
	pos.y-=charSY;
	if(pos.y>charSY)return;
	for(int i=0;i<tabs.size();++i){
		float width=0;//font->getTextDimension(tabs[i].Caption,&m_FontAttributs,&rc).x;
		if(pos.x<width){
			currTab=i;
			return;
		}
		pos.x-=width;
	}

}

void GUITabPanel::addChild(GCPtr<GUIElement> elem)
{
	addChildToTab(elem,currTab);
}

GCPtr<GUIElement> GUITabPanel::getDuplicate()
{
	GUITabPanel*dup=guiMngr->addTabPanel(tabs.size(),getPos(),getSize(),m_event,getParent(),Color(),getId());
	
	dup->align=align;
	dup->width=width;

	dup->tabs.resize(tabs.size());
	for(int i=0;i<tabs.size();++i)
	{
		dup->tabs[i].Caption=tabs[i].Caption;
	}

	return dup;
}


FontAttributes* GUITabPanel::getFontAttributes(){
	return &m_FontAttributs;
}



}
}

