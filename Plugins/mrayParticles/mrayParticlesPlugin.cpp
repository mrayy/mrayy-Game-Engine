#include "stdafx.h"



#include "mrayParticles.h"
#include "compileConfig.h"

namespace mray{
namespace plugins{

mrayParticleEmittersPlugin::mrayParticleEmittersPlugin(){
	m_boxPS=new scene::SBoxPEFactory();
	//m_meshPS=new scene::SMeshPSFactory();
	m_ringPS=new scene::SRingPSFactory();
	m_spherePS=new scene::SSpherePSFactory();

	
	m_planeBPA=new scene::PlaneBlockerFactory();
	m_boxBPA=new scene::BoxBlockerFactory();
	m_sphereBPA=new scene::SphereBlockerFactory();

	m_colorInterPA=new scene::ColorInterpolaterFactory();
	m_scaleInterPA=new scene::ScaleInterpolaterFactory();
	m_velocityInterPA=new scene::VelocityInterpolaterFactory();
	m_rotationInterPA=new scene::RotationInterpolaterFactory();
}
mrayParticleEmittersPlugin::~mrayParticleEmittersPlugin(){
	m_colorInterPA.dispose();
}

core::string mrayParticleEmittersPlugin::getName(){
	return mT("mrayParticleEmitter");
}

void mrayParticleEmittersPlugin::install(){
	if(isInstalled())return;
	IPlugin::install();

	scene::SParticleSystemFactory::getInstance().addEmitterFactory(m_boxPS);
	//scene::SParticleSystemFactory::getInstance().addEmitterFactory(m_meshPS);
	scene::SParticleSystemFactory::getInstance().addEmitterFactory(m_ringPS);
	scene::SParticleSystemFactory::getInstance().addEmitterFactory(m_spherePS);
	
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_planeBPA);
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_boxBPA);
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_sphereBPA);
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_colorInterPA);
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_scaleInterPA);
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_velocityInterPA);
	scene::SParticleSystemFactory::getInstance().addAffectorFactory(m_rotationInterPA);
}

void mrayParticleEmittersPlugin::uninstall(){
	if(!isInstalled())return;
	IPlugin::uninstall();

	scene::SParticleSystemFactory::getInstance().removeParticleEmitter(m_boxPS->getType());
	//scene::SParticleSystemFactory::getInstance().removeParticleEmitter(m_meshPS->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleEmitter(m_ringPS->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleEmitter(m_spherePS->getType());
	
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_planeBPA->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_boxBPA->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_sphereBPA->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_colorInterPA->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_scaleInterPA->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_velocityInterPA->getType());
	scene::SParticleSystemFactory::getInstance().removeParticleAffector(m_rotationInterPA->getType());
}


	


extern "C" MRAY_PS_DLL IPlugin* dllCreatePlugin(){
	return new mrayParticleEmittersPlugin();

}


}
}
