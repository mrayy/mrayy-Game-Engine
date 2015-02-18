

#include "stdafx.h"
#include "TweetParticle.h"
#include "TwitterTweet.h"
#include "TweetsEmitter.h"



namespace mray
{
namespace ted
{
	class TwitterTweet;
}
namespace scene
{

TweetParticle::TweetParticle(TweetsEmitter* e, ted::TwitterTweet* Tweet)
:IBaseParticle(e), m_Tweet(Tweet)
{
	m_levelID = 0;
	m_randomAxis = math::quaternion(math::Randomizer::rand01() * 360, math::Randomizer::rand01() * 360, math::Randomizer::rand01() * 360)*math::vector3d::ZAxis;
}
TweetParticle::~TweetParticle()
{

}

void TweetParticle::update(float dt)
{
	IBaseParticle::update(dt);
	math::vector3d parent = m_emitter->GetSystem()->getOwner()->getAbsolutePosition();
	math::vector3d dir = position - parent;
	float len = dir.Normalize();
	float diff = len-m_targetRadius;

	color.A = 1 - math::clamp<float>(diff / 10,0,1);
	scale = color.A*0.3f;

	if (diff <= 0)
	{
		//position = parent+dir*m_targetRadius;
		velocity = 0;
	}
	else
	{
		velocity = -dir*10;
	}

	velocity += dir.crossProduct(m_randomAxis);
}

}
}
