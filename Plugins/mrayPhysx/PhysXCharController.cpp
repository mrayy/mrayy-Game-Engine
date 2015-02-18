#include "stdafx.h"

#include "PhysXCharController.h"
#include "PhysXCCBoxInterface.h"
#include "PhysXCCCapsuleInterface.h"
#include "physXCommon.h"
#include "CPhysXNode.h"
#include "PhysXManager.h"

#include <NxController.h>

namespace mray{
namespace physics{


	class ControllerCallBack:public NxUserControllerHitReport
	{
		PhysXCharController* m_c;
	public:
		ControllerCallBack(PhysXCharController*c);

		virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit) ;
		virtual NxControllerAction  onControllerHit(const NxControllersHit& hit) ;

	};

ControllerCallBack::ControllerCallBack(PhysXCharController*c)
{
	m_c=c;
}

NxControllerAction  ControllerCallBack::onShapeHit(const NxControllerShapeHit& hit) {
	PhysicsCCCallBack* cb=m_c->getCallback();
	if(!cb){
		return NX_ACTION_NONE;
	}
	PhysXManager *p=(PhysXManager*)m_c->GetCreator();

	CControllerShapeHit h;
	h.controller=p->getControllerX(hit.controller);
	h.worldPos.set(hit.worldPos.x,hit.worldPos.y,hit.worldPos.z);
	h.worldNormal.set(hit.worldNormal.x,hit.worldNormal.y,hit.worldNormal.z);
	h.dir.set(hit.dir.x,hit.dir.y,hit.dir.z);
	h.id=hit.id;
	h.length=hit.length;

	if(cb->onShapeHit(h)==ECCA_Push)
		return NX_ACTION_PUSH;
	return NX_ACTION_NONE;
}
NxControllerAction  ControllerCallBack::onControllerHit(const NxControllersHit& hit) {
	PhysicsCCCallBack* cb=m_c->getCallback();
	if(!cb){
		return NX_ACTION_NONE;
	}

	PhysXManager *p=(PhysXManager*)m_c->GetCreator();

	CControllersHit h;
	h.controller=p->getControllerX(hit.controller);
	h.other=p->getControllerX(hit.other);

	if(cb->onControllerHit(h)==ECCA_Push)
		return NX_ACTION_PUSH;
	return NX_ACTION_NONE;
}


//////////////////////////////////////////////////////////////////////////

PhysXCharController::PhysXCharController(NxController *controller,IPhysicManager* creator)
{
	m_creator=creator;
	m_controller=controller;
	m_collisionFlags=ECCMF_COLLISION_SIDES|ECCMF_COLLISION_UP|ECCMF_COLLISION_DOWN;

	if(controller->getType()==NX_CONTROLLER_BOX)
		m_interface=new PhysXCCBoxInterface((NxBoxController*) controller);
	else m_interface=new PhysXCCCapsuleInterface((NxCapsuleController*)controller);

	m_callback=0;
	m_xcallback=new ControllerCallBack(this);
	m_actor=new CPhysXNode(m_controller->getActor(),creator);
}
PhysXCharController::~PhysXCharController(){
	m_actor=0;
	delete m_interface;
	if(m_xcallback)
		delete m_xcallback;
}



void PhysXCharController::setMovingDir(const math::vector3d &disp)
{
	m_movingDir=disp;
}
math::vector3d PhysXCharController::getMovingDir()
{
	return m_movingDir;
}
void PhysXCharController::setCollisionFlags(uint flags)
{
	m_collisionFlags=flags;
}
uint PhysXCharController::getCollisionFlags()
{
	return m_collisionFlags;
}


void PhysXCharController::setPosition(const math::vector3d &pos){

	NxVec3 v=ToNxVec3(pos);
	NxExtendedVec3 ev(v.x,v.y,v.z);

	m_controller->setPosition(ev);
}

math::vector3d PhysXCharController::getPosition(){
	NxExtendedVec3 v=m_controller->getPosition();
	return math::vector3d(v.x,v.y,v.z);
}

math::vector3d PhysXCharController::getFilteredPosition(){
	NxExtendedVec3 v=m_controller->getFilteredPosition();
	return ToVec3(v);
}

GCPtr<IPhysicalNode> PhysXCharController::getActor(){
	
	return m_actor;
}

void PhysXCharController::setStepOffset(float offset){
	m_controller->setStepOffset(offset);
}

void PhysXCharController::setCollision(bool e){
	m_controller->setCollision(e);
}

void PhysXCharController::setInteraction(ECCInteractionFlag e){

	NxCCTInteractionFlag f=NXIF_INTERACTION_INCLUDE;
	if(e==ECCI_Include)
		f=NXIF_INTERACTION_INCLUDE;
	else if(e==ECCI_Exclude)
		f=NXIF_INTERACTION_EXCLUDE;

	m_controller->setInteraction(f);
}

ECCInteractionFlag PhysXCharController::getInteraction(){
	NxCCTInteractionFlag f=m_controller->getInteraction();

	ECCInteractionFlag c=ECCI_Include;
	if(f==NXIF_INTERACTION_INCLUDE)
		c=ECCI_Include;
	else if(f==NXIF_INTERACTION_EXCLUDE)
		c=ECCI_Exclude;
	return c;
}

ECCType PhysXCharController::getType(){
	NxControllerType t= m_controller->getType(); 
	if(t==NX_CONTROLLER_BOX)
		return ECCT_Box;
	return ECCT_Capsule;
}


void PhysXCharController::update(float dt)
{
	m_controller->move(ToNxVec3(m_movingDir)*dt,0xFFFFFFFF, 0.001, m_collisionFlags);
}

}
}

