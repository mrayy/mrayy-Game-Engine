

/********************************************************************
	created:	2014/03/31
	created:	31:3:2014   0:39
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\TweetsEmitter.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	TweetsEmitter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TweetsEmitter__
#define __TweetsEmitter__

#include "ParticleEmitter.h"


namespace mray
{
	namespace ted
	{
		class TwitterTweet;
	}
namespace scene
{
	class TweetParticle;
class TweetsEmitter:public ParticleEmitter
{
protected:
	int m_lastSpawnTweet;

	float m_startRadius;


	virtual IParticle*  createParticle();


	std::map<ulong, TweetParticle*> m_TweetParticleMap;
public:
	TweetsEmitter();
	virtual~TweetsEmitter();
	virtual void reSpawn(IParticle* part);
	void AddTweet(ted::TwitterTweet* t);

	TweetParticle* GetTweetParticle(ulong id);
};

}
}


#endif
