

/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   17:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\CircleTriggerShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	CircleTriggerShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CircleTriggerShape___
#define ___CircleTriggerShape___

#include "ITriggerShape.h"
#include "AIActor.h"

namespace mray{
namespace AI{

class CircleTriggerShape:public ITriggerShape
{
private:
protected:
	float m_radius;
	math::vector2d m_pos;
	math::box3d m_bbox;
	void CalculateBBox()
	{
		m_bbox.reset(math::vector3d(m_pos.x,0,m_pos.y));
		m_bbox.AddPoint(math::vector3d(m_pos.x-m_radius,-1000,m_pos.y-m_radius));
		m_bbox.AddPoint(math::vector3d(m_pos.x+m_radius,1000,m_pos.y+m_radius));
	}
public:
	CircleTriggerShape(float r,const math::vector2d& p):m_radius(r),m_pos(p)
	{
		CalculateBBox();
	}
	virtual~CircleTriggerShape(){}

	void setRadius(float r){
		m_radius=r;
		CalculateBBox();
	}
	float getRadius(){return m_radius;}

	void setPosition(const math::vector2d& p){
		m_pos=p;
		CalculateBBox();
	}
	const math::vector2d& setPosition(){return m_pos;}

	virtual const math::box3d& GetAABB(){return m_bbox;}
	virtual bool IsActorInside(AIActor*actor)
	{
		math::vector3d p=actor->GetPosition();
		if(math::vector2d(p.x-m_pos.x,p.z-m_pos.y).LengthSqr()<=m_radius*m_radius)
			return true;
		return false;
	}

};

}
}


#endif //___CircleTriggerShape___
