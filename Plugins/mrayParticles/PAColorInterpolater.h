

#ifndef ___PAColorInterpolater___
#define ___PAColorInterpolater___


#include <IParticleAffector.h>
#include <IParticleAffectorFactory.h>
#include <CKeyableAttr.h>
#include <SColor.h>
#include <MinMaxPair.h>

#include "compileConfig.h"

namespace mray{
namespace scene{


class MRAY_PS_DLL PAColorInterpolater:public IParticleAffector
{

	core::CKeyableAttr<math::MinMaxPair<video::SColor>> m_colorKeys;
public:

	PAColorInterpolater();
	virtual~PAColorInterpolater();

	virtual bool affectsTransformations(){return false;}

	virtual void applyOnParticle(IParticle*p,float dt);

	void addKey(float time,video::SColor color);
	void addKey(float time,video::SColor mincolor,video::SColor maxcolor);
	void removeKey(int index);
};


class MRAY_PS_DLL ColorInterpolaterFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);

	virtual IParticleAffector*createAffector();
};


}
}




#endif


