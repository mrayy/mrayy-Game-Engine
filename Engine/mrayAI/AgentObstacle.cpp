#include "stdafx.h"


#include "AgentObstacle.h"
#include "AIComponentTypes.h"
#include "IRenderDevice.h"
#include "AIObstacleRectangle.h"
#include "AIObstacleCircle.h"
#include "IAIPhysics.h"
#include "DefaultColors.h"

namespace mray{
namespace AI{


AgentObstacle::AgentObstacle(IAIPhysics* ph):m_shape(0),m_physics(ph)
{
	//Radius=new rwProperty<AgentObstacle,float>(this,&AgentObstacle::SetRadius,&AgentObstacle::_GetRadius,EPT_FLOAT,mT("Radius"));
	//addPropertie(Radius);
}

AgentObstacle::~AgentObstacle()
{
	//delete Radius;
	delete m_shape;
}

IAIObstacleShape* AgentObstacle::GetShape()
{
	return m_shape;

}
void AgentObstacle::SetShape(IAIObstacleShape*shape)
{
	delete m_shape;
	m_shape=shape;
	m_obstaclePoints.clear();
	if(m_shape)
	{
		m_shape->GetShapeConvexPoints(m_obstaclePoints);
	}
}

/*
float& AgentObstacle::_GetRadius()
{
	return m_radius;
}
*/

void AgentObstacle::SetRadius(const float& r)
{
	m_radius=r;
}
float AgentObstacle::GetRadius()const
{
	return m_radius;
}

IAIPhysics* AgentObstacle::GetPhysics()const
{
	return m_physics;
}
void AgentObstacle::SetPhysics(IAIPhysics*ph)
{
	m_physics=ph;
}

bool AgentObstacle::IntersectWithLine(const math::line2d& line,math::vector2d*interPoint)
{
	math::line2d l(line);
	math::vector3d pos=GetPhysics()->GetGlobalPosition();
	l.pStart-=math::vector2d(pos.x,pos.z);
	l.pEnd-=math::vector2d(pos.x,pos.z);
	return m_shape->IntersectWithLine(l,interPoint);
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
		(*interPoint).x+=pos.x;
		(*interPoint).y+=pos.z;

	}
	return true;
}

void AgentObstacle::Visualize(IRenderDevice*dev)
{
	if(!m_obstaclePoints.size())
		return;
	math::matrix4x4 m;
	m_physics->GetGlobalOrintation().toMatrix(m);
	m.setTranslation(m_physics->GetGlobalPosition());

	math::line3d l;

	l.pStart.set(m_obstaclePoints[0].x,0,m_obstaclePoints[0].y);
	l.pStart=m*l.pStart;

	for (int i=1;i<m_obstaclePoints.size();++i)
	{
		l.pEnd.set(m_obstaclePoints[i].x,0,m_obstaclePoints[i].y);
		l.pEnd=m*l.pEnd;
		dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::White);
		l.pStart=l.pEnd;
	}
}

xml::XMLElement* AgentObstacle::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadXMLSettings(elem);
	xml::XMLElement*e= elem->getSubElement(mT("shape"));
	if(e)
	{
		xml::XMLAttribute*attr=e->getAttribute(mT("type"));
		if(attr){
			IAIObstacleShape*s=0;
			if(attr->value.equals_ignore_case(mT("circle"))){
				s=new AIObstacleCircle(1);
				s->loadXMLSettings(e);
			}else  if(attr->value.equals_ignore_case(mT("rectangle"))){
				s=new AIObstacleRectangle(1,1);
				s->loadXMLSettings(e);
			}
			if(s)
				SetShape(s);
		}
	}
	return elem;
}

}
}