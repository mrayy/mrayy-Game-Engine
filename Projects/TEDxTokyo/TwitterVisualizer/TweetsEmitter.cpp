

#include "stdafx.h"
#include "TweetsEmitter.h"

#include "TwitterTweet.h"
#include "TweetParticle.h"

namespace mray
{
namespace scene
{

TweetsEmitter::TweetsEmitter()
{
	m_startRadius = 100;
	m_lastSpawnTweet = 0;
}
TweetsEmitter::~TweetsEmitter()
{
}

IParticle*  TweetsEmitter::createParticle()
{
	ted::TwitterTweet*t;
	if (m_lastSpawnTweet >= ted::TwitterTweet::TwitterTweetList.size())
		return 0;
	t = ted::TwitterTweet::TwitterTweetList[m_lastSpawnTweet++];
	TweetParticle* part = new TweetParticle(this, t);

	return part;
}
void TweetsEmitter::reSpawn(IParticle* p)
{
	TweetParticle* part = dynamic_cast<TweetParticle*>(p);
	if (!part)
		return;
	part->mainLifeSpawn = part->lifeSpawn = -1;
	if (part->lifeSpawn == 0)return;


	float rnd = math::Randomizer::rand01();
	part->color.Set(255, 255, 255, 255);
	part->scale = 0.1;

	part->velocity = 0;
	part->acceleration = 0;

	part->bAlive = 1;

	part->randVal.x = math::Randomizer::rand01();
	part->randVal.y = math::Randomizer::rand01();
	part->randVal.z = math::Randomizer::rand01();
	part->randVal.w = math::Randomizer::rand01();


	float rand = math::Randomizer::rand01();
	rand = rand*rand;
	float r = rand*m_startRadius;
	float a = math::Randomizer::rand01()*math::PI32 * 2;
	float b = math::Randomizer::rand01()*math::PI32 * 2;

	part->position.x = r*cos(a)*cos(b);
	part->position.y = 0.3*r*sin(b);
	part->position.z = 1.5*r*sin(a)*cos(b);
	part->lifeSpawn = -1;
	part->scale = 0.8;

	float random = 3 + part->GetTweet()->date.GetDate().GetDay();
	part->SetTargetRadius(random);

	setupParticle(part);

	part->position = m_system->getOwner()->getAbsoluteTransformation()*(part->position);
	m_TweetParticleMap[part->GetTweet()->ID] = part;
}
void TweetsEmitter::AddTweet(ted::TwitterTweet* t)
{
}
TweetParticle* TweetsEmitter::GetTweetParticle(ulong id)
{
	std::map<ulong, TweetParticle*>::iterator it= m_TweetParticleMap.find(id);
	if (it == m_TweetParticleMap.end())
		return 0;
	return it->second;
}

}
}
