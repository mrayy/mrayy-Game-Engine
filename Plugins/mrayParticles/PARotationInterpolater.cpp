#include "stdafx.h"

#include "PARotationInterpolater.h"
#include "ParticleEmitter.h"
#include "IParticle.h"

namespace mray{
namespace scene{

PARotationInterpolater::PARotationInterpolater(){
}
PARotationInterpolater::~PARotationInterpolater(){
	m_rotateKeys.clear();
}

void PARotationInterpolater::applyOnParticle(IParticle*p,float dt){
	float time=1-p->lifeSpawn/p->mainLifeSpawn;
	p->AngleZ=m_rotateKeys.getInterpolatedKey(time).lerp(p->randVal.x);
}

void PARotationInterpolater::addKey(float time,float angle){
	m_rotateKeys.addKey(time,math::MinMaxPair<float>(angle,angle));
}
void PARotationInterpolater::addKey(float time,float minAngle,float maxAngle){
	m_rotateKeys.addKey(time,math::MinMaxPair<float>(minAngle,maxAngle));
}

void PARotationInterpolater::removeKey(int index){
	m_rotateKeys.removeKey(index);
}

////////////////////////////////////////////////////

const core::string g_type=mT("RotationInterpolater");

core::string RotationInterpolaterFactory::getType(){
	return g_type.c_str();
}
bool RotationInterpolaterFactory::canCreate(const core::string&type){
	return g_type.equals_ignore_case(type);
}

IParticleAffector* RotationInterpolaterFactory::createAffector(){
	return new PARotationInterpolater();
}


}
}




