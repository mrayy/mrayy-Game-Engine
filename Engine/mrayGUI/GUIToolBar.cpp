

#include "GUIToolBar.h"
#include "GUIElement.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "KeyCode.h"
#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"
#include "IEventReciver.h"
//#include "SceneManager.h"


namespace mray{
namespace GUI{

GUIToolBar::GUIToolBar():
GUIElement(0)
{
	Selectable=false;

	setTypeStr(mT("ToolBar"));
}

GUIToolBar::GUIToolBar(GUIManager* manager,int Width,EAlignment Align,IEventReciver* event,
	GCPtr<GUIElement>  parent,video::SColor color,int id):
GUIElement(manager,parent,0,0,event,id),width(Width),align(Align)
{
	Color=color;
	Selectable=false;
	
	setTypeStr(mT("ToolBar"));

}

GUIToolBar::~GUIToolBar()
{
}

void GUIToolBar::draw(float dt)
{
	if(!Visible)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	

	math::rectf rc;
//	recti rv;
//	SceneManager* smngr=device->getWindow()->getSceneMngr();
//	if(smngr->getActiveCamera())
//		rv=smngr->getActiveCamera()->getViewPort();
	switch(align)
	{
	case A_LEFT:
		rc.ULPoint=0;
		rc.BRPoint.x=width;
		rc.BRPoint.y=device->getScreenSize().y;
	//	if(rv.ULPoint.x<rc.BRPoint.x)
	//		rv.ULPoint.x=rc.BRPoint.x;
		MainPos=0;
		MainSize=math::vector2d(width,device->getScreenSize().y);
		break;
	case A_RIGHT:
		rc.ULPoint.x=device->getScreenSize().x-width;
		rc.ULPoint.y=0;
		rc.BRPoint.x=device->getScreenSize().x;
		rc.BRPoint.y=device->getScreenSize().y;
	//	if(rv.BRPoint.x>rc.ULPoint.x)
		//		rv.BRPoint.x=rc.ULPoint.x;
		MainPos=math::vector2d(rc.ULPoint.x,0);
		MainSize=math::vector2d(width,rc.BRPoint.y);
		break;
	case A_TOP:
		rc.ULPoint=0;
		rc.BRPoint.x=device->getScreenSize().x;
		rc.BRPoint.y=width;
	//	if(rv.ULPoint.y<rc.BRPoint.y)
	//		rv.ULPoint.y=rc.BRPoint.y;
		MainPos=0;
		MainSize=math::vector2d(width,rc.BRPoint.y);
		break;
	case A_BOTTOM:
		rc.ULPoint.x=0;
		rc.ULPoint.y=device->getScreenSize().y-width;
		rc.BRPoint.x=device->getScreenSize().x;
		rc.BRPoint.y=device->getScreenSize().y;
	//	if(rv.BRPoint.y>rc.ULPoint.y)
	//		rv.BRPoint.y=rc.ULPoint.y;
		MainPos=math::vector2d(0,rc.ULPoint.y);
		MainSize=math::vector2d(width,rc.BRPoint.y);
		break;
	}
// 	setPos(mainPos());
// 	setSize(MainSize());


	skin->drawBox(getManager()->GetRenderQueue(),rc,m_Color,0,0,0);
		
	GUIElement::draw(dt);
}


void GUIToolBar::onEvent(const EventPtr& e)
{
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	if(!Visible)return;
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf rc=getRect();
	rc.clipRect(clipRect);
	
	switch(e->getType())
	{
	case ET_Mouse:
	{
		GCPtr<MouseEvent> event=e;
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



GCPtr<GUIElement> GUIToolBar::getDuplicate()
{
	GUIToolBar*dup=guiMngr->addToolBar(width,align,m_event,getParent(),Color(),getId());
	
	return dup;
}


}
}
