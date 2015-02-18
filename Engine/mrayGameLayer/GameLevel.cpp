#include "stdafx.h"
#include "stdafx.h"

#include "GameLevel.h"
#include "ILogManager.h"

#include "ISceneManager.h"
#include "Sun.h"
#include "IFileSystem.h"

#include "ShaderResourceManager.h"
#include "MaterialResourceManager.h"

#include "XMLSceneLoader.h"
#include "XMLTree.h"
#include "XMLWriter.h"
#include "StreamWriter.h"
#include "IResLoadListener.h"


namespace mray
{
namespace game
{

GameLevel::GameLevel():m_isEditor(false),m_state(EGLS_Normal)
{
	m_gmGlobals=new game::GameGlobals();
	m_envManager=new game::GameEnvironment(this);

	m_aiSystem=new AI::AISystem();

	m_entManager=new game::GameEntityManager();
	m_entManager->SetGameLevel(this);
	m_camera=0;

	m_phUpdate=0;
}
GameLevel::~GameLevel()
{
	
	m_entManager=0;
	m_mainAction=0;
	m_envManager=0;
	m_aiSystem=0;
	m_root=0;
	m_phManager=0;
	m_gmGlobals=0;
	m_soundManager=0;
	m_guiManager=0;
}


void GameLevel::SetGUIManager(GCPtr<GUI::IGUIManager> guiManager)
{
	m_guiManager=guiManager;
}
GUI::IGUIManager* GameLevel::GetGUIManager()
{
	return m_guiManager;
}


void GameLevel::SetSoundManager(GCPtr<sound::ISoundManager> sndManager)
{
	m_soundManager=sndManager;
}
sound::ISoundManager* GameLevel::GetSoundManager()
{
	return m_soundManager;
}
void GameLevel::SetCamera(scene::SCameraNode* cam)
{
	m_camera=cam;
}

scene::SCameraNode* GameLevel::GetCamera()
{
	return m_camera;
}

void GameLevel::SetState(EGameLevelState state)
{
	m_state=state;
}
EGameLevelState GameLevel::GetState()
{
	return m_state;
}
const GameLevelInfo& GameLevel::GetCompleteInfo()
{
	return m_info;
}
const core::string& GameLevel::GetLevelName()
{
	return m_info.name;
}
const core::string& GameLevel::GetLevelDescription()
{
	return m_info.description;
}
const core::string& GameLevel::GetLevelThumbNailName()
{
	return m_info.thumbnail;
}

void GameLevel::SetLevelName(const core::string& name)
{
	m_info.name=name;
}
void GameLevel::SetLevelDescription(const core::string& desc)
{
	m_info.description=desc;
}
void GameLevel::SetLevelThumbNailName(const core::string& name)
{
	m_info.thumbnail=name;
}

void GameLevel::InitManagers()
{
	gLogManager.log(mT("Initiating Game Framework"),ELL_INFO);

	m_entManager->SetSceneManager(m_smngr);

	m_root=m_smngr->createSceneNode(mT("Root"));
}

void GameLevel::SetPhysicsManager(GCPtr<physics::IPhysicManager> phManager)
{
	m_phManager=phManager;
}
GCPtr<physics::IPhysicManager> GameLevel::GetPhysicsManager()
{
	return m_phManager;
}


GameEntityManager*GameLevel:: GetEntityManager()
{
	return m_entManager;
}
GameEnvironment* GameLevel::GetEnvironment()
{
	return m_envManager;
}
GameGlobals* GameLevel::GetGameGlobals()
{
	return m_gmGlobals;
}
AI::AISystem* GameLevel::GetAISystem()
{
	return m_aiSystem;
}

void GameLevel::SetSceneManager(scene::ISceneManager* smngr)
{
	m_smngr=smngr;
}
scene::ISceneManager* GameLevel::GetSceneManager()
{
	return m_smngr;
}
void GameLevel::SetMainAction(GCPtr<game::IGameLevelAction> a)
{
	m_mainAction=a;
	//m_mainAction->AttachToEntity(0);
	if(m_mainAction)
		m_mainAction->Init(this);
}
game::IGameLevelAction* GameLevel::GetMainAction()
{
	return m_mainAction;
}

void GameLevel::ClearLevel()
{
	m_entManager->Clear();
	m_mainAction=0;
	
	m_smngr->removeSceneNode( m_envManager->GetTerrainNode());

	//m_smngr->removeChild( m_envManager->GetSkybox());
	//m_smngr->removeChild( m_envManager->GetSun()->GetLight());
	m_envManager->RemoveTerrain();
	m_root->removeAll();

	m_aiSystem->Clear();
	m_gmGlobals->ClearPlugins();
	m_gmGlobals->ClearSearchPaths();
	if(m_soundManager)
	{
		m_soundManager->resumeAllSounds();
	}
	if(m_phManager){
		m_phManager->clearJoints();
		m_phManager->clearNodes();
		m_phManager->clearControllers();
		m_phManager->clearMaterials();
	}
	m_state=EGLS_Normal;
}

bool GameLevel::LoadLevel(const core::string& path,IResLoadListener*listener)
{
	gLogManager.log(mT("Loading level:")+path,ELL_INFO);
	ClearLevel();

	//level loading goes here
	OS::IStream* stream=gFileSystem.openFile(path,OS::TXT_READ);
	if(!stream)
	{
		gLogManager.log(mT("GameLevel::LoadLevel - Failed to load GameLevel file : ")+ path,ELL_WARNING);
		return false;
	}

	xml::XMLTree tree;
	if(!tree.load(stream)){
		gLogManager.log(mT("GameLevel::LoadLevel - Failed to load GameLevel file : ")+ path,ELL_WARNING);
		return false;
	}
	xml::XMLElement*elem=tree.getSubElement(mT("GameLevel"));
	if(!elem)
	{
		gLogManager.log(mT("GameLevel::LoadLevel - Couldn't find the main tag \"GameLevel\" in file : ")+ path,ELL_WARNING);
		return false;
	}

	m_info.path=path;

	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Name"));
	if(attr)
		SetLevelName(attr->value);
	else
		SetLevelName(mT(""));
	attr=elem->getAttribute(mT("Description"));
	if(attr)
		SetLevelDescription(attr->value);
	else
		SetLevelDescription(mT(""));
	attr=elem->getAttribute(mT("Thumbnail"));
	if(attr)
		SetLevelThumbNailName(attr->value);
	else
		SetLevelThumbNailName(mT(""));

	if(listener)
		listener->beginLoadResources(mT("Game Level"),8);

	xml::XMLElement*e=elem->getSubElement(mT("GameGlobals"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("GameGlobals"));
		GetGameGlobals()->loadXMLSettings(e);
		if(listener)
			listener->onResourceLoadDone(mT("GameGlobals"));
	}
	
	IGameLevelAction* m_gameAction=0;
	/*attr=elem->getAttribute(mT("MainAction"));
	if(attr)
	{
		IGameAction*a=GameActionFactory::getInstance().CreateObject(attr->value);
		m_gameAction=dynamic_cast<IGameLevelAction*>(a);
	}*/


	e=elem->getSubElement(mT("Shaders"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("Shaders"));
		gShaderResourceManager.parseShaderXML(e);
		if(listener)
			listener->onResourceLoadDone(mT("Shaders"));
	}
	e=elem->getSubElement(mT("Materials"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("Materials"));
		gMaterialResourceManager.parseMaterialXML(e);
		if(listener)
			listener->onResourceLoadDone(mT("Materials"));
	}
	e=elem->getSubElement(mT("Scene"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("Scene"));
		scene::XMLSceneLoader loader(GetSceneManager());
		loader.loadXML(e,GetRootSceneNode(),listener);
		if(listener)
			listener->onResourceLoadDone(mT("Scene"));
		/*
		m_lastID=1;
		core::IteratorPair<scene::NodeList> it=m_root->getChildren();
		for(;!it.done();++it){
		++m_lastID;
		}*/
	}

	e=elem->getSubElement(mT("AISystem"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("AISystem"));
		GetAISystem()->loadXMLSettings(e);
		if(listener)
			listener->onResourceLoadDone(mT("AISystem"));
	}

	e=elem->getSubElement(mT("GameEnvironment"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("GameEnvironment"));
		GetEnvironment()->loadXMLSettings(e);
		if(listener)
			listener->onResourceLoadDone(mT("GameEnvironment"));
	}
	//
	//m_smngr->addSceneNode( m_envManager->GetSkybox());
	//m_smngr->addSceneNode( m_envManager->GetSun()->GetLight());
	if(m_envManager->GetTerrain())
		m_smngr->addSceneNode( m_envManager->GetTerrainNode());


	if(m_gameAction)
		SetMainAction(m_gameAction);


	e=elem->getSubElement(mT("GameEntityManager"));
	if(e)
	{
		if(listener)
			listener->onResourceLoadStart(mT("Entities"));
		GetEntityManager()->loadXMLSettings(e);
		if(listener)
			listener->onResourceLoadDone(mT("Entities"));
	}




	if(listener)
		listener->loadDone();


	stream->close();

	return true;
}
bool GameLevel::ExportLevel(const core::string& path)
{
	OS::IStream* stream=gFileSystem.openFile(path,OS::TXT_WRITE);
	if(!stream)
	{
		gLogManager.log(mT("GameLevel::LoadLevel - Failed to open file to write : ")+ path,ELL_WARNING);
		return false;
	}

	xml::XMLWriter writer;
	xml::XMLElement* elem=new xml::XMLElement(mT("GameLevel"));

	elem->addAttribute(mT("Name"),GetLevelName());
	elem->addAttribute(mT("Description"),GetLevelDescription());
	elem->addAttribute(mT("Thumbnail"),GetLevelThumbNailName());

	if(m_mainAction)
	{
		elem->addAttribute(mT("MainAction"),m_mainAction->getObjectType());
		m_mainAction->exportXMLSettings(elem);
	}

	GetGameGlobals()->exportXMLSettings(elem);
	gShaderResourceManager.exportShaderXML(elem);
	gMaterialResourceManager.exportMaterialXML(elem);

	{

		xml::XMLElement*e=new xml::XMLElement(mT("Scene"));
		elem->addSubElement(e);
		
		scene::MovableNodeList::const_iterator p=GetRootSceneNode()->getChildren().begin();
		scene::MovableNodeList::const_iterator end=GetRootSceneNode()->getChildren().end();

		for(;p!=end;++p)
		{
			scene::IMovable* node=*p;//dynamic_cast<scene::ISceneNode*>(*it);
			if(!node)
				continue;/*
			if(node->getNodeType() & scene::EST_EDITOR)
				continue;*/
			node->exportXMLSettings(e);
		}
	}

	GetEntityManager()->exportXMLSettings(elem);
	GetEnvironment()->exportXMLSettings(elem);
	GetAISystem()->exportXMLSettings(elem);

	writer.addElement(elem);

	OS::StreamWriter ww(stream);
	ww.writeString(writer.flush());
	delete elem;
	stream->close();
	return true;
}


GCPtr<scene::ISceneNode> GameLevel::GetRootSceneNode()
{
	return m_root;
}

void GameLevel::SetEditorMode(bool e)
{
	m_isEditor=e;

	if(m_mainAction)
		m_mainAction->SetEditorMode(m_isEditor);
	/*
	if(m_smngr)
		m_smngr->SetEditorMode(e);*/
}
bool GameLevel::IsEditorMode()
{
	return m_isEditor;
}


void GameLevel::GenerateTerrain(GCPtr<loaders::IHeightMapLoader> hm,const math::vector3d &scale,float lod,float lodDist,GCPtr<video::ITexture> base,GCPtr<video::ITexture> baseNormal,
									GCPtr<video::ITexture> detail,GCPtr<video::ITexture> detailNormal)
{
//	m_smngr->removeSceneNode(m_envManager->GetTerrain());
	m_envManager->GenerateTerrain(hm,lod,lodDist,scale);

/*	scene::SPatchTerrain* terrain=m_envManager->GetTerrain();
	if(terrain){
		terrain->setBaseTexture(base);
		terrain->setDetailNormal(detailNormal);
		terrain->setDetailTexture(detail);
		m_smngr->addSceneNode(terrain);

	}*/
}


void GameLevel::Update(float dt)
{
	if(m_state!=EGLS_Normal)
		return;
	if(m_phManager){
		m_phUpdate+=dt;
		if(m_phUpdate>=1.0f/60.0f){
			m_phManager->update(1.0f/60.0f);
			//m_phUpdate-=1.0f/25.0f;
			m_phUpdate -= 1.0f/60.0f;
		}
		
	}
	m_envManager->Update(dt);
	m_entManager->Update(dt);
	m_aiSystem->Update(dt);
	if(m_mainAction)
		m_mainAction->Update(dt);
}

}
}

