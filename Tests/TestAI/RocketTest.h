
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   19:35
	filename: 	i:\Programing\GameEngine\mrayEngine\TestAI\RocketTest.h
	file path:	i:\Programing\GameEngine\mrayEngine\TestAI
	file base:	RocketTest
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RocketTest___
#define ___RocketTest___

#include <SceneManager.h>
#include <SParticleSystem.h>
#include <SBillboardChain.h>
#include <SBillboardNode.h>

namespace mray{

class RocketTest
{
private:
protected:
	GCPtr<scene::ISceneNode> m_shape;
	GCPtr<scene::SParticleSystem> m_smoke;
	GCPtr<scene::SBillboardChain> m_trail;
	GCPtr<scene::SBillboardNode> m_flare;
	math::vector3d m_speed;
	math::vector3d m_acceleration;
	float m_time;
	scene::SceneManager*m_mngr;
public:
	RocketTest(scene::SceneManager*mngr,const math::vector3d&pos,const math::quaternion&orintation);
	virtual~RocketTest();

	virtual void update(float dt);
};

}


#endif //___RocketTest___
