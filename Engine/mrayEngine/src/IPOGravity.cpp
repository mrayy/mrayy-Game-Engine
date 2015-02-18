#include "stdafx.h"

#include "IPOGravity.h"


namespace mray{
namespace physics{

IPOGravity::IPOGravity(math::vector3d force){
	m_force=force;
}
IPOGravity::~IPOGravity(){

}

void IPOGravity::affect(IPhysicObject*o,float dt){
	int cnt=o->getVertexCount();
	PhysicalVertex*vert=o->getVerticies();
	for (int i=0;i<cnt;++i)
	{
		vert[i].force+=m_force*vert[i].getWeight();
	}
}

}
}