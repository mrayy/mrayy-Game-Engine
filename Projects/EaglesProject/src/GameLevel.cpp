
#include "GameLevel.h"
#include "IEaglesManager.h"
#include "XMLLevelLoader.h"
#include "SoundManagerFactory.h"
#include "ObjectFactory.h"
#include "DemoCamera.h"
#include "PostProcessing.h"
#include "BloomPP.h"
#include "GlarePP.h"
#include "GameOptionManager.h"
#include "ScenarioExitCode.h"
#include "EnvironmentManager.h"
#include "MissileLauncher.h"

#include <IFileSystem.h>
#include <GUIImage.h>
#include <SAnimatedMesh.h>
#include <XMLNodeReader.h>
#include <GUIOverlayManager.h>
#include <KeyboardEvent.h>
#include <GUIEvent.h>
#include <SLensEffect.h>
#include <GenericValues.h>
#include <MathValues.h>
#include <MaterialResourceManager.h>
#include <TextureResourceManager.h>
#include <GUIStaticText.h>
#include <MeshGenerator.h>
#include <ICursorControl.h>
#include <ITimer.h>
#include <GUIDebug.h>
#include <CMultiPassPP.h>

namespace mray{
namespace gameMod{

	const int ResumeButtonId=100;
	const int QuitButtonId=101;

GameLevel::GameLevel(){

	m_state=0;
	m_pause=0;

	GCPtr<video::IVideoDevice> dev=eagles::IEaglesManager::getInstance().getVideoDevice();

	m_guiMngr=new GUI::GUIManager(dev);
	m_sndMngr=sound::SoundManagerFactory::getInstance().
		createSoundManager(eagles::IEaglesManager::getInstance().getSoundManagerType().c_str());
	m_sceneMngr=new scene::SceneManager(dev);
	m_sndMngr->enableListener(true);


	GCPtr<video::IRenderTarget> shadowRenderTarget=eagles::IEaglesManager::getInstance().getVideoDevice()->createRenderTarget(mT("shadowRenderTarget"),
		4096,video::EPixel_Float16_R,0);

	m_sceneMngr->setShadowMap(shadowRenderTarget);

	GCPtr<GUI::GUIImage>img=m_guiMngr->addImage(math::vector2d(10,10),math::vector2d(200,200));
	img->setTexture(shadowRenderTarget->getColorTexture());

	m_collisionManager=new CollisionManager();

	m_freeCamera=new DemoCamera(m_sceneMngr,200);
	m_freeCamera->position=math::vector3d(0,100,0);
	m_sceneMngr->addSceneNode(m_freeCamera);
	m_sndMngr->attachToListener(m_freeCamera);

	m_controller=new HumanController();

	m_windSound=m_sndMngr->loadSound(mT("Game/Wind.mp3"),1,sound::ESNDT_2D);

	m_dangerImage=m_guiMngr->addImage(0,1);
	m_dangerImage->setTexture(gTextureResourceManager.loadTexture2D(mT("Danger.png")));
	m_dangerImage->Visible=false;
	m_dangerImage->Enable=false;
	m_dangerImage->Dock=GUI::ED_FILL;
	m_dangerImage->Color().Set(255,0,0,128);

	GCPtr<video::CMultiPassPP> postProcessing=new video::CMultiPassPP(dev);

	m_sceneMngr->enableRenderTarget(1);
	m_sceneMngr->setRenderTargetSize(dev->getViewportRect().getSize());
	m_sceneMngr->enablePostProcessing(1);
	if(GameOptionManager::getInstance().postEffect){
 		postProcessing->addPostProcessor(new GlarePP(dev,m_sceneMngr->getRenderTarget()->getSize()));
		//m_sceneMngr->setPostProcessing(new GrayscalePP(dev));
	}
	m_gamePP=new InGamePP(dev,m_sceneMngr->getRenderTarget()->getSize());
	postProcessing->addPostProcessor(m_gamePP);
	m_sceneMngr->setPostProcessing(postProcessing);

	if(0)
	{
		GCPtr<GUI::GUIImage> img=m_guiMngr->addImage(10,200);
		img->setTexture(m_sceneMngr->getRenderTarget()->getDepthTexture());
	}

	GUI::GUIOverlayManager::getInstance().loadOverlay(mT("GameMenuOverlay.xml"));
	{
		GCPtr<GUI::GUIOverlay> o= GUI::GUIOverlayManager::getInstance().getOverlay(mT("GameMenuOverlay"));
		if(o){
			core::IteratorPair<GUI::ElementList> p(o->createElements());
			for (;!p.done();p++)
			{
				m_guiMngr->addCustomChild(*p);
				if((*p)->Name().equals_ignore_case(mT("GameMenu")))
					m_menuWnd=*p;
			}
		}	
	}

	m_planeCamera=new PlaneCamera(m_sceneMngr);
	m_sceneMngr->addSceneNode(m_planeCamera);

	if(m_menuWnd){
		m_menuWnd->Visible=false;
	}


	GCPtr<scene::SLightNode> light=m_sceneMngr->addLight(video::SColor(235,230,100,255),100
		,1,math::vector3d(10000));


	math::matrix4x4 projection;
	projection.buildProjectionMatrixPerspectiveFovLH(m_planeCamera->fovY,
		m_planeCamera->Aspect,1,500);

	light->setProjection(projection);

	light->setTarget(math::vector3d(313,0,-827));

	//light->position().x=-5000;
	light->position().z=0;
	//light->active=false;
	GCPtr<scene::SLensEffect> eff=new scene::SLensEffect(m_sceneMngr,100);
	eff->position().x=-100;
	eff->position().z=0;
	eff->addChild(light);
	m_sceneMngr->addSceneNode(eff);

	m_waterHeight=-math::Infinity;

	m_planeHeightTxt=m_guiMngr->addStaticText(mT(""),10,math::vector2d(200,30));
	m_planeHeightTxt->Background=false;
	m_planeHeightTxt->AutoSize=true;

	EnvironmentManager::getInstance().attachLightToSun(light);


	m_boundingBoxShape=m_sceneMngr->addBox(1,1,1);
	m_boundingBoxShape->setMaterial(gMaterialResourceManager.getMaterial(mT("LvlBoundingBox")));

	setLevelBoundingBox(math::box3d(-1000,1000));


	m_warningText=m_guiMngr->addStaticText(mT("Warning"),0,1);
	m_warningText->getFontAttributes()->fontColor.Set(255,0,0,255);
	m_warningText->getFontAttributes()->hasShadow=0;
	m_warningText->getFontAttributes()->fontSize=30;
	m_warningText->AutoSize=1;
	m_warningText->addEffector(m_guiMngr->addAnchorEffector(GUI::EPAnchor_B,math::vector2d(0,100)));
	m_warningText->Background=false;
	m_warningText->Visible=false;

	m_warningBar=m_guiMngr->addLoadingBar(1,10,0,math::vector2d(300,30));
	m_warningBar->addEffector(m_guiMngr->addAnchorEffector(GUI::EPAnchor_B,math::vector2d(0,60)));
	m_warningText->Visible=false;

/*
	GCPtr<GUI::GUIDebug> cameraDbg=new GUI::GUIDebug(m_guiMngr,math::vector2d(100,40),math::vector2d(400,400));
	m_guiMngr->addChild(cameraDbg);
	cameraDbg->addWatch(&m_freeCamera->position);*/

}
GameLevel::~GameLevel(){
	m_gameEntities.clear();

	m_oldGuiMngr=0;
	m_oldSndMngr=0;
	m_oldSceneMngr=0;

	m_guiMngr=0;
	m_sndMngr=0;
	m_sceneMngr=0;

	m_freeCamera=0;

	m_terrain=0;
}

void GameLevel::setName(const core::string&name){
	m_name=name;
}

const core::string& GameLevel::getName(){
	return m_name;
}

void GameLevel::setLevelBoundingBox(const math::box3d&bb){
	m_boundingBox=bb;
	scene::MeshGenerator::getInstance().generateBox(m_boundingBox.getExtent(),m_boundingBoxShape->getMesh());
	m_boundingBoxShape->position=m_boundingBox.getCenter();
}

void GameLevel::addSceneNode(GCPtr<scene::ISceneNode>node){
	m_sceneMngr->addSceneNode(node);
}
void GameLevel::setTerrain(GCPtr<scene::ITerrain> terr){
	m_terrain=terr;
	m_sceneMngr->addSceneNode(m_terrain);
	//m_terrain->Visible=false;
}

GCPtr<scene::ITerrain> GameLevel::getTerrain(){
	return m_terrain;
}
void GameLevel::setPlayer(const core::string& type,const core::string& missile,const math::vector3d&pos,
			   const math::vector3d&rot,const math::vector3d&scale)
{
	scene::XMLNodeReader reader;
	GCPtr<scene::IAnimatedMesh> ent=reader.loadXML(gFileSystem.openFile(type));;

	if(!ent)
		return;


	m_planeCamera->attachToPlane(ent);

 	m_planeCamera->Visible=true;
 	m_freeCamera->Visible=false;
////
	m_sceneMngr->addSceneNode(ent);

	ent->scale=scale;


	m_player=new AirPlane(this,ent,missile);
	m_player->setRotation(rot);
	m_player->setPosition(pos);

	m_controller->attachToPlane(m_player);

	m_gameEntities.push_back(m_player);
}

void GameLevel::addAntiPlane(const core::string& type,const core::string& missile,const math::vector3d&pos,
				  const math::vector3d&rot,const math::vector3d&scale)
{

	scene::XMLNodeReader reader;
	GCPtr<scene::IAnimatedMesh> ent=reader.loadXML(gFileSystem.openFile(type));;

	if(ent)
		m_sceneMngr->addSceneNode(ent);

	GCPtr<MissileLauncher> ml=new MissileLauncher(this,ent,missile);
	ent->scale=scale;

	ml->setRotation(rot);
	ml->setPosition(pos);

	if(m_terrain){
		math::vector3d p=ent->getPosition();
		p.y=m_terrain->getHeight(p.x,p.z)+30;
		ent->setPosition(p);
	}

	m_gameEntities.push_back(ml);
}
void GameLevel::addEntity(GCPtr<GameEntity> ent,bool toTerrain){
	m_gameEntities.push_back(ent);

	if(m_terrain && toTerrain){
		math::vector3d p=ent->getPosition();
		p.y=m_terrain->getHeight(p.x,p.z);
		ent->setPosition(p);
	}

	m_sceneMngr->addSceneNode(ent->getShape());
}
void GameLevel::addEntity(const core::string& oclass,int type,const math::vector3d&pos,
			   const math::vector3d&rot,const math::vector3d&scale)
{
	GCPtr<GameEntity> ent=ObjectFactory::getInstance().createObject(oclass,type);
	if(!ent)
		return;
	ent->setRotation(rot);
	ent->setScale(scale);
	ent->setPosition(pos);

	addEntity(ent);
}



void GameLevel::createWaterSurface(float height,int detail,const math::vector2d& size)
{
	m_waterHeight=height;
	GCPtr<scene::MeshSceneNode> water=m_sceneMngr->addPlane(detail,detail,math::vector3d(0,height,0));
	water->scale=math::vector3d(size.x,1,size.y);
	
	GCPtr<video::SMaterial> mtrl=gMaterialResourceManager.getMaterial(mT("water_mtrl"));
	if(mtrl)
	{
		water->setMaterial(mtrl,0);
	}
	EnvironmentManager::getInstance().setWaterSurface(water);
}

void GameLevel::onEnter(){

	m_state=0;
	m_oldGuiMngr=eagles::IEaglesManager::getInstance().getActiveGUIManager();
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_guiMngr);

