#include "stdafx.h"

#include "AIObstacleCircle.h"
#include "XMLElement.h"
#include "mmath.h"
#include "StringConverter.h"

namespace mray
{
namespace AI
{


AIObstacleCircle::AIObstacleCircle(float R)
:m_radius(R)
{
}
AIObstacleCircle::~AIObstacleCircle()
{
}

void AIObstacleCircle::SetRadius(float r)
{
	m_radius=r;
}
float AIObstacleCircle::GetRadius()
{
	return m_radius;
}

bool AIObstacleCircle::IntersectWithLine(const math::line2d& l,math::vector2d*interPoint)
{
	float dx,dy,dr,D;
	float delta;
	dx=l.pEnd.x-l.pStart.x;
	dy=l.pEnd.y-l.pStart.y;
	dr=(dx*dx+dy*dy);

	D=l.pStart.x*l.pEnd.y-l.pEnd.x*l.pStart.y;
	
	delta=(m_radius*m_radius)*dr-D*D;
	if(delta<0)
		return false;
	if(interPoint){
		if(delta<math::Epsilon)
		{
			(*interPoint).x=D*dy/dr;
			(*interPoint).y=-D*dx/dr;
		}else
		{
			math::vector2d p1,p2;
			delta=sqrt(delta);
			p1.x=(D*dy-math::sign(dy)*dx*delta)/dr;
			p1.y=(-D*dx-fabs(dy)*delta)/dr;

			p2.x=(D*dy+math::sign(dy)*dx*delta)/dr;
			p2.y=(-D*dx+fabs(dy)*delta)/dr;

			if(p1.getDistSQ(l.pStart)<p2.getDistSQ(l.pStart))
				(*interPoint)=p1;
			else
				(*interPoint)=p2;
		}
		float t=0;
		if((*interPoint).x!=l.pStart.x)
		{
			t=((*interPoint).x-l.pStart.x)/(l.pEnd.x-l.pStart.x);
		}else if((*interPoint).y!=l.pStart.y)
		{
			t=((*interPoint).y-l.pStart.y)/(l.pEnd.y-l.pStart.y);
		}
		if(t<0 || t>1)
			return false;
	}
	return true;
}

void AIObstacleCircle::GetShapeConvexPoints(std::vector<math::vector2d>& points)
{
	int numEdges=3+m_radius*2;
	if(numEdges>8)
		numEdges=8;


	points.resize(numEdges+1);

	float step=360.0f/numEdges;
	math::vector2d p;
	math::vector2d ptmp;
	float cs=math::cosd(step);
	float sn=math::sind(step);

	p.x=m_radius;
	p.y=0;

	for (int i=0;i<=numEdges;++i)
	{
		ptmp.x=p.x*cs-p.y*sn;
		ptmp.y=p.x*sn+p.y*cs;
		points[i]=ptmp;
		p=ptmp;
	}
}

void AIObstacleCircle::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr= elem->getAttribute(mT("radius"));
	if(attr)
		m_radius=core::StringConverter::toFloat(attr->value);
}


}
}
