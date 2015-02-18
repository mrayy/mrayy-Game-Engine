#include "stdafx.h"


#include "SphereParticleEmitter.h"
#include <IParticle.h>
#include <Randomizer.h>
//#include <CPropertie.h>


namespace mray{
namespace scene{

IMPLEMENT_RTTI(SphereParticleEmitter,ICustomParticleEmitter)

SphereParticleEmitter::SphereParticleEmitter()/*,
	InnerRadius(this,&SphereParticleEmitter::setInnerRadius,&SphereParticleEmitter::getInnerRadius,EPT_VECTOR3Df,mT("innerRadius"),mT("Holds inner radius of the sphere")),
	OutterRadius(this,&SphereParticleEmitter::setOutterRadius,&SphereParticleEmitter::getOutterRadius,EPT_VECTOR3Df,mT("outterRadius"),mT("Holds outter radius of the sphere"))*/
{

/*

	addPropertie(&InnerRadius,mT("Behavior"));
	addPropertie(&OutterRadius,mT("Behavior"));*/

	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeInnerRadius::instance,mT("Design"));
		dic->addPropertie(&PropertyTypeOutterRadius::instance,mT("Design"));
	}
}

void SphereParticleEmitter::setupParticle(IParticle*particle)
{
	//0 .. 360
	float Alpha=math::Randomizer::rand01()*360.0f;
	float theta=math::Randomizer::rand01()*360.0f;

	math::vector3d r;
	r.x=math::Randomizer::randRange(m_innerRadius.x,m_outterRadius.x);
	r.y=math::Randomizer::randRange(m_innerRadius.y,m_outterRadius.y);
	r.z=math::Randomizer::randRange(m_innerRadius.z,m_outterRadius.z);

	float t_cs=math::cosd(theta);

	particle->position.x=r.x*math::cosd(Alpha)*t_cs;
	particle->position.y=r.y*math::sind(theta);
	particle->position.z=r.z*math::sind(Alpha)*t_cs;

}



bool  SphereParticleEmitter::setInnerRadius(const math::vector3d & r){
	if(r.x>m_outterRadius.x){
		m_innerRadius.x=m_outterRadius.x=r.x;
	}else{
		m_innerRadius.x=r.x;
	}
	
	if(r.y>m_outterRadius.y){
		m_innerRadius.y=m_outterRadius.y=r.y;
	}else{
		m_innerRadius.y=r.y;
	}
	
	if(r.z>m_outterRadius.z){
		m_innerRadius.z=m_outterRadius.z=r.z;
	}else{
		m_innerRadius.z=r.z;
	}
	return true;
}

bool  SphereParticleEmitter::setOutterRadius(const math::vector3d & r){
	if(r.x<m_innerRadius.x){
		m_innerRadius.x=m_outterRadius.x=r.x;
	}else{
		m_outterRadius.x=r.x;
	}
	
	if(r.y<m_innerRadius.y){
		m_innerRadius.y=m_outterRadius.y=r.y;
	}else{
		m_outterRadius.y=r.y;
	}
	
	if(r.z<m_innerRadius.z){
		m_innerRadius.z=m_outterRadius.z=r.z;
	}else{
		m_outterRadius.z=r.z;
	}
	return true;
}
IMPLEMENT_PROPERTY_TYPE_HEADER(InnerRadius,SphereParticleEmitter,math::vector3d,mT("InnerRadius"),EPBT_Basic,mT("Inner Radius"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(InnerRadius,SphereParticleEmitter,math::vector3d,setInnerRadius,getInnerRadius,core::StringConverter::toString,core::StringConverter::toVector3d,true)

IMPLEMENT_PROPERTY_TYPE_HEADER(OutterRadius,SphereParticleEmitter,math::vector3d,mT("OutterRadius"),EPBT_Basic,mT("Outter Radius"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(OutterRadius,SphereParticleEmitter,math::vector3d,setOutterRadius,getOutterRadius,core::StringConverter::toString,core::StringConverter::toVector3d,true)


//////////////////////////////////////////////////////////

const core::string SSpherePSFactory::Type=mT("SphereEmitter");
const core::string& SSpherePSFactory::getType(){
	return Type;
}
bool SSpherePSFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

ICustomParticleEmitter*SSpherePSFactory::createParticleEmitter(){
	return new SphereParticleEmitter();
}

}
}








