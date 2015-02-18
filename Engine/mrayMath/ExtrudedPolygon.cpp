
#include "ExtrudedPolygon.h"


namespace mray
{
namespace math
{


ExtrudedPolygon::ExtrudedPolygon()
:m_height(0),m_min(0)
{
}
ExtrudedPolygon::~ExtrudedPolygon()
{
}

void ExtrudedPolygon::setHeight(float h)
{
	m_height=h;
	m_boundingBox.MaxP.y=m_boundingBox.MinP.y+m_height;
}
void ExtrudedPolygon::setMin(float m)
{
	m_min=m;
	m_boundingBox.MinP.y=m;
	m_boundingBox.MaxP.y=m+m_height;
}

void ExtrudedPolygon::addPoint(const math::vector3d &p)
{
	if(p.y<m_min)
	{
		setHeight(m_height+(m_min-p.y));
		setMin(p.y);
	}
	else if(p.y>m_min+m_height)
		setHeight(p.y-m_min);
	Polygon2D::addPoint(math::vector2d(p.x,p.z));
}
const math::box3d& ExtrudedPolygon::getBoundingBox()const
{
	return m_boundingBox;
}

bool ExtrudedPolygon::isPointInside(const math::vector3d&p)const
{
	if(p.y<m_min || p.y>m_height)
		return false;
	return Polygon2D::isPointInside(math::vector2d(p.x,p.z));
}




}
}
