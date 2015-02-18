

#include "AirPlane.h"
#include "EnvironmentManager.h"
#include "GameLevel.h"
#include "JetParticleSystem.h"
#include "IEaglesManager.h"
#include "GameOptionManager.h"

#include <MaterialResourceManager.h>
#include <TangentCalculater.h>
#include <SMeshManipulator.h>
#include <videoValues.h>
#include <mrayParticles.h>
#include <TextureResourceManager.h>
#include <SBillboardChain.h>
#include <sanimatedmesh.h>
#include <XMLNodeReader.h>
#include <IFileSystem.h>
#include <FontResourceManager.h>

namespace mray{
namespace gameMod{

const math::vector3d AirPlane::RotateSpeed(20,10,30);
const float AirPlane::RotateSpeedDamping=0.9;

AirPlane::AirPlane(GameLevel*level,GCPtr<scene::ISceneNode> shape,const core::string&missile){


	setCollisionGroup(0x1000);

	m_missile=missile;

	m_missileTimeOut=10;//10 sec

	m_leftMissileTime=2;
	m_rightMissileTime=2;

	m_LaunchTimeOutCount=5;
	m_LaunchTimeOut=0;


	m_turbo=0;
	m_targetTurbo=0;
	m_exploded=0;

	m_level=level;

	m_targetBrake=0;
	m_brake=0;

	m_shape=shape;

	m_baseSpeed=100;
	m_speed=0;
	m_minHeight=0;

	m_height=0;

	m_radius=4;

	m_jetSound=m_level->getSoundManager()->loadSound(mT("Game/Jet.wav"),0,sound::ESNDT_3D);
	m_jetSound->setLooping(true);

	scene::TangentCalculater calc;

	m_mtrl=gMaterialResourceManager.getMaterial(mT("F15Material"));
	if(m_mtrl){
		m_mtrl->addValue(new TextureValue(mT("envTex"),EnvironmentManager::getInstance().getSkyTexture()));\
	}

#define PREPARE_NODE(x) \
	if(m_mtrl){\
		for(int i=0;i<x->getMaterialCount();i++){\
			x->setMaterial(m_mtrl,i);\
		}\
	}
#define CALC_NORMAL_SMOOTH(x) \
	scene::SMeshManipulator::getInstance().recalcNormalsSmooth(x->getMesh());\
	calc.calculateTangents(x->getMesh());
#define CALC_NORMAL_HARD(x) \
	scene::SMeshManipulator::getInstance().recalcNormalsFlat(x->getMesh());\
	calc.calculateTangents(x->getMesh());

	PREPARE_NODE(shape);
	CALC_NORMAL_SMOOTH(shape);
	{
		GCPtr<scene::MeshSceneNode> blade=level->getSceneManager()->addAnimatedMesh(mT("blade_left.mdl"));
		if(blade){
			m_bladeLeft=blade;
			PREPARE_NODE(blade);
			CALC_NORMAL_HARD(blade);
			m_shape->attachToMountPoint(mT("loc_left"),blade);
			level->getSceneManager()->removeChild(blade);
		}
	}
	{
		GCPtr<scene::MeshSceneNode> blade=level->getSceneManager()->addAnimatedMesh(mT("blade_right.mdl"));
		if(blade){
			m_bladeRight=blade;
			PREPARE_NODE(blade);
			CALC_NORMAL_HARD(blade);
			m_shape->attachToMountPoint(mT("loc_right"),blade);
			level->getSceneManager()->removeChild(blade);
		}
	}
	{
		GCPtr<scene::MeshSceneNode> blade=level->getSceneManager()->addAnimatedMesh(mT("blade_topBack.mdl"),
			0,math::quaternion::Identity,1,mT(""),0,0,1);
		if(blade){
			CALC_NORMAL_HARD(blade);
			PREPARE_NODE(blade);
			m_bladeBTopRight=blade;
			m_shape->attachToMountPoint(mT("loc_rightBackTop"),blade);
			level->getSceneManager()->removeChild(blade);
		}
	}
	{
		GCPtr<scene::MeshSceneNode> blade=level->getSceneManager()->addAnimatedMesh(mT("blade_topBack.mdl"));
		if(blade){
			CALC_NORMAL_HARD(blade);
			PREPARE_NODE(blade);
			m_bladeBTopLeft=blade;
			m_shape->attachToMountPoint(mT("loc_leftBackTop"),blade);
			level->getSceneManager()->removeChild(blade);
		}
	}
	{
		GCPtr<scene::MeshSceneNode> blade=level->getSceneManager()->addAnimatedMesh(mT("blade_rightBotBack.mdl"),
			0,math::quaternion::Identity,1,mT(""),0,0,1);
		if(blade){
			CALC_NORMAL_HARD(blade);
			PREPARE_NODE(blade);
			m_bladeBBotRight=blade;
			m_shape->attachToMountPoint(mT("loc_rightBackBot"),blade);
			level->getSceneManager()->removeChild(blade);
		}
	}
	{
		GCPtr<scene::MeshSceneNode> blade=level->getSceneManager()->addAnimatedMesh(mT("blade_leftBotBack.mdl"));
		if(blade){
			CALC_NORMAL_HARD(blade);
			PREPARE_NODE(blade);
			m_bladeBBotLeft=blade;
			m_shape->attachToMountPoint(mT("loc_leftBackBot"),blade);
			level->getSceneManager()->removeChild(blade);
		}
	}
	{
		GCPtr<scene::SMountPoint> point;
		point= m_shape->getMountPoint(mT("loc_leftMost"));
		;
		if(point){
			GCPtr<scene::SBillboardChain> b=level->getSceneManager()->addBillboardChain(0.4,0.4,30,0);
			b->setRecordInterval(0.1);
			b->setTexture(gTextureResourceManager.loadTexture2D(mT("windBeam.tga")));
			point->addChild(b);
			b->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
			level->getSceneManager()->removeChild(b);
		}
		point= m_shape->getMountPoint(mT("loc_rightMost"));
		if(point){
			GCPtr<scene::SBillboardChain> b=level->getSceneManager()->addBillboardChain(0.4,0.4,30,0);
			b->setRecordInterval(0.1);
			b->setTexture(gTextureResourceManager.loadTexture2D(mT("windBeam.tga")));
			point->addChild(b);
			b->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
			level->getSceneManager()->removeChild(b);
		}
	}
	{

		GCPtr<scene::PAColorInterpolater> pca=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ColorInterpolater"));
		pca->addKey(0  ,video::SColor(0,0,255,128));
		pca->addKey(0.8  ,video::SColor(255,255,255,200));
		pca->addKey(1  ,video::SColor(255,255,255,0));


		GCPtr<scene::PAScaleInterpolater> psa=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ScaleInterpolater"));
		psa->addKey(0  ,1.2);
		psa->addKey(1 ,0.5,0.8);

		GCPtr<scene::PARotationInterpolater> pra=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("RotationInterpolater"));
		pra->addKey(0  ,0,100);
		pra->addKey(1  ,200,300);

		{
			GCPtr<JetParticleSystem> ps=new JetParticleSystem();
			ps->SetSceneManager(level->getSceneManager());
			ps->setDevice(eagles::IEaglesManager::getInstance().getVideoDevice());
			ps->JetRadius=0.1;
			ps->acceleration=math::vector3d(0,0,0);
			ps->setParticleShapeType(scene::EPS_Billboard);
			ps->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke.tga")),0);
			ps->getMaterial()->setMaterialRenderer(video::MRT_ADDALPHA);
			ps->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
			ps->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);

			ps->particlesRate.clear();
			ps->particlesRate.addKey(0,30);

			ps->particlesCount.clear();
			ps->particlesCount.addKey(0,-1);

			ps->particlesLifeSpawn.clear();
			ps->particlesLifeSpawn.addKey(0,0.2);

			ps->addAffector(pra);
			ps->addAffector(pca);
			ps->addAffector(psa);

			m_shape->attachToMountPoint(mT("loc_rightEngine"),ps);

			m_rightEngine=ps;
		}
		{
			GCPtr<JetParticleSystem> ps=new JetParticleSystem();
			ps->SetSceneManager(level->getSceneManager());
			ps->setDevice(eagles::IEaglesManager::getInstance().getVideoDevice());
			ps->JetRadius=0.1;
			ps->acceleration=math::vector3d(0,0,0);
			ps->setParticleShapeType(scene::EPS_Billboard);
			ps->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke.tga")),0);
			ps->getMaterial()->setMaterialRenderer(video::MRT_ADDALPHA);
			ps->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
			ps->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);

