
/********************************************************************
	created:	2009/06/03
	created:	3:6:2009   16:23
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\MissileLauncher.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	MissileLauncher
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MissileLauncher___
#define ___MissileLauncher___

#include "GameEntity.h"
#include "GameEntity.h"
#include "Missile.h"
#include "BankAnimatedSprite.h"
#include "AntiPlaneMissile.h"
#include <ISceneNode.h>
#include <ISound.h>
#include <STextNode.h>
#include <GUIImage.h>

namespace mray{
namespace gameMod{

class GameLevel;

class MissileLauncher:public GameEntity
{
private:
protected:
	GCPtr<scene::ISceneNode> m_shape;
	GameLevel* m_level;

	core::string m_missile;

	float m_missileTimeOut;

	float m_LaunchTimeOutCount;
	float m_LaunchTimeOut;

	GCPtr<scene::BankAnimatedSprite> m_explosion;

	GCPtr<AntiPlaneMissile> m_AntiPlaneMissile;

	GCPtr<AntiPlaneMissile> createMissile();
public:
	MissileLauncher(GameLevel*level,GCPtr<scene::ISceneNode> shape,const core::string&missile);
	virtual~MissileLauncher();

	virtual void update(float dt);

	void setPosition(const math::vector3d& r);
	virtual void setRotation(const math::quaternion&p);
	virtual void setScale(const math::vector3d&p);

	math::vector3d getPosition();
	virtual math::quaternion getRotation();
	virtual math::vector3d getScale();

	virtual GCPtr<scene::ISceneNode> getShape(){return m_shape;}

	virtual math::box3d getBoundingBox();

	virtual bool isStaticEntity(){return true;}
	virtual void hasCollide();
};

}
}


#endif //___MissileLauncher___
