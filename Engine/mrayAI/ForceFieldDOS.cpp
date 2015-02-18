#include "stdafx.h"

#include "ForceFieldDOS.h"
#include "IAIPhysics.h"
#include "AIComponentTypes.h"
#include "IActorQuery.h"


namespace mray
{
namespace AI
{


ForceFieldDOS::ForceFieldDOS(AgentObstacle* agent,IActorQuery* query):
	m_agent(agent),m_query(query)
{

}

ForceFieldDOS::~ForceFieldDOS()
{

}



void ForceFieldDOS::CalculateAvoidanceForce()
{
	//consider all obstacles as force generators,generating force against the agent
	math::vector3d force;
	math::vector3d pos=m_agent->GetPhysics()->GetGlobalPosition();
	float radius=m_agent->GetRadius();

	const ObstacleList& obstacles=m_query->GetObstaclesList();
	int n=0;
	ObstacleList::const_iterator it=obstacles.begin();
	for (;it!=obstacles.end();++it)
	{
		const AgentObstacle& ob=**it;
		//more near the obstacle,the larger the force it generates
		math::vector3d dir=pos-ob.GetPhysics()->GetGlobalPosition();
		float len=dir.Length();
		float r=ob.GetRadius()+radius;

		//	if(len>r*1.5)//is it too far away
		//		continue;
		//dir/=len;
		len-=r;
		dir*=1/(len*len);
		force+=dir;
		++n;
	}
	force.Normalize();
	//return force;
}


}
}
