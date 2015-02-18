
#include "AntiPlaneMissile.h"
#include "GameLevel.h"

namespace mray{
namespace gameMod{


AntiPlaneMissile::AntiPlaneMissile(GameLevel*level,GCPtr<scene::ISceneNode> shape,GameEntity*owner):
	Missile(level,shape,owner)
{
	m_timeToLive=20;

	m_acceleration=50;
	m_maxSpeed=200;

	m_target=0;
}

AntiPlaneMissile::~AntiPlaneMissile(){
}

void AntiPlaneMissile::update(float dt){

	Missile::update(dt);
	if(!m_Launched)return;

	if(m_target){
		GameEntity*ent=m_target->getAsGameEntity();
		if(ent->isDestroyed())
			return;
		math::vector3d dir;
		m_shape->getAbsoluteTransformation().transformNormalPre(math::vector3d::ZAxis,dir);
		math::vector3d toTarget=getPosition()- ent->getPosition();


		toTarget.Normalize();
		math::vector3d vec=(dir-toTarget*2*dt);

		math::quaternion q(0,vec.x,vec.y,vec.z);
		q.Normalize();

	// 	m_shape->yaw(vec.y,scene::TS_World);
	// 	m_shape->pitch(vec.x,scene::TS_Local);

		setRotation(getRotation()* q);
	}
}

void AntiPlaneMissile::LaunchMissile(ILockableEntity* target){
	Missile::LaunchMissile();

	m_target=target;

	m_shape->position=m_owner->getPosition()+math::vector3d(0,10,0);
	m_shape->setOrintation(math::quaternion(90,math::vector3d::XAxis));
}

}
}



