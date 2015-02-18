

/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   15:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayParticles\PARotationInterpolater.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayParticles
	file base:	PARotationInterpolater
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PARotationInterpolater___
#define ___PARotationInterpolater___

#include <SParticleSystem.h>
#include <IParticleAffector.h>
#include <IParticleAffectorFactory.h>
#include <CKeyableAttr.h>
#include <MinMaxPair.h>

#include "compileConfig.h"

namespace mray{

namespace scene{


class MRAY_PS_DLL PARotationInterpolater:public IParticleAffector
{

	core::CKeyableAttr<math::MinMaxPair<float>> m_rotateKeys;
public:

	PARotationInterpolater();
	virtual~PARotationInterpolater();

	virtual bool affectsTransformations(){return false;}//we care about pos and scale

	virtual void applyOnParticle(IParticle*p,float dt);

	void addKey(float time,float angle);
	void addKey(float time,float minAngle,float maxAngle);
	void removeKey(int index);
};


class MRAY_PS_DLL RotationInterpolaterFactory:public IParticleAffectorFactory
{
public:
	virtual core::string getType();
	virtual bool canCreate(const core::string&type);

	virtual IParticleAffector*createAffector();
};


}

}


#endif //___PARotationInterpolater___
