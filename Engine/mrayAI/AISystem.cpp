#include "stdafx.h"

 
#include "AISystem.h"
#include "INavWorld.h"
#include "GridSpacePartition.h"
#include "MessageDispatcher.h"
#include "PathFindingManager.h"

#include "AIIntializing.h"
#include "NavMesh.h"


namespace mray{
namespace AI{


AISystem::AISystem():m_enableVis(false)
{

	AIIntializing::InitAI();

	AISystemDesc desc;

	m_spacePartition=new GridSpacePartition();
	SetParameters(desc);
	m_navWorld=0;

	m_navWorld=new NavMesh(this);

	m_messageDispatcher=new MessageDispatcher();
	m_pathFindingManager=new PathFindingManager();
}
AISystem::~AISystem(){
	Clear();
	delete m_messageDispatcher;
	delete m_pathFindingManager;
	delete m_spacePartition;
	delete m_navWorld;
}
void AISystem::OnNavMeshLoaded()
{
	ActorsList::iterator it=m_actors.begin();
	for (;it!=m_actors.end();++it)
	{
		(*it)->UpdateWorldNode();
	}
}

void AISystem::SetParameters(const AISystemDesc& desc)
{
	m_spacePartition->SetWorldAABB(desc.worldBB);
	m_spacePartition->SetCellSize(desc.cellPartitionSize);
	m_spacePartition->Create();
}

void AISystem::AddActor(GCPtr<AIActor> actor){

	uint id=m_actorsID.AddObject(actor,actor->GetID());
	m_actors.push_back(actor);
	actor->SetID(id);
	actor->SetAISystem(this);
	m_spacePartition->AddActor(actor);
}

void AISystem::RemoveActor(AIActor*actor)
{
	m_spacePartition->RemoveActor(actor);
	m_actorsID.RemoveObject(actor->GetID());
	
	ActorsList::iterator it=m_actors.begin();
	for (;it!=m_actors.end();++it)
	{
		if((*it).pointer()==actor)
		{
			m_actors.erase(it);
			return;
		}
	}
}

void AISystem::RemoveAllActors()
{
	m_actorsID.Clear();
	m_actors.clear();
}

void AISystem::Clear()
{
	m_pathFindingManager->ClearRequests();
	RemoveAllActors();
	m_navWorld->Clear();
}

void AISystem::QueryActors(const math::box3d& box,AIActorList&actors)
{
	m_spacePartition->QueryActors(box,actors);
}

void AISystem::Update(float dt){


	m_messageDispatcher->ProcessMessages();
	m_pathFindingManager->ProcessRequests();

	ActorsList::iterator it=m_actors.begin();
	for (;it!=m_actors.end();++it)
	{
		(*it)->Update(dt);
	}

	m_spacePartition->Update();
}

MessageDispatcher* AISystem::GetMessageDispatcher()
{
	return m_messageDispatcher;
}

INavWorld* AISystem::GetNavigationWorld()
{
	return m_navWorld;
}

PathFindingManager* AISystem::GetPathFindingManager()
{
	return m_pathFindingManager;
}

void AISystem::EnableVisualize(bool e)
{
	m_enableVis=e;
}
bool AISystem::IsEnableVisualize()
{
	return m_enableVis;
}
void AISystem::Visualize(IRenderDevice*dev)
{
	//m_spacePartition->Visualize(dev);
	ActorsList::iterator it=m_actors.begin();
	for(;it!=m_actors.end();++it){
		if((*it)->IsEnableVisualize())
			(*it)->Visualize(dev);
	}

	if(m_navWorld->IsEnableVisualize())
		m_navWorld->Visualize(dev);
}


void AISystem::loadXMLSettings(xml::XMLElement* e)
{
	AISystemDesc desc;
	xml::XMLAttribute*attr;
	attr=e->getAttribute(mT("WorldBB"));
	if(attr)
		desc.worldBB=core::StringConverter::toBox3d(attr->value);
	attr=e->getAttribute(mT("CellSize"));
	if(attr)
		desc.cellPartitionSize=core::StringConverter::toFloat(attr->value);


	xml::XMLElement* elem=e->getSubElement(mT("NavigationMesh"));
	if(elem)
		m_navWorld->loadXMLSettings(elem);
	if(m_navWorld)
	{
		desc.worldBB=m_navWorld->GetBoundingBox();
	}
	SetParameters(desc);
}
xml::XMLElement*  AISystem::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("AISystem"));
	elem->addSubElement(e);

	e->addAttribute(mT("WorldBB"),core::StringConverter::toString(m_spacePartition->GetWorldAABB()));
	e->addAttribute(mT("CellSize"),core::StringConverter::toString(m_spacePartition->GetCellSize()));

	m_navWorld->exportXMLSettings(e);

	return e;
}


}
}