
#include "stdafx.h"
#include "ModelCollisionComponent.h"
#include "SMeshManipulator.h"
#include "MeshFileCreatorManager.h"



namespace mray
{
namespace game
{

IMPLEMENT_PROPERTY_TYPE_HEADER(ModelPath,ModelCollisionComponent,core::string,mT("ModelPath"),EPBT_Basic,mT("Model's path"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(ModelPath,ModelCollisionComponent,core::string,SetModelPath,GetModelPath,,,false);

ModelCollisionComponent::ModelCollisionComponent(GameEntityManager*mngr)
{
	m_scale=1;
	m_convex=true;
	m_shapeDesc=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeModelPath::instance);
	}
}
ModelCollisionComponent::ModelCollisionComponent()
{
	m_scale=1;
	m_convex=true;
	m_shapeDesc=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeModelPath::instance);
	}
	InitComponent();
}
ModelCollisionComponent::~ModelCollisionComponent()
{
	delete m_shapeDesc;
}
void ModelCollisionComponent::_OnTargetNodeChanged()
{
}
bool ModelCollisionComponent::InitComponent()
{
	IGameComponent::InitComponent();
	m_CollNode=scene::MeshFileCreatorManager::getInstance().LoadFromFile(m_modelPath,false);

	if(!m_CollNode || !m_CollNode->getMesh())
		return false;
	scene::SMeshManipulator::getInstance().scaleMesh(m_CollNode->getMesh()->getBuffer(0),m_scale);

	if(!m_shapeDesc)
	{
		if(m_convex)
		{
			m_shapeDesc=new physics::ConvexShapeDesc();
			((physics::ConvexShapeDesc*)m_shapeDesc)->meshPath=m_modelPath;
			((physics::ConvexShapeDesc*)m_shapeDesc)->mesh=m_CollNode->getMesh()->getBuffer(0);
		}else
		{
			m_shapeDesc=new physics::TriangleShapeDesc();
			((physics::TriangleShapeDesc*)m_shapeDesc)->meshPath=m_modelPath;
			((physics::TriangleShapeDesc*)m_shapeDesc)->mesh=m_CollNode->getMesh()->getBuffer(0);
		}
	}

	return true;
}

bool ModelCollisionComponent::SetConvex(bool c)
{
	m_convex=c;
	if(m_shapeDesc)
	{
		delete m_shapeDesc;
		if(m_convex)
		{
			m_shapeDesc=new physics::ConvexShapeDesc();
			((physics::ConvexShapeDesc*)m_shapeDesc)->meshPath=m_modelPath;
		}else
		{
			m_shapeDesc=new physics::TriangleShapeDesc();
			((physics::TriangleShapeDesc*)m_shapeDesc)->meshPath=m_modelPath;
		}
	}
	return true;
}
bool ModelCollisionComponent::GetConvex()const
{
	return m_convex;
}

bool ModelCollisionComponent::CopyShapeDesc(physics::IPhysicalShapeDesc*d)
{
	if(d->getType()==physics::ESS_Convex)
	{
		if(!m_convex)
		{
			m_convex=true;
			delete m_shapeDesc;
			m_shapeDesc=new physics::ConvexShapeDesc();
		}
		(*(physics::ConvexShapeDesc*)m_shapeDesc)=*(physics::ConvexShapeDesc*)d;
	}else if(d->getType()==physics::ESS_Convex)
	{
		if(m_convex)
		{
			m_convex=false;
			delete m_shapeDesc;
			m_shapeDesc=new physics::TriangleShapeDesc();
		}
		(*(physics::TriangleShapeDesc*)m_shapeDesc)=*(physics::TriangleShapeDesc*)d;
	}else
		return false;
	return true;
}
physics::IPhysicalShapeDesc* ModelCollisionComponent::GetShapeDesc()
{
	return m_shapeDesc;
}

bool ModelCollisionComponent::SetModelPath(const core::string& path)
{
	m_modelPath=path;
	if(m_inited)
		InitComponent();
	return true;
}
const core::string& ModelCollisionComponent::GetModelPath()const
{
	return m_modelPath;
}


}
}

