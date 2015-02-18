#include "stdafx.h"

#include "SteerController.h"
#include "AIActor.h"
#include "IAIPhysics.h"
#include "AIComponentTypes.h"
#include <IPhysicalNode.h>
#include <Randomizer.h>
#include <EnumManager.h>

#include <MathUtil.h>

namespace mray{
namespace AI{

SteerController::SteerController(IAIPhysics* ph)
{
	m_targetActor=0;
	m_physics=ph;

	m_steeringFlags[EST_Seek]=0;
	m_steeringFlags[EST_Flee]=0;
	m_steeringFlags[EST_Arrive]=0;
	m_steeringFlags[EST_Pursuit]=0;
	m_steeringFlags[EST_Evade]=0;
	m_steeringFlags[EST_Hide]=0;
	m_steeringFlags[EST_Wander]=0;
	m_steeringFlags[EST_Seperation]=0;
	m_steeringFlags[EST_Align]=0;
	m_steeringFlags[EST_Cohesion]=0;

	m_weights[EST_Seek]=50;
	m_weights[EST_Flee]=0.5;
	m_weights[EST_Arrive]=1;
	m_weights[EST_Pursuit]=1;
	m_weights[EST_Evade]=1;
	m_weights[EST_Hide]=5;
	m_weights[EST_Wander]=1;
	m_weights[EST_Seperation]=10;
	m_weights[EST_Align]=5;
	m_weights[EST_Cohesion]=10;


	m_wanderJitter=1;
	m_wanderRadius=10;
	m_wanderDistance=5;
	float angle=math::Randomizer::rand01()*360.0f;
	m_wanderTarget.x=math::cosd(angle)*m_wanderRadius;
	m_wanderTarget.z=math::sind(angle)*m_wanderRadius;
}
SteerController::~SteerController(){
}


math::vector3d SteerController::GetTargetPos()
{
	if(!m_targetActor)
		return m_targetV;
	return m_targetActor->GetPhysicalProp()->GetGlobalPosition();
}

bool SteerController::isSteering(ESteeringTypes t){
	return m_steeringFlags[t];
}


void SteerController::seekOn(const math::vector3d&pos)
{
	m_steeringFlags[EST_Seek]=true;
	m_targetV=pos;
	m_targetActor=0;
}

void SteerController::seekOn(AIActor*agent){
	m_steeringFlags[EST_Seek]=true;
	m_targetActor=agent;
}
void SteerController::seekOff(){
	m_steeringFlags[EST_Seek]=0;
}

void SteerController::fleeOn(const math::vector3d&pos)
{
	m_steeringFlags[EST_Flee]=true;
	m_targetV=pos;
	m_targetActor=0;
}

void SteerController::fleeOn(AIActor*agent){
	m_steeringFlags[EST_Flee]=true;
	m_targetActor=agent;
}
void SteerController::fleeOff(){
	m_steeringFlags[EST_Flee]=0;
}

void SteerController::pursuitOn(const math::vector3d&pos)
{
	m_steeringFlags[EST_Pursuit]=true;
	m_targetV=pos;
	m_targetActor=0;
}

void SteerController::pursuitOn(AIActor*agent){
	m_steeringFlags[EST_Pursuit]=true;
	m_targetActor=agent;
}
void SteerController::pursuitOff(){
	m_steeringFlags[EST_Pursuit]=0;
}

void SteerController::evadeOn(const math::vector3d&pos)
{
	m_steeringFlags[EST_Evade]=true;
	m_targetV=pos;
	m_targetActor=0;
}
void SteerController::evadeOn(AIActor *agent){
	m_steeringFlags[EST_Evade]=true;
	m_targetActor=agent;
}
void SteerController::evadeOff(){
	m_steeringFlags[EST_Evade]=0;
}

void SteerController::hideOn(const math::vector3d&pos)
{
	m_steeringFlags[EST_Hide]=true;
	m_targetV=pos;
	m_targetActor=0;
}
void SteerController::hideOn(AIActor *agent){
	m_steeringFlags[EST_Hide]=true;
	m_targetActor=agent;
}
void SteerController::hideOff(){
	m_steeringFlags[EST_Hide]=0;
}

void SteerController::arriveOn(const math::vector3d&pos)
{
	m_steeringFlags[EST_Arrive]=true;
	m_targetV=pos;
	m_targetActor=0;
}
void SteerController::arriveOff(){
	m_steeringFlags[EST_Arrive]=0;
}

void SteerController::wanderOn(){
	m_steeringFlags[EST_Wander]=1;
}
void SteerController::wanderOff(){
	m_steeringFlags[EST_Wander]=0;
}

void SteerController::seperationOn(){
	m_steeringFlags[EST_Seperation]=1;
}
void SteerController::seperationOff(){
	m_steeringFlags[EST_Seperation]=0;
}

void SteerController::alignOn(){
	m_steeringFlags[EST_Align]=1;
}
void SteerController::alignOff(){
	m_steeringFlags[EST_Align]=0;
}

void SteerController::cohesionOn(){
	m_steeringFlags[EST_Cohesion]=1;
}
void SteerController::cohesionOff(){
	m_steeringFlags[EST_Cohesion]=0;
}


bool SteerController::accumulateForce(math::vector3d&target,const math::vector3d&force){
	float mag=target.Length();
	float magRemains=math::Max(0.0f,m_physics->GetMaxSpeed()-mag);
	if(magRemains<=0)
		return false;
	float magToAdd=force.Length();

	if(magToAdd<=magRemains)
	{
		target+=force;
	}else{
		target+=(force/magToAdd)*magRemains;
		return false;
	}
	return true;
}

math::vector3d SteerController::calculate(){
	math::vector3d force;
	m_steeringForce=0;

	if(isSteering(EST_Seperation)){
		//force=Seperation();
	}
	if(isSteering(EST_Seek)){
		math::vector3d t=GetTargetPos();
		force=seek(t)*m_weights[EST_Seek];
		if(!accumulateForce(m_steeringForce,force))
			return m_steeringForce;
	}
	if(isSteering(EST_Arrive)){
		force=arrive(GetTargetPos())*m_weights[EST_Arrive];
		if(!accumulateForce(m_steeringForce,force))
			return m_steeringForce;
	}
	if(isSteering(EST_Wander)){
		force=wander()*m_weights[EST_Wander];
		if(!accumulateForce(m_steeringForce,force))
			return m_steeringForce;
	}
	return m_steeringForce;
}

float SteerController::turnAroundTime(const math::vector3d&p){
	math::vector3d toTarget=p-m_physics->GetGlobalPosition();
	toTarget.Normalize();
	float d=m_physics->GetHeading().dotProduct(toTarget);
	const float coff=0.5;
	return (1-d)*-coff;
}

math::vector3d SteerController::seek(const math::vector3d&target){
	math::vector3d velocity=target-m_physics->GetGlobalPosition();
	velocity.Normalize();
	velocity*=m_physics->GetMaxSpeed();

	return velocity-m_physics->GetLinearVelocity();
}


math::vector3d SteerController::wander(){


	m_wanderTarget+=math::vector3d(	math::Randomizer::rand01(),
									math::Randomizer::rand01(),
									math::Randomizer::rand01())*m_wanderJitter;
	m_wanderTarget.Normalize();
	m_wanderTarget*=m_wanderRadius;
	math::vector3d target=m_wanderTarget+
		m_physics->GetGlobalOrintation()*(math::vector3d::ZAxis*m_wanderDistance);

	return target;
}

math::quaternion SteerController::align(AIActorList& target){
	math::quaternion averageHeading;
	int neighborCnt=0;
	AIActorList::iterator it=target.begin();
	for(;it!=target.end();++it){
		AIActor* a=*it;
		if(!a->GetPhysicalProp())
			continue;
		if(a->GetPhysicalProp()!=m_physics){
			averageHeading+=a->GetHeading();
			neighborCnt++;
		}
	}
	if(neighborCnt>0){
		averageHeading=averageHeading-m_physics->GetHeading();
		averageHeading.Normalize();
	}
	return averageHeading;
}

math::vector3d SteerController::cohesion(AIActorList& target){
	math::vector3d centerOfMass,steeringForce;
	int neighborCnt=0;

	AIActorList::iterator it=target.begin();
	for(;it!=target.end();++it){
		AIActor* a=*it;
		if(!a->GetPhysicalProp())
			continue;
		if(a->GetPhysicalProp()!=m_physics){
			centerOfMass+=a->GetPhysicalProp()->GetGlobalPosition();
			neighborCnt++;
		}
	}
	if(neighborCnt>0){
		centerOfMass/=neighborCnt;
		steeringForce=seek(centerOfMass);
		steeringForce.Normalize();
	}
	return steeringForce;
}
math::vector3d SteerController::flee(const math::vector3d&target){
	return -seek(target);
}

math::vector3d SteerController::arrive(const math::vector3d&target,float deceleraion){
	if(deceleraion==0)
		return 0;
	math::vector3d toTarget=target-m_physics->GetGlobalPosition();
	float dist=toTarget.Length();
	if(dist>0){
		float speed=dist/deceleraion;
		speed=math::Min(speed,m_physics->GetMaxSpeed());
		math::vector3d velocity=toTarget*speed/dist;
		return velocity-m_physics->GetLinearVelocity();
	}
	return 0;
}

math::vector3d SteerController::pursuit(AIActor*target){
	IAIPhysics*tPh=target->GetPhysicalProp();
	if(!tPh)
		return math::vector3d::Zero;

	math::vector3d toEvader=tPh->GetGlobalPosition()-m_physics->GetGlobalPosition();
	toEvader.Normalize();
	float relHeading=m_physics->GetHeading().dotProduct(target->GetHeading());

	math::vector3d heading=m_physics->GetHeading()*math::vector3d::ZAxis;
	if(toEvader.dotProduct(heading) >0 && relHeading<-0.95 )
	{
		return seek(tPh->GetGlobalPosition());
	}
	float lookAheadTime=toEvader.Length()/(m_physics->GetMaxSpeed()+tPh->GetLinearVelocity().Length());
	lookAheadTime+=turnAroundTime(tPh->GetGlobalPosition());

	math::vector3d pos=tPh->GetGlobalPosition()+tPh->GetLinearVelocity()*lookAheadTime;

	return seek(pos);
}

math::vector3d SteerController::evade(AIActor*target){
	IAIPhysics*tPh=target->GetPhysicalProp();
	if(!tPh)
		return math::vector3d::Zero;
	math::vector3d toPursuer=tPh->GetGlobalPosition()-m_physics->GetGlobalPosition();

	float lookAheadTime=toPursuer.Length()/(m_physics->GetMaxSpeed()+tPh->GetLinearVelocity().Length());

	math::vector3d pos=tPh->GetGlobalPosition()+tPh->GetLinearVelocity()*lookAheadTime;

	return flee(pos);
}

math::vector3d SteerController::interpose(AIActor*a,AIActor*b){
	if(!a->GetPhysicalProp() || !b->GetPhysicalProp())
		return math::vector3d::Zero;
	math::vector3d center=(a->GetPhysicalProp()->GetGlobalPosition()+
		b->GetPhysicalProp()->GetGlobalPosition())*0.5;
	float timeToReachP=m_physics->GetGlobalPosition().getDist(center)/m_physics->GetMaxSpeed();
	math::vector3d pa=a->GetPhysicalProp()->GetGlobalPosition();
	if(a->GetPhysicalProp())
		pa+=a->GetPhysicalProp()->GetLinearVelocity()*timeToReachP;

	math::vector3d pb=b->GetPhysicalProp()->GetGlobalPosition();
	if(b->GetPhysicalProp())
		pb+=b->GetPhysicalProp()->GetLinearVelocity()*timeToReachP;

	return arrive((pa+pb)*0.5,1);
}

math::vector3d SteerController::getHidePosition(const math::vector3d&center,float radius,
							   const math::vector3d&target)
{
	const float distanceFromOb=10;
	float distAway=distanceFromOb + radius;

	math::vector3d toOb=center - target;
	toOb.Normalize();

	return toOb*distAway+center;
}

math::vector3d SteerController::hide(AIActor*target,ObstacleList&obstacles){
	float bestDist=math::Infinity;
	math::vector3d bestPos;

	bool found=false;

	ObstacleList::iterator bestIt;

	math::vector3d tPos=target->GetPhysicalProp()->GetGlobalPosition();

	ObstacleList::iterator p=obstacles.begin();
	for(;p!=obstacles.end();++p){
		AgentObstacle*ob=*p;
		math::vector3d pos=getHidePosition(ob->GetPhysics()->GetGlobalPosition(),
			ob->GetRadius(),tPos);

		float dist=m_physics->GetGlobalPosition().getDistSQ(pos);
		if(dist<bestDist){
			bestDist=dist;
			bestPos=pos;
			bestIt=p;
			found=true;
		}
	}
	if(!found){
		return evade(target);
	}
	return arrive(bestPos);
}


math::vector3d SteerController::offsetPursuit(AIActor*target,const math::vector3d& offset){

	math::matrix4x4 m;
	m=math::MathUtil::CreateLookAtMatrix(target->GetPhysicalProp()->GetGlobalPosition(),target->GetHeading(),target->GetUp());

	math::vector3d woff=m*offset;

	math::vector3d toOff=woff-m_physics->GetGlobalPosition();

	float lookAheadTime=toOff.Length()/(m_physics->GetMaxSpeed()+target->GetPhysicalProp()->GetLinearVelocity().Length());

	woff+=target->GetPhysicalProp()->GetLinearVelocity()*lookAheadTime;

	return arrive(woff);
}


math::vector3d SteerController::headComponent(){
	return m_physics->GetHeading()*(m_physics->GetHeading().dotProduct(m_steeringForce));
}
math::vector3d SteerController::upComponent(){
	return m_physics->GetUp()*(m_physics->GetUp().dotProduct(m_steeringForce));
}




void SteerController::Visualize(IRenderDevice*dev)
{
}

xml::XMLElement* SteerController::loadXMLSettings(xml::XMLElement* elem)
{
	IObject::loadXMLSettings(elem);
	xml::xmlAttributesMapIT it= elem->getAttributesBegin();
	xml::xmlAttributesMapIT end= elem->getAttributesEnd();
	for(;it!=end;++it)
	{
		xml::XMLAttribute&attr=it->second;
		int i=EnumManager::getInstance().getValue(mT("ESteeringTypes"),attr.name);
		if(i!=-1)
		{
			m_weights[i]=core::StringConverter::toFloat(attr.value);
		}else if(attr.name.equals_ignore_case(mT("WanderJitter")))
		{
			m_wanderJitter=core::StringConverter::toFloat(attr.value);
		}else if(attr.name.equals_ignore_case(mT("WanderRadius")))
		{
			m_wanderRadius=core::StringConverter::toFloat(attr.value);
		}else if(attr.name.equals_ignore_case(mT("WanderDistance")))
		{
			m_wanderDistance=core::StringConverter::toFloat(attr.value);
		}
	}
	return elem;
}


}
}
