#include "stdafx.h"


#include "RingParticleEmitter.h"
#include <IParticle.h>
#include <Randomizer.h>


namespace mray{
namespace scene{

	IMPLEMENT_RTTI(RingParticleEmitter,ICustomParticleEmitter)

RingParticleEmitter::RingParticleEmitter()
/*,
	InnerRadius(this,&RingParticleEmitter::setInnerRadius,&RingParticleEmitter::getInnerRadius,EPT_VECTOR3Df,mT("innerRadius"),mT("Holds inner radius of the ring")),
	OutterRadius(this,&RingParticleEmitter::setOutterRadius,&RingParticleEmitter::getOutterRadius,EPT_VECTOR3Df,mT("outterRadius"),mT("Holds outter radius of the ring")),
	Height(this,&RingParticleEmitter::setHeight,&RingParticleEmitter::getHeight,EPT_FLOAT,mT("height"),mT("Holds the height of the ring"))*/
{
	/*	InnerRadius=0;
	Height=2;
	OutterRadius=1;*/
	m_innerRadius=0;
	m_outterRadius=1;
	m_height=2;


	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeInnerRadius::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeOutterRadius::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeHeight::instance,mT("Design"));
	}
/*
	addPropertie(&InnerRadius,mT("Behavior"));
	addPropertie(&OutterRadius,mT("Behavior"));
	addPropertie(&Height,mT("Behavior"));*/
}

void RingParticleEmitter::setupParticle(IParticle*particle)
{
	//0 .. 360
	float angle=math::Randomizer::rand01()*360.0f;
	math::vector2d r;
	r.x=math::Randomizer::randRange(m_innerRadius.x,m_outterRadius.x);
	r.y=math::Randomizer::randRange(m_innerRadius.y,m_outterRadius.y);


	particle->position.x=r.x*math::cosd(angle);
	particle->position.y=m_height*(math::Randomizer::rand01() - 0.5f);
	particle->position.z=r.y*math::sind(angle);

}



bool  RingParticleEmitter::setInnerRadius(const math::vector2d &r){
	if(r.x>m_outterRadius.x){
		m_innerRadius.x=m_outterRadius.x=r.x;
	}else{
		m_innerRadius.x=r.x;
	}
	if(r.y>m_outterRadius.y){
		m_innerRadius.y=m_outterRadius.x=r.y;
	}else{
		m_innerRadius.y=r.y;
	}
	return true;
}

bool  RingParticleEmitter::setOutterRadius(const math::vector2d &r){
	if(r.x<m_innerRadius.x){
		m_outterRadius.x=m_innerRadius.x=r.x;
	}else{
		m_outterRadius.x=r.x;
	}
	if(r.y<m_innerRadius.y){
		m_outterRadius.y=m_innerRadius.y=r.y;
	}else{
		m_outterRadius.y=r.y;
	}
	return true;
}

IMPLEMENT_PROPERTY_TYPE_HEADER(InnerRadius,RingParticleEmitter,math::vector2d,mT("InnerRadius"),EPBT_Basic,mT("Inner Radius"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(InnerRadius,RingParticleEmitter,math::vector2d,setInnerRadius,getInnerRadius,core::StringConverter::toString,core::StringConverter::toVector2d,true)

IMPLEMENT_PROPERTY_TYPE_HEADER(OutterRadius,RingParticleEmitter,math::vector2d,mT("OutterRadius"),EPBT_Basic,mT("Outter Radius"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(OutterRadius,RingParticleEmitter,math::vector2d,setOutterRadius,getOutterRadius,core::StringConverter::toString,core::StringConverter::toVector2d,true)

IMPLEMENT_PROPERTY_TYPE_HEADER(Height,RingParticleEmitter,float,mT("Height"),EPBT_Basic,mT("Height"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Height,RingParticleEmitter,float,setHeight,getHeight,core::StringConverter::toString,core::StringConverter::toFloat,true)


//////////////////////////////////////////////////////////

const core::string SRingPSFactory::Type=mT("RingEmitter");
const core::string& SRingPSFactory::getType(){
	return Type;
}
bool SRingPSFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

ICustomParticleEmitter*SRingPSFactory::createParticleEmitter(){
	return new RingParticleEmitter();
}

}
}








