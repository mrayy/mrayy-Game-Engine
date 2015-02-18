#include "stdafx.h"

#include "GameEnvironment.h"

#include "Sun.h"
#include "SkyBoxManager.h"
#include "ILogManager.h"
#include "XMLTerrainReader.h"

#include "Engine.h"
#include "GameLevel.h"


#include <SSkyBoxNode.h>
#include <SPatchTerrain.h>
#include <ParsedShaderPP.h>
#include <IPhysicalNode.h>
#include <ISceneManager.h>
#include <IFileSystem.h>

#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray
{
namespace game
{

GameEnvironment::GameEnvironment(GameLevel* level):
	m_timeSpeed(0),m_waterLevel(0),m_sun(0),m_time(12,0),m_level(0),m_postProcessingEffect(0),m_skyBoxNode(0),m_terrainNode(0)
{

	m_skyBox=new scene::SSkyBoxNode();
	m_sun=new Sun(level);
	m_skyBoxNode=level->GetSceneManager()->createSceneNode(mT("GameSkyBox"));
	m_skyBoxNode->AttachNode(m_skyBox);

	m_terrainNode=level->GetSceneManager()->createSceneNode(mT("GameTerrain"));

	video::IVideoDevice*device=Engine::getInstance().getDevice();

	m_postProcessingEffect=new video::ParsedShaderPP(device);
}
GameEnvironment::~GameEnvironment()
{
}

Sun* GameEnvironment::GetSun()
{
	return m_sun;
}

void GameEnvironment::SetGameLevel(GameLevel*level)
{
	m_level=level;
}

GameLevel* GameEnvironment::GetGameLevel()
{
	return m_level;
}

void GameEnvironment::SetSkybox(const core::string&name)
{
	m_skyName=name;
	GCPtr<video::ITexture> tex=scene::SkyBoxManager::getInstance().getTexture(m_skyName);
	if(!tex)
	{
		gLogManager.log(mT("GameEnvironment::SetSkybox - Skybox not found : ")+name,ELL_WARNING);
	}
	m_skyBox->getMaterial(0)->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);
}
scene::SSkyBoxNode* GameEnvironment::GetSkybox()
{
	return m_skyBox;
}


void GameEnvironment::OnTerrainCreated()
{
	if(m_terrainPHNode)
	{
		m_level->GetPhysicsManager()->removeNode(m_terrainPHNode);
		m_terrainPHNode=0;
	}
	if(m_level && m_level->GetPhysicsManager() && m_terrain->getHeightMap()){
		physics::PhysicalNodeDesc nodeDesc;
		physics::TerrainShapeDesc desc;
		desc.cols=m_terrain->getHeightMap()->getWidth();
		desc.rows=m_terrain->getHeightMap()->getHeight();
		desc.data=m_terrain->getHeightMap()->getHeightData();
		desc.scale=m_terrain->GetScale();
		desc.skip=3;
		nodeDesc.shapes.push_back(&desc);

		m_terrainPHNode= m_level->GetPhysicsManager()->createNode(&nodeDesc);
	}
}

void GameEnvironment::GenerateTerrain(GCPtr<loaders::IHeightMapLoader> hm,float lod,float lodDist,const math::vector3d&scale)
{
	if(!m_terrain)
	{
		m_terrain=new scene::SPatchTerrain(0,1);
		m_terrainNode->AttachNode(m_terrain);
	}
	m_terrain->setScale(scale);

	m_terrain->loadTerrain(hm,lod,lodDist);

	OnTerrainCreated();

}

scene::SPatchTerrain* GameEnvironment::GetTerrain()
{
	return m_terrain;
}
scene::ISceneNode* GameEnvironment::GetTerrainNode()
{
	return m_terrainNode;
}

physics::IPhysicalNode* GameEnvironment::GetTerrainPhysics()
{
	return m_terrainPHNode;
}

void GameEnvironment::RemoveTerrain()
{
	m_terrain=0;
}

void GameEnvironment::SetWaterLevel(float l)
{
	m_waterLevel=l;
	if(m_waterSurface)
	{
		math::vector3d pos=m_waterSurface->getPosition();
		pos.y=m_waterLevel;
		m_waterSurface->setPosition(m_waterLevel);
	}
}
float GameEnvironment::GetWaterLevel()
{
	return m_waterLevel;
}

void GameEnvironment::SetWaterSurface(scene::ISceneNode* waterSurface)
{
	m_waterSurface=waterSurface;
	SetWaterLevel(m_waterLevel);
}
scene::ISceneNode* GameEnvironment::GetWaterSurface()
{
	return m_waterSurface;
}

void GameEnvironment::SetTime(const TimeTicket& t)
{
	m_time=t;
}
const TimeTicket& GameEnvironment::GetTime()
{
	return m_time;
}
void GameEnvironment::SetTimeSpeed(double s)
{
	m_timeSpeed=s;
}
double GameEnvironment::GetTimeSpeed()
{
	return m_timeSpeed;
}


void GameEnvironment::Update(float dt)
{
	m_time+=m_timeSpeed;
	m_sun->Update(dt);
}

void GameEnvironment::SetPostProcessingName(const core::string&name)
{
	m_postProcessingEffectName=name;

	GCPtr<OS::IStream> stream=gFileSystem.openFile(name,OS::TXT_READ);
	m_postProcessingEffect->LoadXML(stream);

}
const core::string& GameEnvironment::GetPostProcessingName()
{
	return m_postProcessingEffectName;
}
video::ParsedShaderPP* GameEnvironment::GetPostProcessingEffect()
{
	return m_postProcessingEffect;
}

void GameEnvironment::loadXMLSettings(xml::XMLElement* elem)
{
	if(m_terrainPHNode)
	{
		m_level->GetPhysicsManager()->removeNode(m_terrainPHNode);
		m_terrainPHNode=0;
	}
	xml::XMLElement*e;
	e=elem->getSubElement(mT("Sun"));
	if(e)
		m_sun->loadXMLSettings(e);


	e=elem->getSubElement(mT("SPatchTerrain"));
	if(e){
		scene::XMLTerrainReader loader;
		m_terrain=loader.loadXML(e);
		OnTerrainCreated();
	}else
	{
		m_terrain=0;
	}

	xml::XMLAttribute*attr;

	attr=elem->getAttribute(mT("SkyName"));
	if(attr)
	{
		SetSkybox(attr->value);
	}
	attr=elem->getAttribute(mT("WaterLevel"));
	if(attr)
	{
		SetWaterLevel(core::StringConverter::toFloat(attr->value));
	}
	attr=elem->getAttribute(mT("PostProcessing"));
	if(attr)
	{
		SetPostProcessingName(attr->value);
	}

}
xml::XMLElement*  GameEnvironment::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* root=new xml::XMLElement(mT("GameEnvironment"));
	elem->addSubElement(root);

	m_sun->exportXMLSettings(root);
	if(m_terrain)
		m_terrain->exportXMLSettings(root);

	root->addAttribute(mT("SkyName"),m_skyName);
	root->addAttribute(mT("WaterLevel"),core::StringConverter::toString(m_waterSurface));
	root->addAttribute(mT("PostProcessing"),m_postProcessingEffectName);

	return root;
}


}
}