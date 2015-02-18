
#include "stdafx.h"
#include "ICShape.h"
#include "CanvasUtilities.h"
#include "ICanvasObject.h"
#include "CanvasManager.h"


namespace mray
{
	class MouseEvent;

namespace canvas
{

ICShape::ICShape()
{
}
ICShape::~ICShape()
{
}

void ICShape::Draw(EMouseStatus status)
{
	if(!m_owner)
		return;
	video::SColor clr(0,0,0,1);
	float thickness=1;
	if(status==EHovered || status==EClicked)
		thickness=2;
	if(status==EClicked)
	{
		clr.Set(1,0,0,1);
	}

	m_owner->GetCanvasManager()->DrawShapeWired(m_points,clr,thickness);
}

ICShape::EMouseResult ICShape::ProcessMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	return ENone;
}
void ICShape::SetShapePoints(const PointList &points)
{
	m_points=points;
	m_boundingRect=CanvasUtilities::CalculateBoundingRect(m_points);
}
const PointList& ICShape::GetShapePoints()
{
	return m_points;
}
const math::rectf& ICShape::GetBoundingRect()
{
	return m_boundingRect;
}

bool ICShape::IsPointIn(const math::vector2d& p)
{
	if(!m_boundingRect.IsPointInside(p))
		return false;
	size_t i1=m_points.size()-1;
	for(size_t i=0;i<m_points.size();++i)
	{
		if(p.getSide(m_points[i1],m_points[i])>0)
			return false;
		i1=i;
	}
	return true;
}

}
}

