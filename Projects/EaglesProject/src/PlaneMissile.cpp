
#include "PlaneMissile.h"
#include "AirPlane.h"
#include "GameLevel.h"

namespace mray{
namespace gameMod{


PlaneMissile::PlaneMissile(GameLevel*level,GCPtr<scene::ISceneNode> shape,AirPlane*owner):
	Missile(level,shape,owner)
{
}

PlaneMissile::~PlaneMissile(){
}

void PlaneMissile::update(float dt){

	Missile::update(dt);
}

void PlaneMissile::LaunchMissile(){
	Missile::LaunchMissile();
	m_speed=((AirPlane*)m_owner)->getSpeed();
	m_shape->position=m_shape->getAbsolutePosition();
	m_shape->setOrintation(m_shape->getAbsoluteOrintation());
}

}
}

