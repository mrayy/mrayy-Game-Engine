
#include "stdafx.h"
#include "PhysXCCCapsuleInterface.h"


namespace mray{
namespace physics{

PhysXCCCapsuleInterface::PhysXCCCapsuleInterface(NxCapsuleController*c){
	m_c=c;
}
PhysXCCCapsuleInterface::~PhysXCCCapsuleInterface(){
}

void PhysXCCCapsuleInterface::setRadius(float r ){
	m_c->setRadius(r);
}
float PhysXCCCapsuleInterface::getRadius(){
	return m_c->getRadius();
}

void PhysXCCCapsuleInterface::setHeight(float h ){
	m_c->setHeight(h);
}
float PhysXCCCapsuleInterface::getHeight(){
	return m_c->getHeight();
}

void PhysXCCCapsuleInterface::setClimbing(ECCCapsuleClimbingMode c ){
	NxCapsuleClimbingMode m;
	if(c==ECCCM_Constrained)
		m=CLIMB_CONSTRAINED;
	else m=CLIMB_EASY;
	m_c->setClimbingMode(m);
}
ECCCapsuleClimbingMode PhysXCCCapsuleInterface::getClimbing(){
	NxCapsuleClimbingMode m=m_c->getClimbingMode();
	if(m==CLIMB_CONSTRAINED)
		return ECCCM_Constrained;
	else return  ECCCM_Easy;
}

}
}
#include "stdafx.h"
