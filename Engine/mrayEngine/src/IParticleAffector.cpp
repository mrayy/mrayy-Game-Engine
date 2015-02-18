#include "stdafx.h"

#include "IParticleAffector.h"
#include "ParticleEmitter.h"


namespace mray{
namespace scene{
	
	
	void IParticleAffector::applyOnParticles(ParticleEmitter*ps,float dt)
	{
		ParticleEmitter::ParticleList&plist= ps->getParticles();
		ParticleEmitter::ParticleList::iterator it=plist.begin();
		for (;it!=plist.end();++it)
		{
			applyOnParticle(*it,dt);
		}

	}

}
}
