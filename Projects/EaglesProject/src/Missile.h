
/********************************************************************
	created:	2009/06/03
	created:	3:6:2009   16:24
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\Missile.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	Missile
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Missile___
#define ___Missile___

#include "GameEntity.h"
#include <ISceneNode.h>
#include <BankAnimatedSprite.h>
#include <SParticleSystem.h>

namespace mray{
namespace gameMod{


	class GameLevel;
class Missile:public GameEntity
{
private:
protected:

	GCPtr<scene::ISceneNode> m_shape;
	GCPtr<scene::SParticleSystem> m_smoke;

	GameEntity*m_owner;
	GameLevel*m_level;


	float m_speed;
	float m_acceleration;
	float m_maxSpeed;

	float m_timeToLive;

	GCPtr<scene::BankAnimatedSprite> m_explosion;

	bool m_Launched;

	float m_minHeight;

public:
	Missile(GameLevel*level,GCPtr<scene::ISceneNode> shape,GameEntity*owner);
	virtual~Missile();

	virtual void update(float dt);

	virtual void LaunchMissile();

	virtual void setPosition(const math::vector3d&p);
	virtual void setRotation(const math::quaternion&p);
	virtual void setScale(const math::vector3d&p);

	virtual math::vector3d getPosition();
	virtual math::quaternion getRotation();
	virtual math::vector3d getScale();

	virtual GCPtr<scene::ISceneNode> getShape();

	virtual void hasCollide();

	virtual math::box3d getBoundingBox();

	virtual bool isStaticEntity(){return false;}

};

}
}

#endif //___Missile___
