
#include "Polygon2D.h"


namespace mray{
namespace math{


Polygon2D::Polygon2D()
{

}
Polygon2D::~Polygon2D()
{
	clear();
}

void Polygon2D::addPoint(const math::vector2d &p)
{
	m_points.push_back(p);
	if(m_points.size()==0)
		m_boundingRect.reset(p);
	else 
		m_boundingRect.addPoint(p);
	
}
void Polygon2D::removePoint(size_t i)
{
	if(i>=getPointsCount())
		return;
	m_points.erase(m_points.begin()+i);
}
void Polygon2D::clear()
{
	m_points.clear();
}

const math::vector2d& Polygon2D::getPoint(size_t i)const
{
	if(i>=getPointsCount())
		return math::vector2d::Zero;
	return m_points[i];
}
size_t Polygon2D::getPointsCount()const
{
	return m_points.size();
}

const math::rectf& Polygon2D::getBoundingRect()const
{
	return m_boundingRect;
}
bool Polygon2D::isPointInside(const math::vector2d&p)const
{
	if(!m_boundingRect.IsPointInside(p))
		return false;
	size_t i1=m_points.size()-1;
//	bool neg=false,pos=false;
	for(size_t i=0;i<m_points.size();++i){
		if(p.getSide(m_points[i1],m_points[i])<0)
			return false;
	}
	return true;
}


}
}
