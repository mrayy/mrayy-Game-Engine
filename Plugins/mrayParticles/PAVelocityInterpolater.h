

#ifndef __PAVelocityInterpolater___
#define __PAVelocityInterpolater___

#include <IParticleAffector.h>
#include <IParticleAffectorFactory.h>
#include <CKeyableAttr.h>
#include <Point3d.h>
#include <MinMaxPair.h>

#include "compileConfig.h"

namespace mray{
namespace scene{

class MRAY_PS_DLL PAVelocityInterpolater:public IParticleAffector
{
	core::CKeyableAttr<math::MinMaxPair<math::vector3d>> m_velocityKeys;
public:

	PAVelocityInterpolater();
	virtual~PAVelocityInterpolater();

	virtual bool affectsTransformations(){return true;}

	virtual void applyOnParticle(IParticle*p,float dt);

	void addKey(float time,math::vector3d vel);
	void addKey(float time,math::vector3d minvel,math::vector3d maxvel);
	void removeKey(int index);
};


class MRAY_PS_DLL VelocityInterpolaterFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);

	virtual IParticleAffector*createAffector();
};

}
}


#endif

