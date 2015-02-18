
#include "stdafx.h"
#include "GUICatagoryPanel.h"
#include "GUIStackPanel.h"

#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "FontResourceManager.h"
#include "IGUITheme.h"

namespace mray
{
namespace GUI
{


GUICatagoryPanel::GUICatagoryPanel(IGUIManager*mngr)
:IGUICatagoryPanel(mngr),m_tabHeight(30),m_contentHeight(300),m_isOpen(false),m_speed(500)
{
	m_panel=new GUIStackPanel(mngr);

	m_tabRegion=new GUIElementRegion(this);
	m_frameRegion=new GUIElementRegion(this);
	//m_scrollRegion=new GUIElementRegion(this);

	IGUIPanelElement::AddElement(m_panel,m_frameRegion);
	m_currentHeight=0;

	m_panel->SetDocking(EED_Fill);
	m_panel->SetStackDirection(IGUIStackPanel::EVertical);

}

GUICatagoryPanel::~GUICatagoryPanel()
{
	IGUIPanelElement::ClearElements();
	delete m_tabRegion;
	delete m_frameRegion;
	//delete m_scrollRegion;
}


void GUICatagoryPanel::AddElement(IGUIElementCRef elem)
{
	m_panel->AddElement(elem);
}

void GUICatagoryPanel::RemoveElement(IGUIElement* elem)
{
	m_panel->RemoveElement(elem);
}

IGUIElementCRef GUICatagoryPanel::GetElementByID(uint id)const
{
	return m_panel->GetElementByID(id);
}

void GUICatagoryPanel::ClearElements()
{
	m_panel->ClearElements();
}
bool GUICatagoryPanel::_UpdateRegion(const math::rectf*vp)
{
	if(IGUIElement::_UpdateRegion(vp))
	{
		//int m_scrollerWidth=25;
		const math::rectf& clipRect=GetDefaultRegion()->GetClippedRect();
		math::rectf m_tabRC;//=m_tabRegion->GetRect();
		math::rectf m_frameRect;//=m_frameRegion->GetRect();
		math::rectf m_scrollRect;//=m_scrollRegion->GetRect();

		m_tabRC.ULPoint=clipRect.ULPoint;
		m_tabRC.BRPoint.x=clipRect.BRPoint.x;
		m_tabRC.BRPoint.y=m_tabRC.ULPoint.y;
		m_tabRC.BRPoint.y+=math::Min(m_tabHeight,clipRect.getHeight());

		m_frameRect.ULPoint.x=clipRect.ULPoint.x;
		m_frameRect.ULPoint.y=m_tabRC.BRPoint.y;
		m_frameRect.BRPoint.x=clipRect.BRPoint.x;//-m_scrollerWidth;
		m_frameRect.BRPoint.y=m_tabRC.BRPoint.y+m_currentHeight;
/*
		m_scrollRect.ULPoint.x=m_frameRect.BRPoint.x;
		m_scrollRect.ULPoint.y=m_frameRect.ULPoint.y;
		m_scrollRect.BRPoint.x=clipRect.BRPoint.x;
		m_scrollRect.BRPoint.y=m_frameRect.ULPoint.y;
		m_scrollRegion->SetRect(m_scrollRect);
*/
		m_tabRegion->SetRect(m_tabRC);
		m_frameRegion->SetRect(m_frameRect);

		return true;
	}
	return false;
}
/*

const math::rectf& GUICatagoryPanel::GetUnclippedRect()
{
	if(m_clippedRectDirt || m_unclippedRectDirt)
	{
		const math::rectf& clipRect=IGUIElement::GetUnclippedRect();
		m_tabRC.ULPoint=clipRect.ULPoint;
		m_tabRC.BRPoint.x=clipRect.BRPoint.x;
		m_tabRC.BRPoint.y=m_tabRC.ULPoint.y;
		m_tabRC.BRPoint.y+=math::Min(m_tabHeight,clipRect.getHeight());
		
		m_frameRect.ULPoint.x=clipRect.ULPoint.x;
		m_frameRect.ULPoint.y=m_tabRC.BRPoint.y;
		m_frameRect.BRPoint.x=clipRect.BRPoint.x;
		m_frameRect.BRPoint.y=m_tabRC.BRPoint.y+m_currentHeight;
/ *
		IGUITheme* skin=GetCreator()->GetActiveTheme();
		if(skin)
		{
			m_frameRect=skin->getSizableRect(4,m_innerRect,0,"WindowFrame");
		}else
			m_frameRect=m_innerRect;* /
		m_unclippedRectDirt=false;
		m_clippedRectDirt=true;
	}
	return m_frameRect;
}*/
/*
const math::rectf& GUICatagoryPanel::GetClippedRect()
{
	if(m_clippedRectDirt)
	{
		IGUIElement::GetUnclippedRect();
		m_clippedRect=m_tabRC;
		m_clippedRect.BRPoint.y=m_frameRect.BRPoint.y;
	}
	return m_clippedRect;
}*/

void GUICatagoryPanel::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	GUI::IFont* font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);

