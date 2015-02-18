



#include "GUIElement.h"
#include "GUISlider.h"
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

namespace mray{
namespace GUI{


GUISlider::GUISlider():GUIElement(0),
	mouseDown(0),
	Value(this,&GUISlider::setValue,&GUISlider::getValue,EPT_INT,mT("Value"),mT("the value of the slider")),
	MinValue(this,&GUISlider::setMinValue,&GUISlider::getMinValue,EPT_INT,mT("minValue"),mT("Minimum valu for the slider")),
	MaxValue(this,&GUISlider::setMaxValue,&GUISlider::getMaxValue,EPT_INT,mT("maxValue"),mT("Maximum valu for the slider"))
{

	bkTex=new video::TextureUnit();
	crTex=new video::TextureUnit();
	Selectable=true;

	Value=0;
	MinValue=0;
	MaxValue=100;

	m_state=MouseOff;

	setTypeStr(mT("Slider"));

	fillProperties();
}
GUISlider::GUISlider(GUIManager* manager,math::vector2d pos,math::vector2d size,IEventReciver* event,
						 GCPtr<GUIElement>  parent,video::SColor color,int id)
:GUIElement(manager,parent,pos,size,event,id),
mouseDown(0),
Value(this,&GUISlider::setValue,&GUISlider::getValue,EPT_INT,mT("Value"),mT("the value of the slider")),
MinValue(this,&GUISlider::setMinValue,&GUISlider::getMinValue,EPT_INT,mT("minValue"),mT("Minimum valu for the slider")),
MaxValue(this,&GUISlider::setMaxValue,&GUISlider::getMaxValue,EPT_INT,mT("maxValue"),mT("Maximum valu for the slider"))
{

	bkTex=new video::TextureUnit();
	crTex=new video::TextureUnit();
	Selectable=true;
	Color=color;

	Value=0;
	MinValue=0;
	MaxValue=100;

	m_state=MouseOff;

	setTypeStr(mT("Slider"));

	fillProperties();
}



GUISlider::~GUISlider()
{
}

void GUISlider::fillProperties(){
	addPropertie(&Value,mT("Apperance"));
	addPropertie(&MinValue,mT("Apperance"));
	addPropertie(&MaxValue,mT("Apperance"));
}

void GUISlider::setBackTexture(const  mchar*texName)
{
	if(!device)return;
}


void GUISlider::setCursorTexture(const  mchar*texName)
{
	if(!device)return;
}

void GUISlider::loadTextures()
{
	return;

}
void GUISlider::onEvent(const EventPtr& e)
{
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	bool ok=0;
	GUIElement::onEvent(e);
	if(!Visible)return;
	GUI_EVENT_TYPE EventType=GET_NONE;
	math::rectf btnRect=getRect();
	switch(e->getType())
	{
	case ET_Keyboard:
	{
		GCPtr<KeyboardEvent>event=e;
		if(guiMngr->hasFocus(this) && event->press)
		{
			if(Enable){
				switch(event->key)
				{
				case KEY_LEFT:
					Value=Value-1;
					if(Value<MinValue)Value=MinValue;
					EventType=GET_CHANGED;
					ok=1;
					break;
				case KEY_RIGHT:
					Value=Value+1;
					if(Value>MaxValue)Value=MaxValue;
					EventType=GET_CHANGED;
					ok=1;
					break;
				}
			}
		}
		break;
	}
	case ET_Mouse:
		{
			GCPtr<MouseEvent>event=e;
			if(event->Event==MET_MOVED)
			{
				if(!guiMngr->hasMouseOn(this))
				{
					EventType=GET_Mouse_MOVE;
					guiMngr->setMouseOn(this);
				}
				if(mouseDown){
					Value=MinValue+(event->x-(getPos().x+getParentPos().x))
						*MaxValue/getWidth();
					Value=math::clamp(Value(),MinValue(),MaxValue());
					guiMngr->setFocus(this);
					EventType=GET_CHANGED;
					ok=1;
				}
			}
			if(btnRect.IsPointInside(math::vector2d(event->x,event->y))&&
				!event->isRecived())
			{
				if(event->Event==MET_MOVED)
				{
					if(!guiMngr->hasMouseOn(this))
					{
						EventType=GET_Mouse_MOVE;
						guiMngr->setMouseOn(this);
					}
					m_state = MouseOn;
					if(mouseDown)
						EventType=GET_CHANGED;
					ok=1;
				}
				if(event->Event==MET_LEFTDOWN && Enable){
					Value=MinValue+(event->x-(getPos().x+getParentPos().x))
						*MaxValue/getWidth();
					EventType=GET_CHANGED;
					m_state = MouseDown;
					guiMngr->setFocus(this);
					mouseDown=1;
				}
				ok=1;
			}else{
				if(!mouseDown)
					m_state = MouseOff;
			}
		
		if(event->Event==MET_LEFTUP)
		{
			EventType=GET_Mouse_UP;
			mouseDown=0;
			m_state = MouseOff;
		}
		}break;
	};

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


void GUISlider::draw(float dt)
{
	if(!Visible)return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	

	Value = math::clamp(Value(),MinValue(),MaxValue());

	math::rectf tRect=getRect();
	math::rectf tCurRect=tRect;
	float midX=10+(Value-MinValue)*(getWidth()-20)/(MaxValue-MinValue);
	tCurRect.ULPoint.x+=midX-10;
	tCurRect.BRPoint.x=tCurRect.ULPoint.x+20;
	tCurRect.ULPoint.y-=3;
	tCurRect.BRPoint.y+=3;

	static float i3=1.0f/3.0f;
	
	float yCoord=0;

	int state;
	switch (m_state){
		case MouseOff:
			yCoord=0.0f;
			state=0;
			break;
		case MouseOn:
			yCoord=i3;
			state=1;
			break;
		case MouseDown:
			yCoord=i3+i3;
			state=2;
			break;
	}
	math::rectf coordsRect(0,yCoord,1,yCoord+0.25f);


	video::SColor bclr=Color();
	bclr.A=AlphaEffect ? Alpha:Color().A;
	
	video::SColor clr;
	if(!bkTex->GetTexture()){
		math::rectf rc=tRect;
		float height=tRect.getHeight();
		rc.ULPoint.y+=(height/2.0f)-2;
		rc.BRPoint.y=rc.ULPoint.y+4;
		skin->drawSizableBox3PH(getManager()->GetRenderQueue(),tRect,state,mT("SliderBar"),bclr,&clipRect);

		if(guiMngr->hasFocus(this))
		{
			math::rectf rc=tRect;
			rc.BRPoint-=4;
			rc.ULPoint+=4;
			guiMngr->getDevice()->draw2DRectangle(rc,80,0,&clipRect);
		}
	}
	else device->draw2DImage(tRect,bkTex,video::SColor(255,255,255,bclr.A),0);

	
	if(!crTex->GetTexture())
		skin->drawBox(getManager()->GetRenderQueue(),tCurRect,mT("SliderCursor"),state,bclr,&clipRect);
	else 
	{
		float h=tCurRect.getHeight();
		tCurRect.BRPoint=tCurRect.ULPoint+math::vector2d(crTex->GetTexture()->getSize().x,math::Min<float>(h,crTex->GetTexture()->getSize().y));
		tCurRect-=math::vector2d(tCurRect.getWidth()/2,0);
		
		float ratio=0.5f*((float)Value/(float)MaxValue)+0.5f;
		
		device->draw2DImage(tCurRect,crTex,video::SColor(255,255,255,bclr.A*ratio),0,&coordsRect);
	}


/*	if(!bkCurrTex)
		device->Draw2DRectangle(tChBox,Color,0,&clipRect);
	
	*/

		
	GUIElement::draw(dt);
}


GCPtr<GUIElement> GUISlider::getDuplicate()
{
	GCPtr<GUISlider>dup=guiMngr->addSlider(MinValue,MaxValue,getPos(),getSize(),m_event,getParent(),Color(),getId());
	
	dup->Value=Value;

	dup->crTex=crTex;
	dup->bkTex=bkTex;

	return dup;
}



}
}
