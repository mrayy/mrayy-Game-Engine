

#include "stdafx.h"
#include "SkyBoxComponent.h"
#include "SSkyBoxNode.h"
#include "ISceneNode.h"
#include "TextureResourceManager.h"
#include "ISceneManager.h"
#include "SSkyBoxNode.h"
#include "GameEntityManager.h"




namespace mray
{
namespace scene
{
	class SSkyBoxNode;
	class ISceneNode;
}
namespace game
{
	IMPLEMENT_PROPERTY_TYPE_HEADER(SkyBoxPath,SkyBoxComponent,core::string,mT("SkyBoxPath"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(SkyBoxPath,SkyBoxComponent,core::string,SetSkyBoxPath,GetSkyBoxPath,,,false);
		
SkyBoxComponent::SkyBoxComponent(GameEntityManager*m)
{
	m_skyBoxNode=0;
	m_node=0;
	m_sceneMngr=m->GetSceneManager();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeSkyBoxPath::instance);
	}
}
SkyBoxComponent::~SkyBoxComponent()
{
}

bool SkyBoxComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;

	if(m_node)//already inited
		return true;
	m_node= m_sceneMngr->createSceneNode(GetName()+"_skyBox");
	m_skyBoxNode=new scene::SSkyBoxNode();
	m_skyBoxNode->SetTexture(m_skyTex);
	m_node->AttachNode(m_skyBoxNode);
	return true;
}

bool SkyBoxComponent::SetSkyBoxPath(const core::string& p)
{
	m_skyboxPath=p;
	std::vector<core::string> files;
	core::string ext="png";

	files.push_back(m_skyboxPath+"left."+ext);
	files.push_back(m_skyboxPath+"right."+ext);
	files.push_back(m_skyboxPath+"front."+ext);
	files.push_back(m_skyboxPath+"back."+ext);
	files.push_back(m_skyboxPath+"up."+ext);
	files.push_back(m_skyboxPath+"bottom."+ext);

	m_skyTex=gTextureResourceManager.loadTextureCube(GetName()+"_SkyBox",&files[0]);
	if(m_skyBoxNode)
		m_skyBoxNode->SetTexture(m_skyTex);
	return true;
}

const core::string& SkyBoxComponent::GetSkyBoxPath()
{
	return m_skyboxPath;
}

 xml::XMLElement*  SkyBoxComponent::loadXMLSettings(xml::XMLElement* elem)
{
	return IGameComponent::loadXMLSettings(elem);
}

}
}

