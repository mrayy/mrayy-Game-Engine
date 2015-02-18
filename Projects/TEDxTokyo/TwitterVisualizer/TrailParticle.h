

/********************************************************************
	created:	2014/04/10
	created:	10:4:2014   1:30
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\TrailParticle.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	TrailParticle
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TrailParticle__
#define __TrailParticle__


#include "IBaseParticle.h"

namespace mray
{
namespace scene
{

class TrailParticle :public IBaseParticle
{
protected:
public:
	TrailParticle(ParticleEmitter*e);
	virtual~TrailParticle();



	virtual void render();
	virtual void update(float dt);
};

}
}


#endif
