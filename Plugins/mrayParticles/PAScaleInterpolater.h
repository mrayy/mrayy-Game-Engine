

#ifndef ___PAScaleInterpolater___
#define ___PAScaleInterpolater___


#include <IParticleAffector.h>
#include <IParticleAffectorFactory.h>
#include <CKeyableAttr.h>
#include <MinMaxPair.h>
#include <Point3d.h>

#include "compileConfig.h"

namespace mray{
namespace scene{


class MRAY_PS_DLL PAScaleInterpolater:public IParticleAffector
{

	core::CKeyableAttr<math::MinMaxPair<math::vector3d>> m_scaleKeys;
public:

	PAScaleInterpolater();
	virtual~PAScaleInterpolater();

	virtual bool affectsTransformations(){return true;}

	virtual void applyOnParticle(IParticle*p,float dt);

	void addKey(float time,math::vector3d scale);
	void addKey(float time,math::vector3d minscale,math::vector3d maxscale);
	void removeKey(int index);
};


class MRAY_PS_DLL ScaleInterpolaterFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);

	virtual IParticleAffector*createAffector();
};


}
}




#endif