			ps->particlesRate.clear();
			ps->particlesRate.addKey(0,30);

			ps->particlesCount.clear();
			ps->particlesCount.addKey(0,-1);

			ps->particlesLifeSpawn.clear();
			ps->particlesLifeSpawn.addKey(0,0.2);

			ps->addAffector(pra);
			ps->addAffector(pca);
			ps->addAffector(psa);

			m_shape->attachToMountPoint(mT("loc_leftEngine"),ps);

			m_leftEngine=ps;
		}
	}

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

	{
		m_leftMissile=createMissile();
		m_rightMissile=createMissile();

		m_shape->attachToMountPoint(mT("loc_leftMissile"),m_leftMissile->getShape());
		m_shape->attachToMountPoint(mT("loc_rightMissile"),m_rightMissile->getShape());
 		level->getSceneManager()->removeChild(m_leftMissile->getShape());
 		level->getSceneManager()->removeChild(m_rightMissile->getShape());
	}

	if(0)
	{
		m_heightTxt=new scene::STextNode(m_shape,gFontResourceManager.getDefaultFont(),0,math::quaternion::Identity,1,0);
		m_heightTxt->SetSceneManager(level->getSceneManager());
		m_heightTxt->setDevice(eagles::IEaglesManager::getInstance().getVideoDevice());
		m_shape->addChild(m_heightTxt);
		m_heightTxt->position().x=3;
		m_heightTxt->position().y=1;
		m_heightTxt->setText(mT(""));
		m_heightTxt->getAttrs()->fontSize=300;
	}

	{
		m_leftMissileGUI=m_level->getGUIManager()->addImage(math::vector2d(0),math::vector2d(32,128));
		m_rightMissileGUI=m_level->getGUIManager()->addImage(math::vector2d(0),math::vector2d(32,128));
		m_leftMissileGUI->addEffector(m_level->getGUIManager()->addAnchorEffector(GUI::EPAnchor_BR,math::vector2d(-50,50)));
		m_rightMissileGUI->addEffector(m_level->getGUIManager()->addAnchorEffector(GUI::EPAnchor_BR,math::vector2d(-90,50)));
		m_leftMissileGUI->setTexture(gTextureResourceManager.loadTexture2D(mT("missileGUI.png")));
		m_rightMissileGUI->setTexture(gTextureResourceManager.loadTexture2D(mT("missileGUI.png")));

		m_stopGUI=m_level->getGUIManager()->addImage(math::vector2d(0),math::vector2d(128,128));
		m_stopGUI->addEffector(m_level->getGUIManager()->addAnchorEffector(GUI::EPAnchor_BR,math::vector2d(-90+64,50)));
		m_stopGUI->setTexture(gTextureResourceManager.loadTexture2D(mT("Stop.png")));
		m_stopGUI->Visible=false;
	}
}
AirPlane::~AirPlane(){
}