	m_oldSndMngr=eagles::IEaglesManager::getInstance().getActiveSoundManager();
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_sndMngr);

	m_oldSceneMngr=eagles::IEaglesManager::getInstance().getActiveSceneManager();
	eagles::IEaglesManager::getInstance().setActiveSceneManager(m_sceneMngr);

	gCursorControl.setVisible(false);
	gCursorControl.setToMidScreen(true);


	m_planeCamera->Visible=true;
	m_freeCamera->Visible=false;

	if(m_windSound)
		m_windSound->play();
}
void GameLevel::onExit(){
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_oldGuiMngr);
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_oldSndMngr);
	eagles::IEaglesManager::getInstance().setActiveSceneManager(m_oldSceneMngr);
	m_oldSceneMngr=0;
	m_oldGuiMngr=0;
	m_oldSndMngr=0;
	gCursorControl.setVisible(true);
	gCursorControl.setToMidScreen(false);
	if(m_windSound)
		m_windSound->stop();
}

float GameLevel::getHeight(float x,float z){
	float y=m_terrain->getHeight(x,z);

	return math::Max<float>(y,m_waterHeight);
}

void GameLevel::update(float dt){
	core::IteratorPair<EntityList> p(m_gameEntities);
	for(;!p.done();p++){
		(*p)->update(dt);
	}

	m_controller->control();
	if(m_player){
		float pY=m_player->getHeight();
		bool setDanger=false;
		m_planeHeightTxt->Caption=mT("Height: ")+core::StringConverter::toString(pY)+mT("(m) ");
		if(pY>150){
			m_planeHeightTxt->getFontAttributes()->fontColor.Set(0,255,0,255);
		}else{
			float t=pY/150.0f;
			static video::SColor red(255,0,0,255);
			static video::SColor green(0,255,0,255);
			m_planeHeightTxt->getFontAttributes()->fontColor=green.Interpolated(red,t);

			m_dangerImage->Color().Set(255,0,0,(1-t)*128);
			m_dangerImage->Visible=true;
			setDanger=true;
		}
		math::vector3d pos=m_player->getPosition();
		if(!m_boundingBox.isPointInSide(pos) && !m_player->isExplode()){
			if(!m_warningBar->Visible){
				m_warningBar->Visible=1;
				m_warningBar->reset();
				m_warningText->Visible=1;
				m_warningText->Caption=mT("Return to World Bounds");
				m_dangerImage->Color().Set(255,0,0,128);
				m_dangerImage->Visible=true;
				m_gamePP->noiseFactor+=5*dt;
				if(m_gamePP->noiseFactor>1)
					m_gamePP->noiseFactor=1;
			}
			setDanger=true;
			m_warningBar->addSteps(dt);
			m_warningText->getFontAttributes()->fontColor.A=abs(128*sinf(gTimer.getActualTime()*0.002))+75;
			if(m_warningBar->isDone()){
				m_player->hasCollide();
			}
		}else {
			if(m_warningBar->Visible){
				m_warningBar->Visible=0;
				m_warningText->Visible=0;
			}

			if(m_gamePP->noiseFactor>0){
				m_gamePP->noiseFactor-=2*dt;
				if(m_gamePP->noiseFactor<0)
					m_gamePP->noiseFactor=0;
			}
		}
		if(!setDanger){
			m_dangerImage->Visible=false;
		}
	}

	m_collisionManager->doCollision(m_gameEntities);
}

