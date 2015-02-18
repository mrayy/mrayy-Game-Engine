

#include "stdafx.h"
#include "GUIPropGridStack.h"
#include "GUIPropertyGrid.h"
#include "IGUIPropGridItem.h"
#include "IGUIPanelElement.h"


namespace mray
{
namespace GUI
{

GUIPropGridStack::GUIPropGridStack(GUIPropertyGrid* grid)
:IGUIElement(GUID(""),grid->GetCreator()),m_grid(grid),m_activeItem(0),m_isActive(false),
m_height(0)
{
}

GUIPropGridStack::~GUIPropGridStack()
{
	Clear();
}


void GUIPropGridStack::AddProperty(IGUIPropGridItem* itm)
{
	m_items.push_back(itm);
}

void GUIPropGridStack::Clear()
{
	PropItemsList::iterator it=m_items.begin();
	for(;it!=m_items.end();++it)
	{
		delete *it;
	}
	m_items.clear();
}

bool GUIPropGridStack::_UpdateRegion(const math::rectf*vp)
{
	if(IGUIElement::_UpdateRegion(vp))
	{
		math::rectf rc=m_defaultRegion->GetRect();
		rc.BRPoint.y=rc.ULPoint.y+m_height;
		m_defaultRegion->SetRect(rc);
		if(m_attachedRegion)
			m_defaultRegion->SetClipRect(m_attachedRegion->GetClippedRect());
		return true;
	}
	return false;
}

void GUIPropGridStack::Draw(const math::rectf*vp)
{
	math::rectf clipRect;
	//if(m_attachedRegion)
	//	clipRect=m_attachedRegion->GetClippedRect();
//	else
		clipRect=GetDefaultRegion()->GetClippedRect();

	GUI::IFont* font=gFontResourceManager.getFontByName(m_grid->GetFontAttributes()->fontName);
	float dimY=font->getTextDimension(core::string(mT("A")),m_grid->GetFontAttributes(),&clipRect).y *1.5;

	math::rectf rc(clipRect);

	rc.BRPoint.y=rc.ULPoint.y+dimY;

	m_size.x=m_grid->GetSize().x;
	float maxY=m_grid->GetDefaultRegion()->GetClippedRect().BRPoint.y;
	//float maxY=GetDefaultRegion()->GetClippedRect().BRPoint.y;

	float h=0;
	PropItemsList::iterator it=m_items.begin();
	for(;it!=m_items.end();++it)
	{
		math::rectf r=(*it)->Draw(rc);
		h+=r.getHeight();
		rc.ULPoint.y+=r.getHeight();
		rc.BRPoint.y+=r.getHeight();
		if(rc.BRPoint.y>maxY)
			break;
	}
	if(h!=m_height)
	{
		m_height=h;
		m_size.y=m_height;
		m_unclippedRectDirt=true;
	}
}

bool GUIPropGridStack::_OnMouseEvent(MouseEvent*e)
{
	PropItemsList::iterator it=m_items.begin();
	for(;it!=m_items.end();++it)
	{
		if((*it)->HandleMouseEvent(e))
		{
			if(m_activeItem)
				m_activeItem->SetActive(false);
			m_activeItem=*it;
			m_activeItem->SetActive(true);
			m_isActive=true;
			return true;
		}
	}
	if(e->event==MET_LEFTDOWN)
	{
		if(m_activeItem)
			m_activeItem->SetActive(false);
		m_activeItem=0;
		m_isActive=false;
	}
	return false;
}

bool GUIPropGridStack::_OnKeyboardEvent(KeyboardEvent*e)
{
	if(m_activeItem)
		return m_activeItem->HandleKeyboardEvent(e);
	return false;
}



}
}



