#include "stdafx.h"

#include "IPOForce.h"


namespace mray{
namespace physics{

IPOForce::IPOForce(math::vector3d force){
	m_force=force;
}
IPOForce::~IPOForce(){

}

void IPOForce::affect(IPhysicObject*o,float dt){
	int cnt=o->getVertexCount();
	PhysicalVertex*vert=o->getVerticies();
	for (int i=0;i<cnt;++i)
	{
		vert[i].force+=m_force;
	}
}

}
}