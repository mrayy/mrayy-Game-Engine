#include "stdafx.h"

#include "PAScaleInterpolater.h"
#include "ParticleEmitter.h"
#include "IParticle.h"

namespace mray{
namespace scene{

PAScaleInterpolater::PAScaleInterpolater(){
}
PAScaleInterpolater::~PAScaleInterpolater(){
	m_scaleKeys.clear();
}

void PAScaleInterpolater::applyOnParticle(IParticle*p,float dt){
	float time=1-p->lifeSpawn/p->mainLifeSpawn;
	math::MinMaxPair<math::vector3d> v= m_scaleKeys.getInterpolatedKey(time);
	p->scale.x=math::lerp(v.minVal.x, v.maxVal.x,p->randVal.x);
	p->scale.y=math::lerp(v.minVal.y, v.maxVal.y,p->randVal.y);
	p->scale.z=math::lerp(v.minVal.z, v.maxVal.z,p->randVal.z);
}

void PAScaleInterpolater::addKey(float time,math::vector3d scale){
	m_scaleKeys.addKey(time,math::MinMaxPair<math::vector3d>(scale,scale));
}
void PAScaleInterpolater::addKey(float time,math::vector3d minscale,math::vector3d maxscale){
	m_scaleKeys.addKey(time,math::MinMaxPair<math::vector3d>(minscale,maxscale));

}

void PAScaleInterpolater::removeKey(int index){
	m_scaleKeys.removeKey(index);
}

////////////////////////////////////////////////////

const core::string g_type=mT("ScaleInterpolater");

core::string ScaleInterpolaterFactory::getType(){
	return g_type.c_str();
}
bool ScaleInterpolaterFactory::canCreate(const core::string&type){
	return g_type.equals_ignore_case(type);
}

IParticleAffector*ScaleInterpolaterFactory::createAffector(){
	return new PAScaleInterpolater();
}


}
}




