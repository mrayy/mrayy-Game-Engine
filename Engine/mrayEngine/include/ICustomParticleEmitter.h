
/********************************************************************
	created:	2013/02/05
	created:	5:2:2013   14:23
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\ICustomParticleEmitter.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ICustomParticleEmitter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ICustomParticleEmitter__
#define __ICustomParticleEmitter__

#include "ParticleEmitter.h"


namespace mray
{
namespace scene
{

class IParticle;

class MRAY_DLL ICustomParticleEmitter:public ParticleEmitter
{
	DECLARE_RTTI
protected:
public:
	ICustomParticleEmitter(){}
	virtual~ICustomParticleEmitter(){}

	virtual void setupParticle(IParticle* p)=0;
};

}
}


#endif

