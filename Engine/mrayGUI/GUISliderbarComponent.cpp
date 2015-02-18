

#include "stdafx.h"
#include "GUISliderbarComponent.h"

#include "IGUIElement.h"
#include "IGUIManager.h"
#include "IGUITheme.h"



namespace mray
{
namespace GUI
{

	const int GUISliderbarComponent::g_scrollSize=20;

GUISliderbarComponent::GUISliderbarComponent()
:m_direction(EVertical),m_lastMousePos(0),m_scrollOn(0),m_value(0),m_minVal(0),m_maxVal(100),
listener(0), itemsCount(0), pageSize(0), m_botButState(0), m_upButState(0), m_startPos(0), m_drawScroll(true)
{
}

GUISliderbarComponent::~GUISliderbarComponent()
{
}


void GUISliderbarComponent::GetScrollBarRects(const math::rectf& innerRect,
											math::rectf &topArrow,math::rectf &botArrow,
											math::rectf &scrollerBG,math::rectf &scrollerBar)
{
	topArrow.ULPoint=innerRect.ULPoint;
	float v=(m_value-m_minVal)/(m_maxVal-m_minVal);
	IGUITheme* skin=owner->GetCreator()->GetActiveTheme();
	if(m_direction==EVertical)
	{
		topArrow.BRPoint.x=innerRect.BRPoint.x;
		topArrow.BRPoint.y=topArrow.ULPoint.y+g_scrollSize;


		scrollerBG.ULPoint.x=innerRect.ULPoint.x;
		botArrow.ULPoint.x=innerRect.ULPoint.x;
		scrollerBar.ULPoint.x=innerRect.ULPoint.x;

		scrollerBG.BRPoint.x=innerRect.BRPoint.x;
		botArrow.BRPoint.x=innerRect.BRPoint.x;
		scrollerBar.BRPoint.x=innerRect.BRPoint.x;

		scrollerBG.ULPoint.y=topArrow.BRPoint.y;
		scrollerBG.BRPoint.y=innerRect.BRPoint.y-g_scrollSize;

		botArrow.ULPoint.y=scrollerBG.BRPoint.y;
		botArrow.BRPoint.y=botArrow.ULPoint.y+g_scrollSize;

		float length=scrollerBG.getHeight()*pageSize/itemsCount;
		float t=scrollerBG.getHeight()-length;

		scrollerBar.ULPoint.y=scrollerBG.ULPoint.y + (t * v);
		scrollerBar.BRPoint.y=scrollerBar.ULPoint.y + length;
	}else
	{
		topArrow.BRPoint.x=innerRect.ULPoint.x+g_scrollSize;

		scrollerBG.ULPoint.y=innerRect.ULPoint.y;
		botArrow.ULPoint.y=innerRect.ULPoint.y;
		scrollerBar.ULPoint.y=innerRect.ULPoint.y;

		topArrow.BRPoint.y=innerRect.BRPoint.y;
		scrollerBG.BRPoint.y=innerRect.BRPoint.y;
		botArrow.BRPoint.y=innerRect.BRPoint.y;
		scrollerBar.BRPoint.y=innerRect.BRPoint.y;

		scrollerBG.ULPoint.x=topArrow.BRPoint.x;
		scrollerBG.BRPoint.x=innerRect.BRPoint.x-g_scrollSize;

		botArrow.ULPoint.x=scrollerBG.BRPoint.x;
		botArrow.BRPoint.x=botArrow.ULPoint.x+g_scrollSize;

		float length=scrollerBG.getWidth()*pageSize/itemsCount;
		float t=scrollerBG.getWidth()-length;

		scrollerBar.ULPoint.x=scrollerBG.ULPoint.x + (t * v);
		scrollerBar.BRPoint.x=scrollerBar.ULPoint.x + length;
	}

}
void GUISliderbarComponent::Draw()
{
	IGUIManager* creator=owner->GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();

	math::rectf topArrow,botArrow;
	math::rectf scrollerBG,scrollerBar;

	if (m_drawScroll)
	{
		GetScrollBarRects(rc, topArrow, botArrow, scrollerBG, scrollerBar);

		skin->drawBox(creator->GetRenderQueue(), scrollerBG, mT("ScrollVBG"), 0, video::DefaultColors::White);
		skin->drawBox(creator->GetRenderQueue(), topArrow, mT("ScrollVCursor"), 0 + 2 * m_upButState, video::DefaultColors::White);
		skin->drawBox(creator->GetRenderQueue(), botArrow, mT("ScrollVCursor"), 1 + 2 * m_botButState, video::DefaultColors::White);
		skin->drawSizableBox3PV(creator->GetRenderQueue(), scrollerBar, m_scrollOn ? 1 : 0, mT("ScrollV"));
		if (m_upButState == 2)
			m_upButState = 1;
		if (m_botButState == 2)
			m_botButState = 1;
	}
}

void GUISliderbarComponent::_changeValue(float amount)
{
	m_value+=amount*(m_maxVal-m_minVal)*pageSize/itemsCount;
	if(m_value<m_minVal)m_value=m_minVal;
	if(m_value>m_maxVal)m_value=m_maxVal;

	if(listener)
		listener->OnValueChanged(this);
}

void GUISliderbarComponent::SetValue(float v)
{
	m_value=v;
	m_value=math::clamp(m_value,m_minVal,m_maxVal);
	if(listener && m_value!=v)
		listener->OnValueChanged(this);
}
void GUISliderbarComponent::SetMinValue(float v)
{
	m_minVal=v;
	if(m_minVal>m_maxVal)
		m_maxVal=m_minVal;
	v=math::clamp(m_value,m_minVal,m_maxVal);
	if(listener && m_value!=v)
	{
		m_value=v;
		listener->OnValueChanged(this);
	}
}
void GUISliderbarComponent::SetMaxValue(float v)
{
	m_maxVal=v;
	if(m_minVal>m_maxVal)
		m_maxVal=m_minVal;
	v=math::clamp(m_value,m_minVal,m_maxVal);
	if(listener && m_value!=v)
	{
		m_value=v;
		listener->OnValueChanged(this);
	}
}
bool GUISliderbarComponent::OnKeyboardEvent(KeyboardEvent* e)
{
	return false;
}

bool GUISliderbarComponent::OnMouseEvent(MouseEvent* e)
{
	if (!m_drawScroll)
		return false;

	if(itemsCount<=0)
		return false;
	math::rectf topArrow,botArrow;
	math::rectf scrollerBG,scrollerBar;

	bool res=false;
	switch(e->event)
	{
	case MET_LEFTDOWN:
		{
			if(!m_scrollOn && rc.IsPointInside(e->pos))
			{
				GetScrollBarRects(rc,topArrow,botArrow,scrollerBG,scrollerBar);

				if(topArrow.IsPointInside(e->pos))
				{
					_changeValue(-1);
					m_scrollOn=false;
					m_upButState=2;
				}else if(botArrow.IsPointInside(e->pos))
				{
					_changeValue(1);
					m_scrollOn=false;
					m_botButState=2;
				}else if(scrollerBar.IsPointInside(e->pos))
				{
					m_scrollOn=!m_scrollOn;
					if(m_direction== EVertical)
					{
						m_lastMousePos=e->pos.y;
						m_startPos=scrollerBar.ULPoint.y;
					}
					else
					{
						m_lastMousePos=e->pos.x;
						m_startPos=scrollerBar.ULPoint.x;
					}
				}
				e->setRecived();
				res=true;
			}
		}
		break;
	case MET_LEFTUP:
		{
			m_scrollOn=false;
	//		e->setRecived();
	//		res=true;
		}break;
	case MET_MOVED:

		if(m_scrollOn)
		{
			GetScrollBarRects(rc,topArrow,botArrow,scrollerBG,scrollerBar);
			float dpos=0;
			if(m_direction== EVertical)
			{
				dpos=e->pos.y-m_lastMousePos;
				scrollerBar.ULPoint.y=m_startPos+dpos;
			}
			else
			{
				dpos=e->pos.x-m_lastMousePos;
				scrollerBar.ULPoint.x=m_startPos+dpos;
			}
			//m_lastMousePos=e->pos.y;
			float v=0;

			if(m_direction== EVertical)
				v=(scrollerBar.ULPoint.y-scrollerBG.ULPoint.y)*itemsCount/scrollerBG.getHeight();
			else v=(scrollerBar.ULPoint.x-scrollerBG.ULPoint.x)*itemsCount/scrollerBG.getWidth();
			if(v<0)v=0;
			if(v>itemsCount-pageSize)
				v=itemsCount-pageSize;
			if(itemsCount==pageSize)
				m_value=m_minVal;
			else
				m_value=m_minVal+(m_maxVal-m_minVal)*v/(itemsCount-pageSize);

			if(listener)
				listener->OnValueChanged(this);
			res=true;
			e->setRecived();
		}else
		{
			if(topArrow.IsPointInside(e->pos))
			{
				m_upButState=1;
			}else
				m_upButState=0;
			if(botArrow.IsPointInside(e->pos))
			{
				m_botButState=1;
			}else
				m_botButState=0;
		}
	}
	return res;
}


void GUISliderbarComponent::LostFocus()
{
}

void GUISliderbarComponent::LostMouseOn()
{
}


}
}