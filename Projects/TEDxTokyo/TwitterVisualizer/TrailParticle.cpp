


#include "stdafx.h"
#include "TrailParticle.h"


namespace mray
{
namespace scene
{

TrailParticle::TrailParticle(ParticleEmitter*e)
:IBaseParticle(e)
{
}


TrailParticle::~TrailParticle()
{
}



void TrailParticle::render()
{
}


void TrailParticle::update(float dt)
{
	IBaseParticle::update(dt);
}


}
}
