 #include "stdafx.h"

#include "GameEntity.h"
#include "IPhysicsControl.h"
#include "IMovable.h"
#include "GameEntityManager.h"

#include "SceneManager.h"
#include "ILogManager.h"
#include "IGameVisitor.h"
#include "ILogManager.h"
#include "IGameComponent.h"
#include "AIComponent.h"
#include "SceneComponent.h"

#include "GameComponentCreator.h"
#include "ArchiveManager.h"
#include "MaterialResourceManager.h"
#include "IFileSystem.h"


namespace mray
{
namespace game
{


	
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

//	IMPLEMENT_PROPERTY_TYPE_HEADER(Action,GameEntity,core::string,mT("Action"),EPBT_Basic,mT("Action's Name"),mT(""));
//	IMPLEMENT_PROPERTY_TYPE_GENERIC(Action,GameEntity,core::string,SetActionByName,GetActionName,,,false)


//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RTTI(GameEntity,IObject)



GameEntity::GameEntity(const core::string& name,GameEntityManager*mngr):m_manager(mngr)
{
	CPropertieDictionary*dic=0;
	if(CreateDictionary(&dic))
	{
//		dic->addPropertie(&PropertyTypeAction::instance);
	}
	this->OnAttachedToComponent(this);
/*
	m_sceneComponent=new SceneComponent(mngr);
	m_aiComponent=new AIComponent(mngr);

	AddComponent(m_sceneComponent);
	AddComponent(m_aiComponent);

	new rwProperty<GameEntity,core::string>(this,&GameEntity::SetName,&GameEntity::GetName,EPT_STRING,mT("Name"),mT("Entity Name")));
	addPropertie(new readProperty<GameEntity,uint>(this,&GameEntity::_getID,EPT_INT,mT("ID"),mT("Entity ID")));
	addPropertie(new readProperty<GameEntity,uint>(this,&GameEntity::_getShapeID,EPT_INT,mT("SceneNode"),mT("Attached Scene node's ID")));
	addPropertie(new readProperty<GameEntity,core::string>(this,&GameEntity::_getActionName,EPT_STRING,mT("Action"),mT("Associated Action's Name")));*/
}
GameEntity::~GameEntity()
{
//	m_action=0;
	for(int i=0;i<m_loadedArchives.size();++i)
	{
		OS::ArchiveManager::getInstance().removeArchive(m_loadedArchives[i]);
	}
}


void GameEntity::AddTag(const GUID& t)
{
	if(HasTag(t))
		return;
	m_tags.push_back(t);
}
void GameEntity::RemoveTag(const GUID& t)
{
	for(GameEntityTagList::iterator it=m_tags.begin();it!=m_tags.end();++it)
	{
		if(*it==t)
		{
			m_tags.erase(it);
			break;
		}
	}
}
void GameEntity::ClearTags()
{
	m_tags.clear();
}
bool GameEntity::HasTag(const GUID& t)
{
	for(GameEntityTagList::iterator it=m_tags.begin();it!=m_tags.end();++it)
	{
		if(*it==t)
			return true;
	}
	return false;
}
const GameEntityTagList& GameEntity::GetTags()
{
	return m_tags;
}


/*
const core::string& GameEntity::GetActionName()const
{
	if(m_action)
		return m_action->GetName();
	return core::string::Empty;
}

bool GameEntity::SetGameAction(IGameAction* action)
{
	m_action=action;
	if(m_action)
		m_action->AttachToEntity(this);
	return m_action!=0;
}

IGameAction* GameEntity::GetGameAction()
{
	return m_action;
}

bool GameEntity::SetActionByName(const core::string&name)
{
	IGameAction*action=GameActionFactory::getInstance().CreateObject(name);
	return SetGameAction(action);
	
}
*/
void GameEntity::Initialize()
{
	std::list<IObjectComponent*>::iterator it=m_componentList.begin();
	for(;it!=m_componentList.end();++it)
	{
		IGameComponent* c=dynamic_cast<IGameComponent*>(*it);
		if(c)
			c->InitComponent();
	}
}

const core::string& GameEntity::GetName()
{
	return m_name;
}
bool GameEntity::SetName(const core::string &val) 
{
	m_name=val;
	return true;
}



void GameEntity::Update(float dt)
{
//	if(m_action)
//		m_action->Update(dt);

	IGameComponent::Update(dt);

	/*
	//update components
	std::list<IObjectComponent*>::iterator it=m_componentList.begin();
	for(;it!=m_componentList.end();++it)
	{
		IGameComponent* comp=dynamic_cast<IGameComponent*>(*it);
		if(comp)
			comp->Update(dt);
	}*/
}
/*
std::list<IGameComponent*> GameEntity::GetGameComponents(const RTTI* type)const
{
	const std::list<IObjectComponent*>& lst= GetComponent(type->getTypeName());
	std::list<IGameComponent*> ret;
	std::list<IObjectComponent*>::const_iterator it= lst.begin();
	for(;it!=lst.end();++it)
	{
		IGameComponent* c=dynamic_cast<IGameComponent*>(*it);
		if(c)
			ret.push_back(c);
	}
	return ret;
}

std::list<IGameComponent*> GameEntity::GetGameComponents(const core::string& type)const
{
	const std::list<IObjectComponent*>& lst= GetComponent(type);
	std::list<IGameComponent*> ret;
	std::list<IObjectComponent*>::const_iterator it= lst.begin();
	for(;it!=lst.end();++it)
	{
		IGameComponent* c=dynamic_cast<IGameComponent*>(*it);
		if(c)
			ret.push_back(c);
	}
	return ret;
}*/

xml::XMLElement* GameEntity::loadXMLSettings(xml::XMLElement* enode)
{
	xml::XMLElement* elem;
	xml::XMLAttribute* attr;
	elem=enode->getSubElement(mT("Archieve"));
	while(elem)
	{
		attr=elem->getAttribute(mT("Path"));
		if(attr)
		{
			OS::ArchiveManager::getInstance().addArchive(attr->value);
			m_loadedArchives.push_back(attr->value);
		}
		elem=elem->nextSiblingElement(mT("Path"));
	}
	elem=enode->getSubElement(mT("Materials"));
	while(elem)
	{
		attr=elem->getAttribute(mT("Path"));
		if(attr)
		{
			gMaterialResourceManager.parseMaterialXML(gFileSystem.openFile(attr->value));
			m_materialSets.push_back(attr->value);
		}
		elem=elem->nextSiblingElement(mT("Path"));
	}
	elem=IGameComponent::loadXMLSettings(enode);

	/*
	xml::XMLElement* comps=elem->getSubElement(mT("Components"));
	if(comps)
	{
		std::vector<IGameComponent*> compsList;
		xml::xmlSubElementsMapIT it=comps->getElementsBegin();
		xml::xmlSubElementsMapIT end=comps->getElementsEnd();
		for(;it!=end;++it)
		{
			if((*it)->GetType()!=xml::ENT_Element)
				continue;

			xml::XMLElement* e=(xml::XMLElement*)*it;
			core::string type= e->getName();
			IGameComponent* comp= GameComponentCreator::getInstance().CreateObject(type,GetGameEntityManager());
			if(comp && comp->IsUnique() && GetFirstComponent(type)!=0)
			{
				delete comp;
				comp=dynamic_cast<IGameComponent*>(GetFirstComponent(type));
			}
			if(comp)
			{
				AddComponent(comp);
				comp->loadXMLSettings(e);
				compsList.push_back(comp);
			}
		}

	}*/

	//Initialize();
// 	attr=elem->getAttribute(mT("ID"));
// 	if(attr)
// 	{
// 		m_id=core::StringConverter::toUInt(attr->value);
// 	}
	/*
	attr=elem->getAttribute(mT("SceneNode"));
	if(attr)
	{
		scene::ISceneManager*smngr=m_manager->GetSceneManager();
		if(smngr)
		{
			uint id=core::StringConverter::toUInt(attr->value);
			scene::ISceneNode* node=smngr->getNodeByID(id);
			if(!node)
			{
				gLogManager.log(mT("GameEntity::loadXMLSettings - Couldn't find scene node with ID: ")+attr->value,ELL_WARNING);
			}
			SetShape(node);
		}
	}*/

/*	xml::XMLElement*e=elem->getSubElement(mT("GameAction"));
	if(e)
	{
		xml::XMLAttribute*attr= e->getAttribute(mT("Name"));
		if(attr){
			SetActionByName(attr->value);
			if(m_action){
				m_action->loadXMLSettings(e);
			}else
			{
				gLogManager.log(mT("GameEntity::loadXMLSettings - Couldn't find GameAction with name : ")+attr->value,ELL_WARNING);
			}
		}
	}*/

	return elem;
}
xml::XMLElement*  GameEntity::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IGameComponent::exportXMLSettings(elem);

/*	if(m_action){
		m_action->exportXMLSettings(e);
	}
*/
	return e;
}


void GameEntity::Visit(IGameVisitor* visitor)
{
	visitor->OnVisit(this);
}

}
}
