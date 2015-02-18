#include "stdafx.h"


#include "PAVelocityInterpolater.h"
#include "ParticleEmitter.h"
#include "IParticle.h"

namespace mray{
namespace scene{

PAVelocityInterpolater::PAVelocityInterpolater(){
}
PAVelocityInterpolater::~PAVelocityInterpolater(){
	m_velocityKeys.clear();
}

void PAVelocityInterpolater::applyOnParticle(IParticle*p,float dt){
	float time=1-p->lifeSpawn/p->mainLifeSpawn;
	math::MinMaxPair<math::vector3d> v= m_velocityKeys.getInterpolatedKey(time);
	p->velocity.x=math::lerp(v.minVal.x, v.maxVal.x,p->randVal.x);
	p->velocity.y=math::lerp(v.minVal.y, v.maxVal.y,p->randVal.y);
	p->velocity.z=math::lerp(v.minVal.z, v.maxVal.z,p->randVal.z);
}

void PAVelocityInterpolater::addKey(float time,math::vector3d vel){
	m_velocityKeys.addKey(time,math::MinMaxPair<math::vector3d>(vel,vel));
}
void PAVelocityInterpolater::addKey(float time,math::vector3d minvel,math::vector3d maxvel){
	m_velocityKeys.addKey(time,math::MinMaxPair<math::vector3d>(minvel,maxvel));

}
void PAVelocityInterpolater::removeKey(int index){
	m_velocityKeys.removeKey(index);
}

////////////////////////////////////////////////////

const core::string g_type=mT("VelocityInterpolater");

core::string VelocityInterpolaterFactory::getType(){
	return g_type.c_str();
}
bool VelocityInterpolaterFactory::canCreate(const core::string&type){
	return g_type.equals_ignore_case(type);
}

IParticleAffector*VelocityInterpolaterFactory::createAffector(){
	return new PAVelocityInterpolater();
}


}
}






