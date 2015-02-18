
#include "Missile.h"
#include "GameLevel.h"
#include "TextureResourceManager.h"
#include "IEaglesManager.h"
#include "JetParticleSystem.h"
#include "GameOptionManager.h"
#include <mrayParticles.h>
#include <SBillboardChain.h>

namespace mray{
namespace gameMod{

class MissileParticleSystem:public scene::SParticleSystem
{
private:
protected:
	math::vector3d m_a;
	math::vector3d m_b;
	virtual void setupParticlePosition(GCPtr<scene::IParticle>particle){
		float t=math::Randomizer::rand01();
		particle->position.x=JetRadius*(math::Randomizer::rand01() - 0.5f);
		particle->position.y=JetRadius*(math::Randomizer::rand01() - 0.5f);
		particle->position.z=JetRadius*(math::Randomizer::rand01() - 0.5f);
		particle->position+=(m_b-m_a)*t;
	}
public:

	float JetRadius;

	MissileParticleSystem(){
		JetRadius=1;
	}
	virtual~MissileParticleSystem(){
	}

	virtual void update(float dt){
		m_a=getAbsolutePosition();
		scene::SParticleSystem::update(dt);
		m_b=getAbsolutePosition();
	}
};


Missile::Missile(GameLevel*level,GCPtr<scene::ISceneNode> shape,GameEntity*owner){
	m_owner=owner;
	m_shape=shape;
	m_level=level;

	setCollisionGroup(owner->getCollisionGroup());

	m_minHeight=0;

	m_Launched=false;

	m_timeToLive=15;//

	m_exploded=false;

	m_speed=0;
	m_acceleration=100;
	m_maxSpeed=250;

	{
		GCPtr<video::TextureBank> bank=new video::TextureBank();
		for(int i=0;i<24;i++){
			core::string name=mT("PlaneExp/");
			name+=core::StringConverter::toString(i);
			name+=mT(".png");
			bank->addTexture(gTextureResourceManager.loadTexture2D(name));
		}
		//for(int i=0;i<4;i++)
		{
			m_explosion=new scene::BankAnimatedSprite(bank,0,0,math::quaternion::Identity,100,0);
			// 			m_explosion->position().x=(math::Randomizer::rand01()*2-1)*10;
			// 			m_explosion->position().y=(math::Randomizer::rand01()*2-1)*10;
			// 			m_explosion->position().z=(math::Randomizer::rand01()*2-1)*10;
			// 			m_explosion->scale=(math::Randomizer::rand01()*2-1)*30+20;
			m_explosion->SetSceneManager(level->getSceneManager());
			m_explosion->setDevice(eagles::IEaglesManager::getInstance().getVideoDevice());
			m_explosion->setAutoAnimate(1);
			m_explosion->setAutoReplay(false);
			m_explosion->setSpeed(100);
			m_explosion->getMaterial(0)->setMaterialRenderer(video::MRT_ADD);
			m_explosion->getMaterial(0)->setRenderState(video::RS_Lighting,video::ES_DontUse);
			m_explosion->getMaterial(0)->setRenderState(video::RS_ZWrite,video::ES_DontUse);

			//m_shape->addChild(m_explosion);
		}
	}
	if(0)
	{

		GCPtr<scene::PAColorInterpolater> pca=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ColorInterpolater"));
		pca->addKey(0  ,video::SColor(255,255,255,255));
		pca->addKey(0.3  ,video::SColor(255,255,255,255));
		pca->addKey(0.5  ,video::SColor(214,211,180,200));
		pca->addKey(1  ,video::SColor(214,211,180,0));


		GCPtr<scene::PAScaleInterpolater> psa=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ScaleInterpolater"));
		psa->addKey(0  ,1.2);
		psa->addKey(0.3  ,1.2);
		psa->addKey(0.5 ,2,3);
		psa->addKey(1 ,7,10);


		GCPtr<scene::PAVelocityInterpolater> pva=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("VelocityInterpolater"));
		pva->addKey(0  ,0);
		pva->addKey(0.5 ,0);
		pva->addKey(1 ,math::vector3d(-3,4,-3),math::vector3d(3,4,3));

		GCPtr<MissileParticleSystem> ps=new MissileParticleSystem();
		m_smoke=ps;
		ps->JetRadius=1;

		m_smoke->SetSceneManager(m_level->getSceneManager());
		m_smoke->setDevice(eagles::IEaglesManager::getInstance().getVideoDevice());
		m_smoke->setParticleShapeType(scene::EPS_Billboard);
		m_smoke->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke.tga")),0);
		m_smoke->getMaterial()->setMaterialRenderer(video::MRT_ADDALPHA);
		m_smoke->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
		m_smoke->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);

