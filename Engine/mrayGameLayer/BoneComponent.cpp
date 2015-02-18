

#include "stdafx.h"
#include "BoneComponent.h"

#include "BoneNode.h"
#include "SceneComponent.h"
#include "ISceneNode.h"
#include "Skeleton.h"
#include "SMesh.h"
#include "MeshBufferData.h"
#include "IDebugDrawManager.h"
#include "DefaultColors.h"

namespace mray
{
namespace game
{

IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode,BoneComponent,core::string,mT("TargetNode"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode,BoneComponent,core::string,SetTargetNode,GetTargetNode,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(OwnerNode,BoneComponent,core::string,mT("OwnerNode"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(OwnerNode,BoneComponent,core::string,SetOwnerNode,GetOwnerNode,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(BoneName,BoneComponent,core::string,mT("BoneName"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(BoneName,BoneComponent,core::string,SetBoneName,GetBoneName,,,false);

BoneComponent::BoneComponent(GameEntityManager*)
{
	m_attachedBone=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
		dic->addPropertie(&PropertyTypeOwnerNode::instance);
		dic->addPropertie(&PropertyTypeBoneName::instance);
	}
}
BoneComponent::~BoneComponent()
{
}

bool BoneComponent::InitComponent()
{
	if(!IMovableComponent::InitComponent())
		return false;
	SceneComponent* comp=RetriveComponent<SceneComponent>(GetOwnerComponent(),m_ownerNode);;
	if(!comp)
		return true;

	comp->InitComponent();
	if(!comp->GetSceneNode())
		return true;
	scene::ISceneNode* node=comp->GetSceneNode();
	const scene::AttachNodesList& lst= node->GetAttachedNodes();
	scene::AttachNodesList::const_iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		scene::IRenderable* r=(*it);
		if(!r->getMesh())
			continue;
		scene::Skeleton* s=r->getMesh()->getSkeleton();
		if(!s)
			continue;
		m_attachedBone= s->getBone(m_boneName);
		if(m_attachedBone)
			break;
	}

	IMovableComponent* targetNode=RetriveComponent<IMovableComponent>(GetOwnerComponent(),m_targetNode);;
	if(targetNode && m_attachedBone)
	{
		targetNode->InitComponent();
		if(targetNode->GetMovableObject())
		{
			m_attachedBone->updateAbsoluteTransformation();
			targetNode->GetMovableObject()->updateAbsoluteTransformation();
			targetNode->GetMovableObject()->addChild(m_attachedBone,true);
//			m_attachedBone->setPosition(0);
	//		m_attachedBone->setOrintation(math::quaternion::Identity);
		}
	}
	return true;
}

bool BoneComponent::SetTargetNode(const core::string& name)
{
	m_targetNode=name;
	return true;
}

bool BoneComponent::SetOwnerNode(const core::string& name)
{
	m_ownerNode=name;
	return true;
}

bool BoneComponent::SetBoneName(const core::string& name)
{
	m_boneName=name;
	return true;
}


IObject* BoneComponent::GetAttachedObject()
{
	return m_attachedBone;
}

scene::IMovable* BoneComponent::GetMovableObject()
{
	return m_attachedBone;
}

void BoneComponent::DebugRender(scene::IDebugDrawManager* renderer)
{

	return;
	if (!m_attachedBone)
		return;
	const scene::BonesList& lst=m_attachedBone->getSubBones();
	scene::BonesList::const_iterator it = lst.begin();
	for (; it != lst.end();++it)
	{
		renderer->AddLine(m_attachedBone->getAbsolutePosition(), (*it)->getAbsolutePosition(), video::DefaultColors::GreenYellow, 1, 0,false);
	}
}


}
}