void AirPlane::doAction(EPlaneAction action,bool press){

	if(m_exploded)
		return;
	if(m_level->isPaused())
		return;

	switch(action){
		case EPA_Brake:
			m_targetBrake=press*0.8;
			break;
		case EPA_Turbo:
			m_targetTurbo=press;
			break;
		case EPA_PitchUp:
			if(press)
				m_targetRotateAngle.x=1;
			break;
		case EPA_PitchDown:
			if(press)
				m_targetRotateAngle.x=-1;
			break;
		case EPA_YawLeft:
			if(press)
				m_targetRotateAngle.y=1;
			break;
		case EPA_YawRight:
			if(press)
				m_targetRotateAngle.y=-1;
			break;
		case EPA_RollLeft:
			if(press)
				m_targetRotateAngle.z=-1;
			break;
		case EPA_RollRight:
			if(press)
				m_targetRotateAngle.z=1;
			break;
		case EPA_Missile:
			if(press){
				if(m_LaunchTimeOut==0){
					if(m_leftMissileTime==0 && m_leftMissile){
						m_leftMissile->LaunchMissile();
						m_leftMissileTime=m_missileTimeOut;
						m_leftMissile=0;
						m_LaunchTimeOut=m_LaunchTimeOutCount;
					}else if(m_rightMissileTime==0 && m_rightMissile){
						m_rightMissile->LaunchMissile();
						m_rightMissileTime=m_missileTimeOut;
						m_rightMissile=0;
						m_LaunchTimeOut=m_LaunchTimeOutCount;
					}
				}
			}break;
	}

}

