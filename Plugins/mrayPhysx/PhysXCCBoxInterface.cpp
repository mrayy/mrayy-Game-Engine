#include "stdafx.h"

#include "PhysXCCBoxInterface.h"
#include "physXCommon.h"


namespace mray{
namespace physics{


PhysXCCBoxInterface::PhysXCCBoxInterface(NxBoxController*c){
	m_c=c;
}
PhysXCCBoxInterface::~PhysXCCBoxInterface(){

}

void PhysXCCBoxInterface::setExtents(const math::vector3d& e){
	m_c->setExtents(ToNxVec3(e));
}
math::vector3d PhysXCCBoxInterface::getExtents(){
	const NxVec3& v=m_c->getExtents();
	return ToVec3(v);
}


}
}