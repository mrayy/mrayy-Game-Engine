#include "stdafx.h"

#include "WorldSensor.h"
#include "INavWorld.h"
#include "IRenderDevice.h"
#include "DefaultColors.h"

#include "AIComponentTypes.h"

namespace mray
{
namespace AI
{


WorldSensor::WorldSensor(INavWorld* world):m_world(world),m_isCollide(false),m_distance(0)
{
	m_beginNode=0;
	m_endNode=0;
}

WorldSensor::~WorldSensor()
{
}

bool WorldSensor::SenseEnvironment()
{

	NavWorldNodeListPtr l;
	m_beginNode=m_world->QueryPolys(m_pos,l);
	m_endNode=m_world->QueryPolys(m_target,l);
	if(!m_beginNode ){
		m_isCollide=true;
		m_distance=0;
	}else{
		m_isCollide=!m_world->lineOfSightTest(m_pos,m_beginNode,m_target,m_endNode,&m_collision);
		if(m_isCollide)
			m_distance=m_collision.getDist(m_pos);
		else
			m_distance=0;
	}

	return m_isCollide;
}


bool WorldSensor::SenseEnvironmentAndDyn(const ObstacleList& obstacles)
{

	//1st,detect with the environment
	SenseEnvironment();
	//2nd,check dynamic obstacles

	math::line2d l(math::vector2d(m_pos.x,m_pos.z),math::vector2d(m_target.x,m_target.z));

	float bestDist=m_distance*m_distance;
	
	if(!m_isCollide)
		bestDist=l.LengthSQ();


	ObstacleList::const_iterator it=obstacles.begin();
	for (;it!=obstacles.end();++it)
	{
		AgentObstacle*o=*it;
		math::vector2d inter;
		if(o->IntersectWithLine(l,&inter)){
			m_isCollide=true;
			float len=inter.getDistSQ(l.pStart);
			if(bestDist>len){
				bestDist=len;
				m_distance=sqrt(len);
			}
		}
	}

	return m_isCollide;
}

bool WorldSensor::IsCollide()
{
	return m_isCollide;
}

float WorldSensor::GetCollisionDistance()
{
	return m_distance;
}
float WorldSensor::GetCollisionDistanceNorm()
{
	if(!m_isCollide)
		return 1;
	return m_distance/(m_target.getDist(m_pos));
}
const math::vector3d& WorldSensor::GetCollisionPoint()
{
	return m_collision;
}


void WorldSensor::SetPosition(const math::vector3d&pos)
{
	m_pos=pos;
}

const math::vector3d& WorldSensor::GetPosition()
{
	return m_pos;
}


void WorldSensor::SetTarget(const math::vector3d&pos)
{
	m_target=pos;
}

const math::vector3d& WorldSensor::GetTarget()
{
	return m_target;
}

void WorldSensor::Visualize(IRenderDevice*dev)
{
	float collideRatio=GetCollisionDistanceNorm();
	
	video::SColor c=video::DefaultColors::Green;
	dev->drawLine(m_pos,m_target,c.Interpolated(video::DefaultColors::Red,collideRatio));
}

}
}