void AirPlane::update(float dt){
	if(m_exploded)
		return;

	if(m_level->isPaused())
		return;

	if(m_jetSound){
		if(!m_jetSound->isPlaying()){
			m_jetSound->play();
		}
		m_jetSound->setPosition(m_shape->position);
	}
	m_rotateAngle=m_rotateSpeed*RotateSpeed*dt;
	float RotateSpeedDamping2=0;
	{
		math::vector3d rotateNorm=m_rotateSpeed;
		float dragCoff=rotateNorm.Length();
		dragCoff=RotateSpeedDamping*dragCoff + RotateSpeedDamping2*dragCoff*dragCoff;

		rotateNorm.Normalize();
	}
	if(m_bladeLeft){
		math::quaternion q(-45*m_rotateAngle.z,math::vector3d::XAxis);
		m_bladeLeft->setOrintation(q);
	}
	if(m_bladeRight){
		math::quaternion q(45*m_rotateAngle.z,math::vector3d::XAxis);
		m_bladeRight->setOrintation(q);
	}
	{
		math::quaternion q(-45*m_rotateAngle.x,math::vector3d::XAxis);
		if(m_bladeBBotRight){
			m_bladeBBotRight->setOrintation(q);
		}
		if(m_bladeBBotLeft){
			m_bladeBBotLeft->setOrintation(q);
		}
	}
	{
		math::quaternion q(-45*m_rotateAngle.y,math::vector3d::YAxis);
		if(m_bladeBTopRight){
			m_bladeBTopRight->setOrintation(q);
		}
		if(m_bladeBTopLeft){
			m_bladeBTopLeft->setOrintation(q);
		}
	}

	m_speed=(m_baseSpeed+m_baseSpeed*0.5*m_turbo)*(1-m_brake);

	if(m_shape)
	{
//
		math::quaternion q(m_rotateAngle);
		m_shape->setOrintation(q*m_shape->getOrintation());


		//m_shape->translate(math::vector3d(0,0,50)*dt,scene::TS_Local);
		math::vector3d dir;
		m_shape->getRelativeTransformation().transformNormalPre(math::vector3d(0,0,1),dir);

		float groundAccel=-dir.dotProduct(math::vector3d::YAxis)*9.8;

		math::vector3d upDir=dir;
		upDir.y=(upDir.y)*math::sign(groundAccel);
		groundAccel=abs(groundAccel);

		m_shape->position()+=dir*dt*m_speed+upDir*groundAccel*dt;

	}

	m_leftEngine->particlesRate.clear();
	m_rightEngine->particlesRate.clear();

	m_leftEngine->particlesRate.addKey(0,30*(1+0.5*m_turbo)*(1-m_brake));
	m_rightEngine->particlesRate.addKey(0,30*(1+0.5*m_turbo)*(1-m_brake));

	if(m_targetTurbo!=0){
		m_turbo+=2*dt;
		m_turbo=math::Min<float>(m_turbo,m_targetTurbo);
	}else{
		m_turbo-=2*dt;
		m_turbo=math::Max<float>(m_turbo,0);
	}

	if(m_targetBrake!=0){
		m_brake+=0.2*dt;
		m_brake=math::Min<float>(m_brake,m_targetBrake);
	}else{
		m_brake-=0.4*dt;
		m_brake=math::Max<float>(m_brake,0);
	}

	for(int i=0;i<3;i++){
		if(m_targetRotateAngle[i]!=0){
			float s=math::sign(m_targetRotateAngle[i]);
			m_rotateSpeed[i]+=s*5*dt;
			if(s>0)
				m_rotateSpeed[i]=math::Min<float>(m_rotateSpeed[i],m_targetRotateAngle[i]);
			else
				m_rotateSpeed[i]=math::Max<float>(m_rotateSpeed[i],m_targetRotateAngle[i]);
		}else{
			float s=math::sign(m_rotateSpeed[i]);
			m_rotateSpeed[i]-=s*5*dt;
			if(s>0)
				m_rotateSpeed[i]=math::Max<float>(m_rotateSpeed[i],0);
			else
				m_rotateSpeed[i]=math::Min<float>(m_rotateSpeed[i],0);
		}
	}


	m_targetRotateAngle=0;

	if(m_LaunchTimeOut>0){
		m_stopGUI->Visible=true;
		m_LaunchTimeOut-=dt;
		if(m_LaunchTimeOut<0){
			m_stopGUI->Visible=false;
			m_LaunchTimeOut=0;
		}
	}

	if(m_leftMissileTime>0 ){
		m_leftMissileTime-=dt;
		m_leftMissileGUI->Color().Set(255,0,0,255*(1-(m_leftMissileTime/m_missileTimeOut)));
		if(m_leftMissileTime<=0){
			m_leftMissileTime=0;
			m_leftMissileGUI->Color().Set(255,255,255,255);
			if(!m_leftMissile){
				m_leftMissile=createMissile();
				m_shape->attachToMountPoint(mT("loc_leftMissile"),m_leftMissile->getShape());
				m_level->getSceneManager()->removeChild(m_leftMissile->getShape());
			}
		}
	}
	if(m_rightMissileTime>0 ){
		m_rightMissileTime-=dt;
		m_rightMissileGUI->Color().Set(255,0,0,255*(1-(m_rightMissileTime/m_missileTimeOut)));
		if(m_rightMissileTime<=0){
			m_rightMissileTime=0;
			m_rightMissileGUI->Color().Set(255,255,255,255);
			if(!m_rightMissile){
				m_rightMissile=createMissile();
				m_shape->attachToMountPoint(mT("loc_rightMissile"),m_rightMissile->getShape());
				m_level->getSceneManager()->removeChild(m_rightMissile->getShape());
			}
		}
	}


	m_height=(getPosition().y-getRadius());
	m_minHeight=m_level->getHeight(getPosition().x,getPosition().z);
	m_height-=m_minHeight;

	if(m_heightTxt)
		m_heightTxt->setText(core::StringConverter::toString(m_height,7).c_str());

	if(m_height<0)
	{
		hasCollide();
	}
}
void AirPlane::resetForce(const math::vector3d&f){
	m_force=f;
}
void AirPlane::applyForce(const math::vector3d&f){
	m_force+=f;
}

