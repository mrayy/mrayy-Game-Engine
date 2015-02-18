
#include "MissileLauncher.h"
#include "GameLevel.h"
#include "GameOptionManager.h"

#include <sanimatedmesh.h>
#include <XMLNodeReader.h>
#include <IFileSystem.h>
#include <TangentCalculater.h>
#include <SMeshManipulator.h>


namespace mray{
namespace gameMod{

MissileLauncher::MissileLauncher(GameLevel*level,GCPtr<scene::ISceneNode> shape,
							   const core::string&missile)
{
	setCollisionGroup(0x2000);

	m_exploded=0;

	m_missileTimeOut=10;

	m_LaunchTimeOutCount=15;
	m_LaunchTimeOut=5;

	m_level=level;
	m_missile=missile;
	m_shape=shape;
}
MissileLauncher::~MissileLauncher(){
}

void MissileLauncher::update(float dt){
	if(m_exploded)
		return;

	if(m_level->isPaused())
		return;


	if(m_level->getPlayer()->isDestroyed())
		return;

	if(m_AntiPlaneMissile && m_AntiPlaneMissile->isDestroyed()==0)
		return;

	if(m_LaunchTimeOut<=0){
		m_AntiPlaneMissile=createMissile();
		m_AntiPlaneMissile->LaunchMissile(m_level->getPlayer());
		m_LaunchTimeOut=m_LaunchTimeOutCount;
	}
	m_LaunchTimeOut-=dt;
}


GCPtr<AntiPlaneMissile> MissileLauncher::createMissile(){
	scene::XMLNodeReader reader;
	GCPtr<scene::IAnimatedMesh> ent=reader.loadXML(gFileSystem.openFile(m_missile));;

	if(!ent)return 0;

	scene::TangentCalculater calc;

	scene::SMeshManipulator::getInstance().recalcNormalsSmooth(ent->getMesh());
	calc.calculateTangents(ent->getMesh());

	m_level->getSceneManager()->addSceneNode(ent);
	GCPtr<AntiPlaneMissile> m=new AntiPlaneMissile(m_level,ent,this);
	return m;

}

void MissileLauncher::setPosition(const math::vector3d& r){
	m_shape->position=r;
}
void MissileLauncher::setRotation(const math::quaternion&p){
	m_shape->setOrintation(p);
}
void MissileLauncher::setScale(const math::vector3d&p){
	m_shape->scale=p;
}

math::vector3d MissileLauncher::getPosition(){
	return m_shape->position;
}
math::quaternion MissileLauncher::getRotation(){
	return m_shape->getOrintation();
}
math::vector3d MissileLauncher::getScale(){
	return m_shape->getScale();
}


math::box3d MissileLauncher::getBoundingBox(){
	return m_shape->getTransformedBoundingBox();
}

void MissileLauncher::hasCollide(){
	if(m_exploded){
		return;
	}
	m_shape->Visible=false;
	//m_level->getSceneManager()->removeChild(m_shape);
	if(m_explosion){
		m_level->getSceneManager()->addSceneNode(m_explosion);

		m_explosion->setPosition(m_shape->position);
		m_explosion->setScale(50);
		m_explosion->setFrame(0);
	}
	m_exploded=true;
	m_level->getSoundManager()->playSound(mT("Game/Explosion/Exp2.wav"),m_shape->position,false,GameOptionManager::getInstance().gameSoundVol);

}

}
}
