
#include <Randomizer.h>
#include "StaticEntity.h"
#include "GameOptionManager.h"



namespace mray{
namespace gameMod{

StaticEntity::StaticEntity(GCPtr<scene::ISceneNode> shape){
	m_shape=shape;
	m_id=0;
}
StaticEntity::~StaticEntity(){
}



void StaticEntity::setPosition(const math::vector3d&p){
	if(m_shape)
		m_shape->setPosition(p);
}
void StaticEntity::setRotation(const math::quaternion&p){
	if(m_shape)
		m_shape->setOrintation(p);
}
void StaticEntity::setScale(const math::vector3d&p){
	if(m_shape){
		m_shape->setScale(p);
		m_shape->ViewDist=math::Randomizer::randRange(700,1500)*p.Length()*
			GameOptionManager::getInstance().viewDistance*0.01f;
	}
}

math::vector3d StaticEntity::getPosition(){
	if(m_shape)
		return m_shape->getPosition();
	return 0;
}
math::quaternion StaticEntity::getRotation(){
	if(m_shape)
		return m_shape->getOrintation();
	return math::quaternion::Identity;
}
math::vector3d StaticEntity::getScale(){
	if(m_shape)
		return m_shape->getScale();
	return 1;
}

void StaticEntity::update(float dt){
}

math::box3d StaticEntity::getBoundingBox(){
	return m_shape->getTransformedBoundingBox();
}

}
}


