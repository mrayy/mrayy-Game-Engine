

#include "stdafx.h"
#include "GUISliderBar.h"
#include "GUIManager.h"
#include "IGUITheme.h"


namespace mray
{
namespace GUI
{
	IMPLEMENT_ELEMENT_FACTORY(GUISliderBar);

GUISliderBar::GUISliderBar(IGUIManager* creator)
	:IGUISliderBar(creator),m_value(0),m_minValue(0),m_maxValue(10),m_direction(ESBDir_Horizontal)
{
}
GUISliderBar::~GUISliderBar()
{
}

bool GUISliderBar::_OnKeyboardEvent(KeyboardEvent* e)
{
	if(!e->press)
		return false;
	math::rectf rc=GetDefaultRegion()->GetClippedRect();
	if(GetCreator()->HasFocus(this))
	{
		float step=(GetMaxValue()-GetMinValue())/rc.getWidth();
		float change=0;
		if(m_direction==ESBDir_Horizontal )
		{
			if(e->key==KEY_LEFT)
				change-=step;
			else if(e->key==KEY_RIGHT)
				change+=step;
		}
		if(m_direction==ESBDir_Vertical )
		{
			if(e->key==KEY_UP)
				change-=step;
			else if(e->key==KEY_DOWN)
				change+=step;
		}
		if(change!=0)
		{
			SetValue(GetValue()+change);
			return true;
		}
	}
	return false;
}
bool GUISliderBar::_OnMouseEvent(MouseEvent* e)
{
	bool ret=false;
	math::rectf rc=GetDefaultRegion()->GetClippedRect();
	if(rc.IsPointInside(e->pos))
	{
		GetCreator()->SetMouseOn(this);
	}
	if(e->event==MET_LEFTDOWN)
	{
		if(rc.IsPointInside(e->pos) || m_ticketRect.IsPointInside(e->pos))
		{
			GetCreator()->SetFocus(this);
			if(m_ticketRect.IsPointInside(e->pos))
				m_ticketMover.Enable(m_ticketRect.getCenter(),e->pos);
			ret=true;
		}
		else
			GetCreator()->SetFocus(0);
	}else if(e->event==MET_LEFTUP)
	{
		m_ticketMover.Disable();
	}else  if(e->event==MET_MOVED)
	{
		if(m_ticketMover.IsEnabled())
		{
			math::vector2d pos= m_ticketMover.OnMouseMove(e->pos);
			float v=0;
			if(m_direction==ESBDir_Horizontal)
			{
				if(pos.x<rc.ULPoint.x)
					pos.x=rc.ULPoint.x;
				else if(pos.x>rc.BRPoint.x)
					pos.x=rc.BRPoint.x;
				v=m_minValue+ (m_maxValue-m_minValue)*(pos.x-rc.ULPoint.x)/rc.getWidth();
				SetValue(v);
			}else
			{
				if(pos.y<rc.ULPoint.y)
					pos.y=rc.ULPoint.y;
				else if(pos.y>rc.BRPoint.y)
					pos.y=rc.BRPoint.y;
				v=m_minValue+ (m_maxValue-m_minValue)*(pos.y-rc.ULPoint.y)/rc.getHeight();
				SetValue(v);
			}
			ret=true;
		}
	}
	return ret;

	return true;//m_component->OnMouseEvent(e);
}
bool GUISliderBar::SetDirection(ESliderBarDirection val)
{
	m_direction=val;
	return true;
}
ESliderBarDirection GUISliderBar::GetDirection()const
{
	return m_direction;
}

bool GUISliderBar::SetValue(float val)
{
	m_value=val;
	m_value=math::clamp(m_value,m_minValue,m_maxValue);
	if(m_value!=val)
		OnChanged(this,(void*)&m_value);
	return true;
}
float GUISliderBar::GetValue()const
{
	return m_value;
}

bool GUISliderBar::SetMaxValue(float val)
{
	m_maxValue=val;
	if(m_minValue>m_maxValue)
		m_minValue=m_maxValue;
	val=math::clamp(m_value,m_minValue,m_maxValue);
	if(m_value!=val)
		OnChanged(this,(void*)&m_value);
	return true;
}
float GUISliderBar::GetMaxValue()const
{
	return m_maxValue;
}

bool GUISliderBar::SetMinValue(float val)
{
	m_minValue=val;
	if(m_minValue>m_maxValue)
		m_maxValue=m_minValue;
	val=math::clamp(m_value,m_minValue,m_maxValue);
	if(m_value!=val)
		OnChanged(this,(void*)&m_value);
	return true;
}
float GUISliderBar::GetMinValue()const
{
	return m_minValue;
}
void GUISliderBar::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;
	math::rectf rc=GetDefaultRegion()->GetClippedRect();
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();

	const float m_ticketSize=20;
	float posRatio=(m_value-m_minValue)/(m_maxValue-m_minValue);
	float centerPos=rc.ULPoint.x+ posRatio*rc.getWidth();

	m_ticketRect.ULPoint.y=rc.ULPoint.y;
	m_ticketRect.BRPoint.y=rc.BRPoint.y;
	m_ticketRect.ULPoint.x=centerPos-m_ticketSize*0.5f;
	m_ticketRect.BRPoint.x=centerPos+m_ticketSize*0.5f;

	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	GetCreator()->GetDevice()->setScissorRect(rc);

	skin->drawBox(creator->GetRenderQueue(),rc,mT("Frame"),0,video::DefaultColors::White);
	skin->drawBox(creator->GetRenderQueue(),m_ticketRect,mT("Frame"),0,video::DefaultColors::Gray);

	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);
}

IGUIElement* GUISliderBar::Duplicate()
{
	return new GUISliderBar(GetCreator());
}
FontAttributes* GUISliderBar::GetFontAttributes()
{
	return 0;
}

}
}