void GameLevel::onEvent(GCPtr<Event> event){
	if(event->getType()==ET_Keyboard){
		GCPtr<KeyboardEvent> e=event;
		if(e->press && e->key==KEY_ESCAPE){
			//			onExit();
			//m_state=SenarioGeneric_Done;
			m_menuWnd->Visible=!m_menuWnd->Visible();
			m_pause=!m_pause;
			gCursorControl.setVisible(m_menuWnd->Visible);
			gCursorControl.setToMidScreen(!m_menuWnd->Visible);
		}
	}else if(event->getType()==ET_GuiEvent){
		GCPtr<GUIEvent> e=event;
		if(e->Event==GET_Mouse_UP){
			if(e->Caller->Id==ResumeButtonId){
				m_pause=false;
				m_menuWnd->Visible=false;
				gCursorControl.setVisible(false);
				gCursorControl.setToMidScreen(true);
			}else if(e->Caller->Id==QuitButtonId){
				m_state=SenarioGeneric_Done;
				gCursorControl.setVisible(true);
				gCursorControl.setToMidScreen(false);
			}
		}
	}
}


core::IteratorPair<EntityList> GameLevel::getEntities(){
	return m_gameEntities;
}


void GameLevel::setPlayer(GCPtr<AirPlane> plane){
	m_player=plane;
}
GCPtr<AirPlane> GameLevel::getPlayer(){
	return m_player;
}

void GameLevel::toggleCamera(){
	m_planeCamera->Visible=!m_planeCamera->Visible;
	m_freeCamera->Visible=!m_freeCamera->Visible;

	gCursorControl.setVisible(!gCursorControl.isVisible());
	gCursorControl.setToMidScreen(!gCursorControl.isMidScreen());

	if(m_freeCamera->Visible)
		m_sndMngr->attachToListener(m_freeCamera);
	else
		m_sndMngr->attachToListener(m_planeCamera);
}

}
}