
/********************************************************************
	created:	2009/04/05
	created:	5:4:2009   23:48
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\AirPlane.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	AirPlane
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AirPlane___
#define ___AirPlane___

#include "EPlaneAction.h"
#include "GameEntity.h"
#include "PlaneMissile.h"
#include "BankAnimatedSprite.h"
#include "ILockableEntity.h"

#include <ISceneNode.h>
#include <ISound.h>
#include <STextNode.h>
#include <GUIImage.h>

namespace mray{
namespace gameMod{

	class GameLevel;

class AirPlane:public GameEntity,public ILockableEntity
{
private:
protected:

	static const math::vector3d RotateSpeed;
	static const float RotateSpeedDamping;

	GCPtr<scene::IMovable> m_bladeRight;
	GCPtr<scene::IMovable> m_bladeLeft;
	GCPtr<scene::IMovable> m_bladeBTopRight;
	GCPtr<scene::IMovable> m_bladeBTopLeft;
	GCPtr<scene::IMovable> m_bladeBBotRight;
	GCPtr<scene::IMovable> m_bladeBBotLeft;

	GCPtr<sound::ISound> m_jetSound;

	GCPtr<scene::BankAnimatedSprite> m_explosion;

	GCPtr<scene::STextNode> m_heightTxt;

	GCPtr<scene::ISceneNode> m_shape;

	GCPtr<scene::SParticleSystem> m_leftEngine;
	GCPtr<scene::SParticleSystem> m_rightEngine;

	GCPtr<PlaneMissile> m_leftMissile;
	GCPtr<PlaneMissile> m_rightMissile;

	GCPtr<GUI::GUIImage> m_leftMissileGUI;
	GCPtr<GUI::GUIImage> m_rightMissileGUI;
	GCPtr<GUI::GUIImage> m_stopGUI;

	float m_radius;

	float m_turbo;
	float m_targetTurbo;
	float m_brake;
	float m_targetBrake;
	float m_speed;
	float m_baseSpeed;
	math::vector3d m_rotateSpeed;
	math::vector3d m_force;
	math::vector3d m_vSpeed;
	math::vector3d m_turboSpeed;
	math::vector3d m_targetRotateAngle;
	math::vector3d m_rotateAngle;
	math::vector3d m_position;

	core::string m_missile;

	GameLevel* m_level;

	float m_height;
	float m_minHeight;

	float m_leftMissileTime;
	float m_rightMissileTime;

	GCPtr<video::SMaterial> m_mtrl;

	float m_missileTimeOut;

	float m_LaunchTimeOutCount;
	float m_LaunchTimeOut;

	GCPtr<PlaneMissile> createMissile();

public:
	AirPlane(GameLevel*level,GCPtr<scene::ISceneNode> shape,const core::string&missile);
	virtual~AirPlane();

	virtual void doAction(EPlaneAction action,bool press);

	virtual void update(float dt);

	void resetForce(const math::vector3d&f);
	void applyForce(const math::vector3d&f);

	void setRotationAngles(const math::vector3d& r);
	void setPosition(const math::vector3d& r);
	virtual void setRotation(const math::quaternion&p);
	virtual void setScale(const math::vector3d&p);

	math::vector3d getRotationAngles();
	math::vector3d getPosition();
	virtual math::quaternion getRotation();
	virtual math::vector3d getScale();

	virtual GCPtr<scene::ISceneNode> getShape(){return m_shape;}

	void setRadius(float r);
	float getRadius();

	float getHeight(){return m_height;}


	virtual void hasCollide();
	bool isExplode();

	float getSpeed();

	virtual void addLockedMissile(GCPtr<Missile> m);
	virtual void removeLockedMissile(GCPtr<Missile> m);

	virtual GameEntity *getAsGameEntity();

	virtual math::box3d getBoundingBox();

	virtual bool isStaticEntity(){return false;}

};

}
}


#endif //___AirPlane___
