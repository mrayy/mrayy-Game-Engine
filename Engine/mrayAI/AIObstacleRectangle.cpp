#include "stdafx.h"

#include "AIObstacleRectangle.h"
#include "XMLElement.h"
#include "mmath.h"
#include "StringConverter.h"


namespace mray
{
namespace AI
{


AIObstacleRectangle::AIObstacleRectangle(float w,float h)
:m_width(w),m_height(h)
{
}
AIObstacleRectangle::~AIObstacleRectangle()
{
}

void AIObstacleRectangle::SetWidth(float v)
{
	m_width=v;
}
float AIObstacleRectangle::GetWidth()
{
	return m_width;
}

void AIObstacleRectangle::SetHeight(float v)
{
	m_height=v;
}
float AIObstacleRectangle::GetHeight()
{
	return m_height;
}

bool AIObstacleRectangle::IntersectWithLine(const math::line2d& l,math::vector2d*interPoint)
{
	return false;
}

void AIObstacleRectangle::GetShapeConvexPoints(std::vector<math::vector2d>& points)
{
	points.resize(5);
	float w2=m_width*0.5;
	float h2=m_height*0.5;
	points[0].set(-w2,-h2);
	points[1].set( w2,-h2);
	points[2].set( w2, h2);
	points[3].set(-w2, h2);
	points[4].set(-w2,-h2);
}

void AIObstacleRectangle::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr= elem->getAttribute(mT("width"));
	if(attr)
		m_width=core::StringConverter::toFloat(attr->value);
	attr= elem->getAttribute(mT("height"));
	if(attr)
		m_height=core::StringConverter::toFloat(attr->value);
}


}
}

