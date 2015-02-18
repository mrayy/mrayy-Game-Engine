

#include "stdafx.h"
#include "DraggableItem.h"


namespace mray
{
namespace GUI
{


DraggableItem::DraggableItem():m_dragging(false),m_disableAxis(false,false)
{

}

DraggableItem::~DraggableItem()
{
}


void DraggableItem::Enable(const math::vector2d& currPos,const math::vector2d& mousePos)
{
	m_startMousePos=mousePos;
	m_startPos=currPos;
	m_dragging=true;
}

void DraggableItem::Disable()
{
	m_dragging=false;
}

bool DraggableItem::IsEnabled()
{
	return m_dragging;
}

const math::vector2d& DraggableItem::UndoDrag()
{
	m_dragging=false;
	return m_startPos;
}

math::vector2d DraggableItem::OnMouseMove(const math::vector2d&pos)
{
	math::vector2d dpos=pos-m_startMousePos;
	if(m_disableAxis.x)dpos.x=m_startPos.x;
	if(m_disableAxis.y)dpos.y=m_startPos.y;
	return m_startPos+dpos;
}


}
}
