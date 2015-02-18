

#include "stdafx.h"
#include "GUITransitionBars.h"
#include "GUIElementRegion.h"
#include "IGUIManager.h"


namespace mray
{
namespace GUI
{


	const GUID GUITransitionBars::ElementType = "GUITransitionBars";

IMPLEMENT_RTTI(GUITransitionBars, IGUIElement);

GUITransitionBars::GUITransitionBars(IGUIManager* m)
:IGUIElement(ElementType, m)
{
	m_speed = 1500;
	m_time = 0;
	m_angle = 0;

	m_status = EIdle;
	m_color.Set(0, 0, 0, 1);

	m_barsCount = 10;

	for (int i = 0; i < m_barsCount; ++i)
	{
		BarInfo b;
		m_bars.push_back(b);
	}
}

GUITransitionBars::~GUITransitionBars()
{
}


void GUITransitionBars::Start(const video::SColor& targetColor)
{
	if (m_targetColor != targetColor)
	{
		m_color = m_targetColor;
		m_targetColor = targetColor;
		m_targetColor.A = GetAlpha();
		m_color.A = GetAlpha();
	}
	m_time = 0;
	m_angle = (math::Randomizer::rand() % (360 / 45)) * 45.0f;

	m_status = EPlaying;
	math::rectf rc = GetDefaultRegion()->GetClippedRect();
	float offset = 0;
	for (int i = 0; i < m_bars.size(); ++i)
	{
		float off = i / (float)m_bars.size();
		m_bars[i].width = rc.getWidth() / m_barsCount;
		m_bars[i].offset = -offset ;
		offset += 3*m_bars[i].width;
	}
}


void GUITransitionBars::Draw(const math::rectf*vp)
{
	if (!IsVisible())
		return;
	math::rectf rc = GetDefaultRegion()->GetClippedRect();
	math::rectf oldScissor = gEngine.getDevice()->getScissorRect();

	
	if (m_status == EPlaying)
	{
		/*
		math::matrix3x3 rot;
		rot.setAngle(m_angle);
		math::vector2d vec=rot*math::vector2d(1,0);
		float h = 1000;
		for (int i = 0; i < m_bars.size()-1; ++i)
		{
			float w = m_bars[i + 1].offset - m_bars[i].offset ;
			math::vector2d points[4];
			points[0] = rot*math::vector2d(-w, -h) + vec*m_bars[i].offset;
			points[1] = rot*math::vector2d(w, -h) + vec*m_bars[i].offset;
			points[2] = rot*math::vector2d(w, h) + vec*m_bars[i].offset;
			points[3] = rot*math::vector2d(-w, h) + vec*m_bars[i].offset;

			GetCreator()->GetDevice()->draw2DShape(points, 4, m_bars[i].clr, true);
		}
		*/

		for (int i = 0; i < m_bars.size() ; ++i)
		{

			math::rectf r;
			r.ULPoint.x = rc.ULPoint.x+ m_bars[i].offset;
			r.ULPoint.y = rc.ULPoint.y;
			r.BRPoint.x = rc.ULPoint.x + m_bars[i].offset + m_bars[i].width;
			r.BRPoint.y = rc.BRPoint.y;
			GetCreator()->GetDevice()->draw2DRectangle(r,m_targetColor);
		}
	}
	else{

		video::SColor c = m_color;
		c.A = GetAlpha();
		GetCreator()->GetDevice()->draw2DRectangle(rc, c);
	}
}


void GUITransitionBars::Update(float dt)
{
	if (m_status == EPlaying)
	{
		math::rectf rc = GetDefaultRegion()->GetClippedRect();
		m_time += dt;
		float w = 0;
		bool isDone = true;
		m_speed = rc.getWidth();
		for (int i = 0; i < m_bars.size(); ++i)
		{
			if (m_bars[i].offset + m_bars[i].width<rc.getWidth()-w)
			{
				m_bars[i].offset += m_speed*dt;
				if (m_bars[i].offset + m_bars[i].width > rc.getWidth() - w)
					m_bars[i].offset = (rc.getWidth() - w) - m_bars[i].width;
				else isDone = false;
			}
			w += m_bars[i].width;
		}
		if (isDone)
		{
			m_status = EIdle;
			m_color = m_targetColor;
			m_color.A = GetAlpha();
		}
	
		/*for (int i = 0; i < m_bars.size(); ++i)
		{
			m_bars[i].offset += m_speed*dt;
		}*/
	}
}

IMPLEMENT_ELEMENT_FACTORY(GUITransitionBars);

}
}


