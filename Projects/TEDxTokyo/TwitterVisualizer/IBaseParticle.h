

/********************************************************************
	created:	2014/04/04
	created:	4:4:2014   1:10
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\IBaseParticle.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	IBaseParticle
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IBaseParticle__
#define __IBaseParticle__



#include "IParticle.h"


namespace mray
{
namespace scene
{

class IBaseParticle:public IParticle
{
protected:
public:
	IBaseParticle(ParticleEmitter*e) :IParticle(e){}
	virtual~IBaseParticle(){}


};

}
}


#endif