	//GetUnclippedRect();//just to update rects
	_UpdateRegion(vp);
	
	math::rectf titleRC=m_tabRegion->GetClippedRect();
	/*math::rectf frameRC=m_frameRect;

	if(m_parent)
	{
		titleRC.clipRect(m_parent->GetClippedRect());
		frameRC.clipRect(m_parent->GetClippedRect());
	}*/

	if(skin)
	{
		skin->drawSizableBox3PH(creator->GetRenderQueue(),titleRC,0,mT("ComboList"));
		titleRC=skin->getRect3PH(1,titleRC,0,mT("ComboList"));
	}
	if(font)
		font->print(titleRC,&m_FontAttributes,&titleRC,m_text,creator->GetRenderQueue());
	if(skin)
	{
		skin->drawSizableBox(creator->GetRenderQueue(),m_frameRegion->GetClippedRect(),0,mT("Frame"));
	}
/*
	//update the rect
	if(m_status!=EIdle)
	{
		if(m_status==EClosing)
		{
			m_currentContHeight-=m_speed*gFPS.dt();
			m_currentContHeight=math::Max(0.0f,m_currentContHeight);
			if(m_currentContHeight==0)
			{
				m_status=EIdle;
				m_isOpen=false;
			}
		}else
		{
			if(m_currentContHeight==0)m_currentContHeight=1;
			m_currentContHeight+=m_speed*gFPS.dt();
			m_currentContHeight=math::Min(m_contentHeight,m_currentContHeight);
			if(m_currentContHeight==m_contentHeight)
			{
				m_status=EIdle;
				m_isOpen=true;
			}
		}
		m_size.y=m_currentContHeight+m_tabHeight;
		m_unclippedRectDirt=true;
	}*/

	if(/*m_status==EIdle && */m_isOpen==false)
		return;
	else
		IGUIPanelElement::Draw(vp);

	math::vector2d sz=m_panel->GetSize();
	if( sz.y<m_contentHeight && sz.y!=m_currentHeight && m_isOpen)
	{
		m_currentHeight=sz.y;
		m_size.y=m_tabHeight+m_currentHeight;
		m_unclippedRectDirt=true;
// 		m_size.y=sz.y;
// 		m_clippedRectDirt=true;
// 		m_unclippedRectDirt=true;
	}
}


IGUIElement* GUICatagoryPanel::Duplicate()
{
	return new GUICatagoryPanel(GetCreator());
}

FontAttributes* GUICatagoryPanel::GetFontAttributes()
{
	return &m_FontAttributes;
}

bool GUICatagoryPanel::_OnMouseEvent(MouseEvent*evt)
{
	if(evt->event==MET_LEFTDOWN && m_tabRegion->GetClippedRect().IsPointInside(evt->pos))
	{
		m_isOpen=!m_isOpen;
		if(!m_isOpen)
		{
			m_size.y=m_tabHeight;
			m_currentHeight=0;
		}else
		{
			m_currentHeight=m_contentHeight;
			m_size.y=m_tabHeight+m_currentHeight;
		}
		m_unclippedRectDirt=true;
		evt->setRecived();
		return true;
	}
	return IGUIPanelElement::_OnMouseEvent(evt);
}

IMPLEMENT_ELEMENT_FACTORY(GUICatagoryPanel);


}
}

