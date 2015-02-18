
#include "RocketTest.h"
#include <TextureResourceManager.h>
#include <mrayParticles.h>
#include <ShaderResourceManager.h>
#include <VideoValues.h>
#include <Randomizer.h>



namespace mray{


RocketTest::RocketTest(scene::SceneManager*mngr,const math::vector3d&pos,const math::quaternion&orintation){
	m_shape=mngr->addSphere(0.5,10,10,pos,orintation);
	m_smoke=mngr->addParticleSystem(-1,100,20,0,mT(""),m_shape);
	m_flare=mngr->addBillboard(6,math::vector3d(0,0,0),mT(""),m_shape);
	m_trail=mngr->addBillboardChain(3,3,20,0,mT(""),m_shape);

	m_smoke->Visible=true;
	m_trail->setRecordInterval(0.05);

// 	m_shape->addChild(m_smoke);
// 	m_shape->addChild(m_flare);
// 	m_shape->addChild(m_trail);


	GCPtr<video::ITexture>tex=gTextureResourceManager.loadTexture2D(mT("particlewhite.bmp"));
	m_flare->setTexture(tex);
	m_flare->getMaterial(0)->setMaterialRenderer(video::MRT_ADDALPHA);
	m_flare->getMaterial(0)->setRenderState(video::RS_Lighting,video::ES_DontUse);

	m_smoke->getMaterial()->shader=gShaderResourceManager.loadShader("phong.cg","phong.cg",mT("cg"));
	m_smoke->setParticleShapeType(scene::EPS_Billboard);
	m_smoke->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("black_smoke1.tga")),0);
	m_smoke->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
	m_smoke->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
	m_smoke->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);
// 
// 	m_trail->getMaterial()->diffuseColor.Set(255);
	m_trail->getMaterial()->shader=gShaderResourceManager.loadShader("phong.cg","phong.cg",mT("cg"));
	m_trail->getMaterial()->addValue(new TextureValue(mT("baseTex"),gTextureResourceManager.loadTexture2D(mT("smokeTrail03.tga"))));;
	m_trail->getMaterial()->addValue(new TextureValue(mT("bumpTex"),gTextureResourceManager.loadTexture2D(mT("smoke_trail04_line_local.tga"))));;
	m_trail->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);


	GCPtr<scene::PAColorInterpolater> pca=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ColorInterpolater"));
	pca->addKey(0.8  ,video::SColor(255));
	pca->addKey(1  ,video::SColor(255,255,255,0));


	GCPtr<scene::PAScaleInterpolater> psa=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("ScaleInterpolater"));
	psa->addKey(0  ,3);

	GCPtr<scene::PARotationInterpolater> pra=scene::SParticleSystemFactory::getInstance().createParticleAffector(mT("RotationInterpolater"));
	pra->addKey(0  ,0,30);
	pra->addKey(1  ,50,180);
	m_smoke->addAffector(pra);

	m_smoke->addAffector(pca);
	m_smoke->addAffector(psa);

	m_speed.set(0,0,50);
	m_speed=orintation*m_speed;
	m_acceleration.set(0,-8,0);
	m_time=0;
	m_mngr=mngr;
}
RocketTest::~RocketTest(){
}

void RocketTest::update(float dt){
	if(!m_shape)return;
	m_shape->translate(m_speed*dt,scene::ETransformSpace::TS_World);
	m_flare->setSize(5+math::Randomizer::randRange(-1,1));
	m_speed+=m_acceleration*dt;
	m_time+=dt;
	if(m_time>3){
 		m_mngr->removeChild(m_shape);
		m_smoke->stopEmitting();
		m_shape=0;
		//m_smoke=0;
		m_smoke->position=m_smoke->getAbsolutePosition();
		m_smoke->removeFromParent();
		m_mngr->addSceneNode(m_smoke);
		m_trail=0;
		m_flare=0;
	}
}


}
