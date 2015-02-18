#include "stdafx.h"


#include "BoxParticleEmitter.h"
#include "IParticle.h"
#include <Randomizer.h>
//#include <CPropertie.h>


namespace mray{
namespace scene{
	IMPLEMENT_RTTI(BoxParticleEmitter,ICustomParticleEmitter);
BoxParticleEmitter::BoxParticleEmitter()
	/*,
	Size(this,&BoxParticleEmitter::setSize,&BoxParticleEmitter::getSize,EPT_VECTOR3Df,mT("Size"),mT("Holds box size of the box on three axis"))*/
{
	m_size=1;
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeSize::instance,mT("Design"));
	}
}

void BoxParticleEmitter::setupParticle(IParticle*particle)
{

	particle->position.x=m_size.x*(math::Randomizer::rand01() - 0.5f);
	particle->position.y=m_size.y*(math::Randomizer::rand01() - 0.5f);
	particle->position.z=m_size.z*(math::Randomizer::rand01() - 0.5f);


}



IMPLEMENT_PROPERTY_TYPE_HEADER(Size,BoxParticleEmitter,math::vector3d,mT("Size"),EPBT_Basic,mT("Size"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Size,BoxParticleEmitter,math::vector3d,setSize,getSize,core::StringConverter::toString,core::StringConverter::toVector3d,true)


//////////////////////////////////////////////////////////


const core::string SBoxPEFactory::Type=mT("BoxEmitter");
const core::string& SBoxPEFactory::getType(){
	return Type;
}
bool SBoxPEFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

ICustomParticleEmitter*SBoxPEFactory::createParticleEmitter(){
	return new BoxParticleEmitter();
}

}
}








