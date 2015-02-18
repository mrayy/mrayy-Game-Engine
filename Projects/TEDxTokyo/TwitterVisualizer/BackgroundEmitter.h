

/********************************************************************
	created:	2014/04/04
	created:	4:4:2014   1:08
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\BackgroundEmitter.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	BackgroundEmitter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __BackgroundEmitter__
#define __BackgroundEmitter__



#include "ParticleEmitter.h"


namespace mray
{
namespace scene
{

class BackgroundEmitter :public ParticleEmitter
{
protected:

	virtual IParticle*  createParticle();

	int m_maxConnections;

	int m_particleCount;

	float m_radius;

	bool m_inited;

	void _init();
	virtual void _SpawnParticles(float dt);
public:
	BackgroundEmitter();
	virtual~BackgroundEmitter();


};

}
}

#endif
