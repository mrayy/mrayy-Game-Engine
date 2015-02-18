




#ifndef ___IParticleAffector___
#define ___IParticleAffector___

#include "CompileConfig.h"


namespace mray{
namespace scene{
	class ParticleEmitter;
	class IParticle;

class MRAY_DLL IParticleAffector
{
public :
	virtual~IParticleAffector(){}

	virtual void applyOnParticle(IParticle*p,float dt)=0;

	virtual bool affectsTransformations()=0;

	void applyOnParticles(ParticleEmitter*ps,float dt);
};

}
}





#endif




