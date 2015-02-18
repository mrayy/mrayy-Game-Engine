
#include "stdafx.h"
#include "ModelComponent.h"
#include "GameEntity.h"
#include "SceneComponent.h"
#include "ISceneNode.h"
#include "MeshFileCreatorManager.h"
#include "MeshRenderableNode.h"

#include "TangentCalculater.h"

namespace mray
{
namespace game
{

IMPLEMENT_PROPERTY_TYPE_HEADER(ModelPath,ModelComponent,core::string,mT("ModelPath"),EPBT_Basic,mT("Model's path"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(ModelPath,ModelComponent,core::string,SetModelPath,GetModelPath,,,false);

ModelComponent::ModelComponent(GameEntityManager*mngr)
{
	m_model=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeModelPath::instance);
	}
}
ModelComponent::ModelComponent()
{
	m_model=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeModelPath::instance);
	}
	InitComponent();
}
ModelComponent::~ModelComponent()
{
}
bool ModelComponent::InitComponent()
{
	if(m_inited)
		return true;

	m_model=scene::MeshFileCreatorManager::getInstance().LoadFromFile(m_path,false);
	if(m_model)
		m_model->setShadowCaster(true);
	return IGameComponent::InitComponent();
}

bool ModelComponent::SetModelPath(const core::string& path)
{
	m_path=path;
	if(m_inited)
		return InitComponent();

	return true;
}
const core::string& ModelComponent::GetModelPath()const
{
	return m_path;
}

const GCPtr<scene::MeshRenderableNode>& ModelComponent::GetModel()
{
	return m_model;
}


bool ModelComponent::SetEnabled(bool enabled)
{
	IGameComponent::SetEnabled(enabled);
	return true;
}

IObject* ModelComponent::GetAttachedObject()
{
	return m_model;
}


}
}

