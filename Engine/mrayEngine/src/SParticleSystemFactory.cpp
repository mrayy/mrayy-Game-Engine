#include "stdafx.h"

#include "SParticleSystemFactory.h"
#include "IParticleEmitterFactory.h"
#include "IParticleAffectorFactory.h"
#include "ParticleEmitter.h"

namespace mray{
namespace scene{

SParticleSystemFactory::SParticleSystemFactory()
{
}
SParticleSystemFactory::~SParticleSystemFactory()
{
	Clear();
}

void SParticleSystemFactory::addEmitterFactory(IParticleEmitterFactory *f)
{
	m_PSfactories.insert(ParticleSystemF::value_type(f->getType(),f));
}
IParticleEmitterFactory *SParticleSystemFactory::getEmitterFactory(const core::string&type)
{

	ParticleSystemF::iterator it=m_PSfactories.find(type);
	if(it==m_PSfactories.end())return 0;
	return it->second;
}

void SParticleSystemFactory::addAffectorFactory(IParticleAffectorFactory *f)
{
	m_Affectorfactories.insert(ParticleAffectorF::value_type(f->getType(),f));
}
IParticleAffectorFactory*SParticleSystemFactory::getAffectorFactory(const core::string&type)
{
	ParticleAffectorF::iterator it=m_Affectorfactories.find(type);
	if(it==m_Affectorfactories.end())return 0;
	return it->second;

}

ICustomParticleEmitter*SParticleSystemFactory::createParticleEmitter(const core::string&type){
	IParticleEmitterFactory *f=getEmitterFactory(type);
	if(!f)return 0;
	return f->createParticleEmitter();
}

IParticleAffector*SParticleSystemFactory::createParticleAffector(const core::string&type){
	IParticleAffectorFactory *f=getAffectorFactory(type);
	if(!f)return 0;
	return f->createAffector();
}

void SParticleSystemFactory::removeParticleEmitter(const core::string&type){
	m_PSfactories.erase(type);
}
void SParticleSystemFactory::removeParticleAffector(const core::string&type){
	m_Affectorfactories.erase(type);
}

void SParticleSystemFactory::listParticleEmitters(std::vector<core::string> &out){
	ParticleSystemF::iterator it=m_PSfactories.begin();
	for(;it!=m_PSfactories.end();++it){
		out.push_back(it->second->getType());
	}
}
void SParticleSystemFactory::listParticleAffector(std::vector<core::string> &out){
	ParticleAffectorF::iterator it=m_Affectorfactories.begin();
	for(;it!=m_Affectorfactories.end();++it){
		out.push_back(it->second->getType());
	}
}


void SParticleSystemFactory::Clear()
{

	{
		ParticleSystemF::iterator it=m_PSfactories.begin();
		for (;it!=m_PSfactories.end();++it)
		{
			delete it->second;
		}
	}

	{
		ParticleAffectorF::iterator it=m_Affectorfactories.begin();
		for (;it!=m_Affectorfactories.end();++it)
		{
			delete it->second;
		}
	}

	m_PSfactories.clear();
	m_Affectorfactories.clear();
}


}
}

