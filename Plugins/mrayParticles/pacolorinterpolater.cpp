#include "stdafx.h"

#include "PAColorInterpolater.h"
#include "ParticleEmitter.h"
#include "IParticle.h"

namespace mray{
namespace scene{

PAColorInterpolater::PAColorInterpolater(){
}
PAColorInterpolater::~PAColorInterpolater(){
	m_colorKeys.clear();
}
 
void PAColorInterpolater::applyOnParticle(IParticle*p,float ){
	float time=1-p->lifeSpawn/p->mainLifeSpawn;
	math::MinMaxPair<video::SColor> v= m_colorKeys.getInterpolatedKey(time);
	p->color.R=math::lerp(v.minVal.R, v.maxVal.R,p->randVal.x);
	p->color.G=math::lerp(v.minVal.G, v.maxVal.G,p->randVal.y);
	p->color.B=math::lerp(v.minVal.B, v.maxVal.B,p->randVal.z);
	p->color.A=math::lerp(v.minVal.A, v.maxVal.A,p->randVal.w);
}

void PAColorInterpolater::addKey(float time,video::SColor color){
	m_colorKeys.addKey(time,color);
}
void PAColorInterpolater::addKey(float time,video::SColor mincolor,video::SColor maxcolor){
	m_colorKeys.addKey(time,math::MinMaxPair<video::SColor>(mincolor,maxcolor));

}
void PAColorInterpolater::removeKey(int index){
	m_colorKeys.removeKey(index);
}

////////////////////////////////////////////////////


core::string ColorInterpolaterFactory:: getType(){
	return mT("ColorInterpolater");
}
bool ColorInterpolaterFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

IParticleAffector*ColorInterpolaterFactory::createAffector(){
	return new PAColorInterpolater();
}


}
}