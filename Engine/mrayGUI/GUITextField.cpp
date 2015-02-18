

#include "GUITextField.h"
#include "OS.h"


namespace mray{
namespace GUI{


GUITextField::GUITextField():CursorPos(0),
		startPos(0),MouseMark(0),MarkBegin(0),MarkEnd(0)
{
}

GUITextField::~GUITextField(){
}

void GUITextField::Event(SEvent&event){
	
	bool ok=0;
	GCPtr<GUIFont>font=getManager()->getFont();
	if(!font)return;
	math::rectf rc=getRect();
	GUI_EVENT_TYPE EventType=GET_NONE;

	if(event.EventType==ET_KEYBOARD && event.KeyboardInput.press){
		switch(event.KeyboardInput.key)
		{
		case KEY_DELETE:
			if(MarkEnd!=MarkBegin)
			{
				caption.erase(MarkBegin,MarkEnd);
				CursorPos=MarkBegin;
				MarkBegin=MarkEnd=0;
			}else
				if(CursorPos<caption.size())caption.erase(CursorPos);
			break;
		case KEY_BACK:
			
			if(MarkEnd!=MarkBegin)
			{
				caption.erase(MarkBegin,MarkEnd);
				CursorPos=MarkBegin;
				MarkBegin=MarkEnd=0;
			}else
			if(CursorPos>0)caption.erase(--CursorPos);
			break;
		case KEY_LEFT:
			if(event.KeyboardInput.shift)
			{
				if(MarkBegin==MarkEnd)
					MarkEnd=CursorPos;
				MarkBegin=(CursorPos>0)? CursorPos-1:CursorPos;
			}
			else
			{
				MarkBegin=MarkEnd=0;
			}
			CursorPos=(CursorPos>0)? CursorPos-1:CursorPos;
			break;
		case KEY_RIGHT:
			if(event.KeyboardInput.shift)
			{
				if(MarkBegin==MarkEnd)
					MarkBegin=CursorPos;
				MarkEnd=(CursorPos<caption.size())? CursorPos+1:CursorPos;
			}
			else
			{
				MarkBegin=MarkEnd=0;
			}
			CursorPos=(CursorPos<caption.size())? CursorPos+1:CursorPos;
			break;
		case KEY_HOME:
			if(event.KeyboardInput.shift)
			{
				if(MarkBegin==MarkEnd)
					MarkEnd=CursorPos;
				MarkBegin=0;
			}
			else
			{
				MarkBegin=MarkEnd=0;
			}
			CursorPos=0;
			startPos=0;
			break;
		case KEY_END:{
			if(event.KeyboardInput.shift)
			{
				if(MarkBegin==MarkEnd)
					MarkBegin=CursorPos;
				MarkEnd=caption.size();
			}
			else
			{
				MarkBegin=MarkEnd=0;
			}
			CursorPos=caption.size();
			/*
			float dim=0;
			int i;
			for(int i=caption.size()-1 && dim<rc.getWidth();i>=0;i--){
				dim+=font->getDimension(caption[i],fontSize);
			}
			if(dim>rc.getWidth())
			{
				startPos=i;
			}else
				startPos=0;*/
			}
			break;
		case KEY_UP:
		case KEY_DOWN:
		case KEY_TAB:
		case KEY_RETURN:
		case KEY_ESCAPE:
			break;
		default:
			if(event.KeyboardInput.Char==0 || event.KeyboardInput.Char<32) break;
			
			if(event.KeyboardInput.ctrl){
				switch(event.KeyboardInput.key){
				case KEY_KEY_C://copy
					{
						core::stringw str;
						str=caption.substr(MarkBegin,MarkEnd-MarkBegin);
						OS::OS::copyToClipboard(str.c_str());
					}
					break;
				case KEY_KEY_X://cut
					{
						core::stringw str;
						str=caption.substr(MarkBegin,MarkEnd-MarkBegin);
						caption.erase(MarkBegin,MarkEnd);
						OS::OS::copyToClipboard(str.c_str());
						MarkBegin=MarkEnd=0;
					}
					break;
				case KEY_KEY_V://paste
					{
						int a=MarkBegin,b=MarkEnd;
						if(MarkBegin==MarkEnd)a=b=CursorPos;
						core::stringw str=caption.substr(0,a);
						str+=OS::OS::getClipboardText();
						str+=caption.substr(b,caption.size()-b);
						caption=str;
					}
					break;
				}
				break;
			}
			if(CursorPos==caption.size())
				caption+=(wchar_t)event.KeyboardInput.Char;
			else
				caption.insert((wchar_t)event.KeyboardInput.Char,CursorPos);
			CursorPos++;
			if(event.KeyboardInput.shift)
			{
				if(MarkBegin!=MarkEnd)
					MarkEnd++;
			}
		};
		if(startPos>CursorPos)
			startPos=CursorPos;
		else
		{
			float dim=0;
			int c=0;
			float w=rc.getWidth();
			for(c=caption.size()-1 ;c>=0&& dim<w;c--){
				dim+=font->getDimension(caption[c],fontSize).x;
			}
			if(dim>rc.getWidth())
			{
				startPos=c+1+2;
			}else
				startPos=0;
		}
		if(startPos>CursorPos)
			startPos=CursorPos;
		ok=1;
		EventType=GET_CHANGED;
	}
	if(event.EventType==ET_MOUSE)
	{
		switch(event.MouseInput.Event)
		{
		case MET_LEFTDOWN:
			if(getRect().IsPointInside(math::vector2d(event.MouseInput.x,event.MouseInput.y)))
			{
				CursorPos=getManager()->getFont()->getCharFromPos(math::vector2d(event.MouseInput.x,event.MouseInput.y),fontSize,
					0,1,caption.c_str())+startPos;
				if(caption.size()<=CursorPos)
					CursorPos=caption.size();
				MouseMark=true;
				MarkEnd=MarkBegin=CursorPos;
				getManager()->setFocus(this);
				EventType=GET_MOUSE_DOWN;
				ok=1;
			}
			break;
		case MET_LEFTUP:
			if(getManager()->hasFocus(this))
			{
				if(CursorPos<MarkBegin)
				{
					MarkEnd=MarkBegin;
					MarkBegin=CursorPos;
				}
				EventType=GET_MOUSE_UP;
				MouseMark=0;
				ok=1;
			}
			break;
		case MET_MOVED:
			if(guiMngr->hasFocus(this))
			{
				if(MouseMark){
					CursorPos=guiMngr->getFont()->getCharFromPos(math::vector2d(event.MouseInput.x,event.MouseInput.y),fontSize(),
						clipRect,1,caption.c_str())+startPos;
					if(caption.size()<CursorPos)
						CursorPos=caption.size();
					MarkEnd=CursorPos;
				}
				showHint(1);
				setHintPos(math::vector2d(event.MouseInput.x,event.MouseInput.y));
			}
			if(getRect().IsPointInside(math::vector2d(event.MouseInput.x,event.MouseInput.y))){
				guiMngr->setMouseOn(this);
				EventType=GET_MOUSE_MOVE;
				ok=1;
			}
			break;
		}
		if(!getRect().IsPointInside(math::vector2d(event.MouseInput.x,event.MouseInput.y)))
			showHint(0);
	}
	if(ok)
	{
		event.recived=1;
		SEvent event1;
		event1.EventType=ET_GUI_EVENT;
		event1.GUIEvent.Caller=this;
		event1.GUIEvent.Event=EventType;
		if(guiMngr->getEventReciver())
			guiMngr->getEventReciver()->Event(event1);
		if(m_event)
			m_event->Event(event1);
	}
}

const wchar_t*GUITextField::getcaption{
}


}
}




