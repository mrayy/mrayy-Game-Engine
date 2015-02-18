
#include "stdafx.h"
#include "CD_SelectTargetMain.h"
#include "GUIManager.h"
#include "GameLevel.h"

#include "GUIEvent.h"
#include "MouseEvent.h"
#include "EventQueue.h"
#include "SystemEvent.h"
#include "KeyboardEvent.h"

#include "Engine.h"
#include "FontResourceManager.h"
#include "AIRenderDevice.h"
#include "SceneManager.h"

#include "CHouse.h"
#include "SceneHelper.h"

#include "AICharacterArch.h"
#include "AIListBoxRenderDevice.h"

#include "ShotingObjectAction.h"

namespace mray
{
namespace GameMod
{

	enum 
	{
		EGUI_EnableAIDebug_BTN=10,
		EGUI_AttachCamera_BTN,
		EGUI_DynamicObs_Chk
	};

	
CD_SelectTargetMain::CD_SelectTargetMain()
:game::IGameLevelAction(mT("CD_SelectTargetMain")),m_level(0),m_camera(0)
{
//	m_pickBuffer=0;
	m_selectedEntity=0;
	m_locationArr=new LocationArrow();
	m_characterManager=0;
	m_cameraAttached=0;
	new CHouse();
	m_phUpdate=0;
}
CD_SelectTargetMain::~CD_SelectTargetMain()
{
	if(m_level)
	{
		GUI::GUIManager*mngr= m_level->GetGUIManager();
		if(mngr)
		{
			mngr->removeChild(m_guiRoot);
		}
	}
	EventQueue::getInstance().removeEventHandler(this);

	m_level->GetSceneManager()->removeChild(m_locationArr->GetShape());
	m_level->GetSceneManager()->removeChild(m_aiVisualizeNode);

	delete CHouse::getInstancePtr();
	delete m_characterManager;
	//delete m_pickBuffer;
}

void CD_SelectTargetMain::AttachToEntity(game::GameEntity*ent)
{
}
void CD_SelectTargetMain::Init(game::GameLevel*level)
{


	m_level=level;
	if(level->GetGUIManager())
	{
		InitGUI();
	}

	m_camera=m_level->GetCamera();
	if(!m_camera)
	{
		m_camera=new scene::SCameraNode(m_level->GetSceneManager());
		m_level->GetRootSceneNode()->addChild(m_camera);
		m_level->SetCamera(m_camera);
	}

	m_locationArr->SetShape(m_level->GetSceneManager()->addAnimatedMesh(mT("LocationArrow.mdx"),0,math::quaternion::Identity,1,mT("")));

	m_aiRenderDevice=new AIListBoxRenderDevice(Engine::getInstance().getDevice(),gFontResourceManager.getDefaultFont(),m_visualizeOutput);
	m_aiVisualizeNode=new scene::AIVisualizeRenderNode(m_level->GetSceneManager(),m_aiRenderDevice);

	m_aiVisualizeNode->AddVisualizableNode(m_level->GetAISystem());
	m_level->GetAISystem()->EnableVisualize(true);
	//m_level->GetRootSceneNode()->addChild( m_aiVisualizeNode);
	m_level->GetSceneManager()->addNode(m_aiVisualizeNode);

//	m_pickBuffer=new video::RTPickBuffer(Engine::getInstance().getDevice(),m_camera->getViewPort().getSize()*0.25,m_camera->getViewPort().getSize());



	m_characterManager=new PhysicsBasedCharacter::CharacterManager(m_level->GetPhysicsManager(),m_level->GetSceneManager(),m_level->GetRootSceneNode());
	CHouse::getInstance().SetCharacterManager(m_characterManager);

	SetEditorMode(m_level->IsEditorMode());


	physics::PhysicMaterialDesc mDesc;
	mDesc.restitution = 0;
	mDesc.staticFriction = 1;
	mDesc.dynamicFriction = 0.8;
	CHouse::getInstance().SetCollisionMtrl(m_level->GetPhysicsManager()->createMaterial(&mDesc));

	CHouse::getInstance().AddCollisionGround(m_level->GetEnvironment()->GetTerrainPhysics());
	CHouse::getInstance().SetCamera(m_camera);

	EventQueue::getInstance().addEventHandler(this,Event::getEventMaskingValue(ET_Mouse)|Event::getEventMaskingValue(ET_Keyboard)|
		Event::getEventMaskingValue(ET_SystemEvent));

}
game::GameLevel* CD_SelectTargetMain::GetLevel()
{
	return m_level;
}


void CD_SelectTargetMain::InitGUI()
{
	GUI::GUIManager*mngr= m_level->GetGUIManager();
	m_guiRoot=(GUI::GUIElement*)mngr->addDummy(0,0);

	mngr->addButton(mT("AI Visual"),math::vector2d(50,100),math::vector2d(200,30),this,m_guiRoot,240,EGUI_EnableAIDebug_BTN);
	mngr->addButton(mT("Attach Camera"),math::vector2d(50,150),math::vector2d(200,30),this,m_guiRoot,240,EGUI_AttachCamera_BTN);

	m_visualizeOutput= mngr->addList(math::vector2d(50,200),math::vector2d(250,300),0,m_guiRoot);

	//mngr->addStaticText(mT("Energy: "),math::vector2d(270,100),math::vector2d(100,30),0,m_guiRoot);
	m_characterEnergy= mngr->addStaticText(mT(""),math::vector2d(370,100),math::vector2d(100,30),0,m_guiRoot);
	m_characterEnergy->Visible=false;

	mngr->addStaticText(mT("Speed: "),math::vector2d(270,100),math::vector2d(100,30),0,m_guiRoot)->Background=false;
	m_characterSpeed= mngr->addStaticText(mT(""),math::vector2d(370,100),math::vector2d(100,30),0,m_guiRoot);
	m_characterSpeed->Background=false;

	mngr->addStaticText(mT("Shot: "),math::vector2d(270,130),math::vector2d(100,30),0,m_guiRoot)->Background=false;
	m_shotPowerSlider= mngr->addSlider(1,100,math::vector2d(370,130),math::vector2d(300,30),0,m_guiRoot);
// 	m_BallObjectRdo= mngr->addRadioButton(mT("Ball"),math::vector2d(320,190),math::vector2d(150,20),0,m_guiRoot);
// 	m_CrateObjectRdo= mngr->addRadioButton(mT("Crate"),math::vector2d(320,215),math::vector2d(150,20),0,m_guiRoot);
	m_dynamicObsChk= mngr->addCheckBox(mT("DynamicObstacle"),math::vector2d(320,240),math::vector2d(150,20),this,m_guiRoot,240,EGUI_DynamicObs_Chk);
	//m_dynamicObsChk->PushButton=true;

	//m_BallObjectRdo->Checked=true;

}

void CD_SelectTargetMain::SetEditorMode(bool e)
{
	if(m_guiRoot)
		m_guiRoot->Visible=!e;
	m_locationArr->Hide();
	SetSelectedEntity(0);
	m_cameraAttached=0;
}

void CD_SelectTargetMain::Update(float dt)
{
	m_locationArr->Update(dt);

	if(m_selectedEntity)
	{
		AICharacterArch* charArch=dynamic_cast<AICharacterArch*>(m_selectedEntity->GetAIActor()->GetController());		
		if(!charArch)return;
		m_characterEnergy->Caption=core::StringConverter::toString(charArch->GetCharacterObject()->GetEnergy());
		m_characterSpeed->Caption=core::StringConverter::toString(charArch->GetLastLinearVelocity().Length(),4);
		if(m_cameraAttached)
		{
			m_camera->position=m_selectedEntity->GetShape()->getPosition()+math::vector3d(0,m_selectedEntity->GetShape()->getBoundingBox().getHeight(),0);
			m_camera->setOrintation(m_selectedEntity->GetAIActor()->GetPhysicalProp()->GetGlobalOrintation());
		}
	}


	m_phUpdate+=dt;
	if(m_phUpdate>=1.0f/60.0f){
		m_characterManager->update(1.0f/60.0f);
		//m_phUpdate-=1.0f/25.0f;
		m_phUpdate -= 1.0f/60.0f;
	}
}


void CD_SelectTargetMain::SetSelectedEntity(game::GameEntity*e)
{
	if(m_selectedEntity)
	{
		AI::AIActor*actor=m_selectedEntity->GetAIActor();
		if(actor)
			actor->EnableVisualize(false);	 
	}
	if(!e){
		m_cameraAttached=0;
		m_dynamicObsChk->Checked=false;
	}
	m_selectedEntity=e;
	if(m_selectedEntity)
	{
		AICharacterArch*arch=(AICharacterArch*) m_selectedEntity->GetAIActor()->GetController();
		if(arch)
			m_dynamicObsChk->Checked=arch->IsDynamicAvoidanceEnabled();
		AI::AIActor*actor=m_selectedEntity->GetAIActor();
		if(actor)
			actor->EnableVisualize(true);	 
	}
}

void CD_SelectTargetMain::OnRMouseDown(int x,int y)
{
	//SetSelectedEntity(0);
}
void CD_SelectTargetMain::OnLMouseDown(int x,int y)
{
/*
	m_pickBuffer->Clear();
	m_pickBuffer->SetViewMatrix(m_camera->getViewMatrix());
	m_pickBuffer->SetProjectionMatrix(m_camera->getProjectionMatrix());

	game::GameEntityManager* entMngr=m_level->GetEntityManager();
	const game::EntityList& entities=entMngr->GetEntities();
	game::EntityList::const_iterator it=entities.begin();
	for (;it!=entities.end();++it)
	{
		game::GameEntity* ent=*it;
		AI::AIActor*actor= ent->GetAIActor();
		scene::ISceneNode* node=(scene::ISceneNode*)ent->GetShape();
		if(!actor || !node)continue;
		AI::IAIArchitect*arch= actor->GetController();
		if(!arch)continue;
		AICharacterArch* charArch=dynamic_cast<AICharacterArch*>(arch);
		if(!charArch)continue;
		scene::SMesh*mesh=node->getMesh();
		if(!mesh)continue;
		if(m_camera->isCulling(node))continue;
		m_pickBuffer->PushPickID(ent->GetId());
		for(int i=0;i<mesh->getBuffersCount();++i){
			m_pickBuffer->AddRenderObject(mesh->getBufferData(i),node);
		}
	}
	m_pickBuffer->Render();

	uint id= m_pickBuffer->GetID(x,y);
*/
	scene::NodeList nodes;
	game::GameEntityManager* entMngr=m_level->GetEntityManager();
	const game::EntityList& entities=entMngr->GetEntities();
	game::EntityList::const_iterator it=entities.begin();
	for (;it!=entities.end();++it)
	{
		game::GameEntity* ent=*it;
		AI::AIActor*actor= ent->GetAIActor();
		scene::ISceneNode* node=(scene::ISceneNode*)ent->GetShape();
		if(!actor || !node)continue;
		AI::IAIArchitect*arch= actor->GetController();
		if(!arch)continue;
		AICharacterArch* charArch=dynamic_cast<AICharacterArch*>(arch);
		if(!charArch)continue;
		scene::SMesh*mesh=node->getMesh();
		if(!mesh)continue;
		if(m_camera->isCulling(node))continue;
		nodes.push_back(node);
	}

	math::line3d ray= scene::SceneHelper::getInstance().getRayFromScreenCoord(math::vector2d(x,y),m_camera);
	GCPtr<scene::ISceneNode> node=scene::SceneHelper::getInstance().getNodeFromRay(nodes,ray,0,0);
	uint id=0;
	if(node)
	{
		id=entMngr->GetSceneNodeEntID(node->getID());
	}
	if(id!=0){
		SetSelectedEntity(entMngr->GetGameEntity(id));
		return;
	}

	if(!m_selectedEntity)
		return;

	math::vector3d dir=scene::SceneHelper::getInstance().getVectorFromScreen(m_camera->getViewPort(),math::vector2d(x,y),1000,m_camera);

	math::vector3d contact;
	
	if(CHouse::getInstance().CastRay(m_camera->getAbsolutePosition(),dir,contact))
	{
		m_locationArr->SetLocation(contact);
		m_locationArr->Show();

		AICharacterArch* charArch=dynamic_cast<AICharacterArch*>(m_selectedEntity->GetAIActor()->GetController());
		charArch->GetCharacterObject()->GotoPosition(contact);
	}
}



void CD_SelectTargetMain::CreateShotingBall()
{
	EShotingObjectType type=ESOT_Ball;
	core::string modelName=mT("ball.mdx");
	type=ESOT_Ball;
	
	float size=0.2;
	GCPtr<scene::ISceneNode> node= m_level->GetSceneManager()->addAnimatedMesh(modelName.c_str(),m_camera->getPosition(),math::quaternion::Identity,size,mT(""),
		m_level->GetRootSceneNode(),true);
	GCPtr<game::GameEntity> ent= m_level->GetEntityManager()->CreateGameEntity(mT(""));
	ent->SetShape(node);

	ent->SetGameAction(new ShotingObjectAction(size,m_camera->getOrintation()*math::vector3d::ZAxis*(float)m_shotPowerSlider->Value,type));
}


void CD_SelectTargetMain::onEvent(GCPtr<Event> event)
{
	if(event->isRecived())
		return;
	if(GetLevel()->IsEditorMode())
		return;

	switch(event->getType()){
	case ET_SystemEvent:
		{
			GCPtr<SystemEvent> e=event;
			if(e->m_eventType==ESET_Resized ){
/*				m_pickBuffer->SetViewportSize(m_camera->getViewPort().getSize());
				m_pickBuffer->SetRenderTargetSize(m_camera->getViewPort().getSize()/2);*/
			}
			break;
		}
	case ET_GuiEvent:
		{
			GCPtr<GUIEvent> e=event;
			if(e->Event==GET_Mouse_UP){
				if(e->Caller->Id==EGUI_EnableAIDebug_BTN)
				{
					m_aiVisualizeNode->setVisible(!m_aiVisualizeNode->isVisible());
					event->setRecived();
					m_visualizeOutput->Visible=(m_aiVisualizeNode->isVisible());
				}else if(e->Caller->Id==EGUI_AttachCamera_BTN)
				{
					if(m_selectedEntity)
						m_cameraAttached=!m_cameraAttached;
				}else if(e->Caller->Id==EGUI_DynamicObs_Chk)
				{
					if(m_selectedEntity)
					{
						AICharacterArch*arch=(AICharacterArch*) m_selectedEntity->GetAIActor()->GetController();
						if(arch){
							arch->EnableDynamicAvoidance(m_dynamicObsChk->Checked);
						}
					}
				}
			}else if(e->Event==GET_Mouse_MOVE){
			}else if(e->Event==GET_SELECT_ITEM)
			{
			}
		}break;
	case ET_Mouse:
		{
			GCPtr<MouseEvent> e=event;
			if(e->Event==MET_LEFTDOWN)
			{
				OnLMouseDown(e->x,e->y);
			}else if(e->Event==MET_RIGHTDOWN)
			{
				OnRMouseDown(e->x,e->y);
			}
		}break;
	case ET_Keyboard:
		{
			GCPtr<KeyboardEvent> e=event;
			if(e->press && e->key==KEY_SPACE)
			{
				CreateShotingBall();
			}
		}break;
	}
}

}
}


