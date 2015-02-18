

#include "stdafx.h"
#include "BackgroundEmitter.h"

#include "IBaseParticle.h"


namespace mray
{
namespace scene
{


BackgroundEmitter::BackgroundEmitter()
{
	m_particleCount = 1000;
	m_radius = 80;
	m_maxConnections = 2;
	m_inited = false;
}
BackgroundEmitter::~BackgroundEmitter()
{
}


IParticle*  BackgroundEmitter::createParticle()
{
	return new IBaseParticle(this);

}


void BackgroundEmitter::_init()
{


	for (int i = 0; i < m_particleCount; ++i)
	{

		IParticle* part = createParticle();
		reSpawn(part);
		m_particles.push_back(part);

		float rand = math::Randomizer::rand01();
		rand = rand*rand;
		float r = rand*m_radius;
		float a = math::Randomizer::rand01()*math::PI32*2;
		float b = math::Randomizer::rand01()*math::PI32 * 2;

		part->position.x = r*cos(a)*cos(b);
		part->position.y = 0.3*r*sin(b);
		part->position.z = 1.5*r*sin(a)*cos(b);
		part->lifeSpawn = -1;
		part->scale = 0.5f;
	}
}

void BackgroundEmitter::_SpawnParticles(float dt)
{

	if (!m_inited)
	{
		_init();
		m_inited = true;
	}
}


}
}
