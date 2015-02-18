
#include "stdafx.h"
#include "GAMutation.h"
#include "GAChromosome.h"
#include <Randomizer.h>


namespace mray
{
namespace AI
{

GAFlipMutation::GAFlipMutation(float chance):m_chance(chance)
{
}
GAFlipMutation::~GAFlipMutation()
{
}

void GAFlipMutation::Apply(GAChromosome* c)
{
	if(1-math::Randomizer::rand01()<m_chance)
		return;
	c->Flip(math::Randomizer::rand(c->GetCodeLength()),1);
}


}
}
