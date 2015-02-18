

#include "JetParticleSystem.h"
#include <Randomizer.h>

namespace mray{
namespace gameMod{

JetParticleSystem::JetParticleSystem(){
}
JetParticleSystem::~JetParticleSystem(){
}

void JetParticleSystem::setupParticlePosition(GCPtr<scene::IParticle>particle){

	particle->position.x=JetRadius*(math::Randomizer::rand01() - 0.5f);
	particle->position.y=JetRadius*(math::Randomizer::rand01() - 0.5f);
	particle->position.z=JetRadius*(math::Randomizer::rand01() - 0.5f);
}


}
}