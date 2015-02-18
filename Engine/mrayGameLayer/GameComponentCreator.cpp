
#include "stdafx.h"
#include "GameComponentCreator.h"
#include "IGameComponentFactory.h"
#include "IGameComponent.h"
#include "ILogManager.h"

#include "AIComponent.h"
#include "SceneComponent.h"
#include "LightComponent.h"
#include "ModelCollisionComponent.h"
#include "ModelComponent.h"
#include "SphereModelComponent.h"
#include "PhysicsComponent.h"
#include "ScriptComponent.h"
#include "SoundComponent.h"
#include "VehicleGameComponent.h"
#include "VehicleWheelComponent.h"
#include "PrimitiveCollisionComponent.h"
#include "KinematicJointDOF6Component.h"
#include "PhysicalJointDOF6Component.h"
#include "PhysicalJointFixedComponent.h"
#include "PhysicalMaterialComponent.h"
#include "ClothComponent.h"
#include "HierarchyComponent.h"
#include "BoneComponent.h"
#include "NullPhysicsComponent.h"
#include "SkyBoxComponent.h"

#include "LineRendererComponent.h"

namespace mray
{
namespace game
{

GameComponentCreator::GameComponentCreator()
{
	REGISTER_COMPONENT_FACTORY(AIComponent);
	REGISTER_COMPONENT_FACTORY(SceneComponent);
	REGISTER_COMPONENT_FACTORY(LightComponent);
	REGISTER_COMPONENT_FACTORY(ModelCollisionComponent);
	REGISTER_COMPONENT_FACTORY(ModelComponent);
	REGISTER_COMPONENT_FACTORY(PhysicsComponent);
	REGISTER_COMPONENT_FACTORY(ScriptComponent);
	REGISTER_COMPONENT_FACTORY(SoundComponent);
	REGISTER_COMPONENT_FACTORY(VehicleGameComponent);
	REGISTER_COMPONENT_FACTORY(VehicleWheelComponent);
	REGISTER_COMPONENT_FACTORY(PhysicalJointDOF6Component);
	REGISTER_COMPONENT_FACTORY(PhysicalJointFixedComponent);
	REGISTER_COMPONENT_FACTORY(KinematicJointDOF6Component);
	REGISTER_COMPONENT_FACTORY(PrimitiveCollisionComponent);
	REGISTER_COMPONENT_FACTORY(SphereModelComponent);
	REGISTER_COMPONENT_FACTORY(PhysicalMaterialComponent);
	REGISTER_COMPONENT_FACTORY(ClothComponent);
	REGISTER_COMPONENT_FACTORY(BoneComponent);
	REGISTER_COMPONENT_FACTORY(HierarchyComponent);
	REGISTER_COMPONENT_FACTORY(NullPhysicsComponent);
	REGISTER_COMPONENT_FACTORY(LineRendererComponent);
	REGISTER_COMPONENT_FACTORY(SkyBoxComponent);

}
GameComponentCreator::~GameComponentCreator()
{
	ClearFactories();
}

GameComponentCreator::FactoryInfo* GameComponentCreator::_GetFactoryInfo(const core::string&name)
{
	AliasMap::iterator it= m_aliasMap.find(name);
	if(it==m_aliasMap.end())
		return 0;
	FactoryMap::iterator it2= m_factories.find(it->second);
	if(it2==m_factories.end())
		return 0;
	return &it2->second;
}

void GameComponentCreator::RegisterFactory(IGameComponentFactory* factory)
{
	int guid=mray::GUID(factory->GetType()).ID();
	m_factories[guid]=factory;
	m_aliasMap[factory->GetType()]=guid;
}
void GameComponentCreator::UnregisterFactory(const core::string&name)
{
	AliasMap::iterator aliasIt= m_aliasMap.find(name);
	if(aliasIt==m_aliasMap.end())
		return;
	FactoryMap::iterator it= m_factories.find(aliasIt->second);
	if(it==m_factories.end())return ;
	m_factories.erase(it);
}
IGameComponentFactory* GameComponentCreator::GetFactory(const core::string&name,bool shouldEnabled)
{
	FactoryInfo* ifo=_GetFactoryInfo(name);
	if(!ifo || shouldEnabled && !ifo->enabled)return 0;
	return ifo->factory;
}

void GameComponentCreator::SetFactoryEnabled(const core::string& name, bool e)
{
	FactoryInfo* ifo=_GetFactoryInfo(name);
	if(!ifo)
		return;
	ifo->enabled=e;
}
bool GameComponentCreator::IsFactoryEnabled(const core::string& name)
{
	FactoryInfo* ifo=_GetFactoryInfo(name);
	if(!ifo)
		return false;
	return ifo->enabled;
}
bool GameComponentCreator::AddAlias(const core::string& factory,const core::string& alias)
{
	AliasMap::iterator it= m_aliasMap.find(factory);
	if(it==m_aliasMap.end())
		return 0;

	AliasMap::iterator aliasIt= m_aliasMap.find(alias);
	if(aliasIt!=m_aliasMap.end())
		return 0;
	m_aliasMap[alias]=it->second;
	return true;
}
void GameComponentCreator::RemoveAlias(const core::string& alias)
{
	AliasMap::iterator aliasIt= m_aliasMap.find(alias);
	if(aliasIt==m_aliasMap.end())
		return;
	m_aliasMap.erase(aliasIt);
}

void GameComponentCreator::ClearFactories()
{
	m_aliasMap.clear();
	m_factories.clear();
}

IGameComponent* GameComponentCreator::CreateObject(const core::string&name,GameEntityManager*mngr)
{
	IGameComponentFactory* f= GetFactory(name);
	if(!f)
	{
		gLogManager.log(mT("Game Component Factory with type: '")+name+mT("' was not found"),ELL_WARNING,EVL_Normal);
		return 0;
	}
	return f->CreateComponent(mngr);
}

}
}
