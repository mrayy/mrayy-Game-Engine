

/********************************************************************
	created:	2014/04/10
	created:	10:4:2014   16:28
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\TweetParticle.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	TweetParticle
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TweetParticle__
#define __TweetParticle__

#include "IBaseParticle.h"


namespace mray
{
	namespace ted
	{
		class TwitterTweet;
	}
namespace scene
{

	class TweetsEmitter;

class TweetParticle:public IBaseParticle
{
protected:
	ted::TwitterTweet* m_Tweet;
	float m_targetRadius;

	int m_levelID;

	math::vector3d m_randomAxis;
public:
	TweetParticle(TweetsEmitter* e,ted::TwitterTweet* Tweet);
	virtual~TweetParticle();

	ted::TwitterTweet* GetTweet(){ return m_Tweet; }

	int GetLevelID(){ return m_levelID; }

	void SetTargetRadius(float r)
	{
		m_levelID = r;
		m_targetRadius = r;
	}

	virtual void update(float dt);

};

}
}


#endif