		m_smoke->particlesRate.clear();
		m_smoke->particlesRate.addKey(0,20);

		m_smoke->particlesCount.clear();
		m_smoke->particlesCount.addKey(0,-1);

		m_smoke->particlesLifeSpawn.clear();
		m_smoke->particlesLifeSpawn.addKey(0,10);

		m_smoke->addAffector(pca);
		m_smoke->addAffector(psa);
		m_smoke->addAffector(pva);
	}
}

Missile::~Missile(){
}




void Missile::update(float dt){

	if(!m_Launched)return;
	if(m_exploded){
		if(m_smoke && m_smoke->isEmpty){
			m_level->getSceneManager()->removeChild(m_smoke);
			m_smoke=0;
		}
		if(m_explosion && m_explosion->isDone()){
			m_level->getSceneManager()->removeChild(m_explosion);
			m_explosion=0;
		}
		return;
	}

	m_timeToLive-=dt;

	if(m_timeToLive<=0){
		hasCollide();
		return;
	}

	math::vector3d pos=0;

	m_shape->getAbsoluteTransformation().transformNormalPre(math::vector3d(0,0,m_speed)*dt,pos);
	m_shape->position()+=pos;

	m_speed+=m_acceleration*dt;
	if(m_speed>m_maxSpeed){
		m_speed=m_maxSpeed;
	}

	float m_height=(getPosition().y);
	m_minHeight=m_level->getHeight(getPosition().x,getPosition().z);
	m_height-=m_minHeight;
	if(m_height<0)
	{
		hasCollide();
	}
}

void Missile::hasCollide(){
	if(m_exploded)
		return;
	m_shape->position().y+=5;
	m_shape->Visible=false;
	//m_level->getSceneManager()->removeChild(m_shape);
	if(m_explosion){
		m_level->getSceneManager()->addSceneNode(m_explosion);

		m_explosion->setPosition(m_shape->position);
		m_explosion->setScale(50);
		m_explosion->setFrame(0);
	}

	if(m_smoke){
		m_smoke->position=m_smoke->getAbsolutePosition();
		m_smoke->stopEmitting();
		m_smoke->removeFromParent();
		m_level->getSceneManager()->addSceneNode(m_smoke);
	}

	m_exploded=true;
	m_level->getSoundManager()->playSound(mT("Game/Explosion/Exp1.wav"),m_shape->position,false,GameOptionManager::getInstance().gameSoundVol);
}


void Missile::LaunchMissile(){
	if(m_Launched)
		return;
	m_Launched=true;
	m_shape->removeFromParent();
	m_level->addEntity(this,false);
	m_shape->addChild(m_smoke);


	{

		GCPtr<scene::SBillboardChain> b=m_level->getSceneManager()->addBillboardChain(2,4,20,0);
		b->setRecordInterval(0.2);
		b->setTexture(gTextureResourceManager.loadTexture2D(mT("windBeam.tga")));
		m_shape->addChild(b);
		b->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
		m_level->getSceneManager()->removeChild(b);
	}

	m_level->getSoundManager()->playSound(mT("Game/Missile.wav"),m_shape->position,false,GameOptionManager::getInstance().gameSoundVol);
}


void Missile::setPosition(const math::vector3d&p){
	if (m_shape)
		return m_shape->setPosition(p);
}
void Missile::setRotation(const math::quaternion&p){
	if (m_shape)
		return m_shape->setOrintation(p);
}
void Missile::setScale(const math::vector3d&p){
	if (m_shape)
		return m_shape->setScale(p);
}

math::vector3d Missile::getPosition(){
	if (m_shape)
		return m_shape->getPosition();
	return 0;
}
math::quaternion Missile::getRotation(){
	if (m_shape)
		return m_shape->getOrintation();
	return math::quaternion::Identity;
}
math::vector3d Missile::getScale(){
	if (m_shape)
		return m_shape->getScale();
	return 1;
}

GCPtr<scene::ISceneNode> Missile::getShape(){
	return m_shape;
}

math::box3d Missile::getBoundingBox(){
	return m_shape->getTransformedBoundingBox()+math::box3d(-1,1);
}


}
}