math::vector3d AirPlane::getRotationAngles(){
	return m_rotateAngle;
}
math::vector3d AirPlane::getPosition(){
	return m_shape->position;
}
math::quaternion AirPlane::getRotation(){
	return m_shape->getOrintation();
}
math::vector3d AirPlane::getScale(){
	return m_shape->getScale();
}

void AirPlane::setRotationAngles(const math::vector3d& r){
	m_shape->setOrintation(r);
}
void AirPlane::setPosition(const math::vector3d& r){
	m_shape->position=r;
}
void AirPlane::setRotation(const math::quaternion&p){
	m_shape->setOrintation(p);
}
void AirPlane::setScale(const math::vector3d&p){
	m_shape->scale=p;
}

void AirPlane::setRadius(float r){
	m_radius=r;
}
float AirPlane::getRadius(){
	return m_radius;
}

void AirPlane::hasCollide(){
	if(m_exploded){
		return;
	}
	m_shape->position().y+=m_radius;
	m_shape->Visible=false;
	//m_level->getSceneManager()->removeChild(m_shape);
	m_level->getSceneManager()->addSceneNode(m_explosion);

	m_explosion->setPosition(m_shape->position);
	m_explosion->setScale(50);
	m_explosion->setFrame(0);

	m_exploded=true;
	if(m_jetSound){
		m_jetSound->stop();
	}
	m_level->getSoundManager()->playSound(mT("Game/Explosion/Exp2.wav"),m_shape->position,false,GameOptionManager::getInstance().gameSoundVol);
}
bool AirPlane::isExplode(){
	return m_exploded;
}

GCPtr<PlaneMissile> AirPlane::createMissile(){
	scene::XMLNodeReader reader;
	GCPtr<scene::IAnimatedMesh> ent=reader.loadXML(gFileSystem.openFile(m_missile));;

	if(!ent)return 0;

 	for(int i=0;i<ent->getMaterialCount();i++){
 		ent->setMaterial(m_mtrl,i);
 	}
 	scene::TangentCalculater calc;
 
 	scene::SMeshManipulator::getInstance().recalcNormalsSmooth(ent->getMesh());
 	calc.calculateTangents(ent->getMesh());

	m_level->getSceneManager()->addSceneNode(ent);
	GCPtr<PlaneMissile> m=new PlaneMissile(m_level,ent,this);
	return m;
}

float AirPlane::getSpeed(){
	return m_speed;
}

math::box3d AirPlane::getBoundingBox(){
	return m_shape->getTransformedBoundingBox();
}



void AirPlane::addLockedMissile(GCPtr<Missile> m){
}
void AirPlane::removeLockedMissile(GCPtr<Missile> m){
}

GameEntity *AirPlane::getAsGameEntity(){
		return this;
}

}
}