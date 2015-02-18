

#include "stdafx.h"
#include "ClothComponent.h"
#include "IPhysicManager.h"
#include "IPhysicalCloth.h"
#include "SceneComponent.h"
#include "ISceneNode.h"
#include "Smesh.h"
#include "PhysicsComponent.h"
#include "GameEntityManager.h"
#include "MeshBufferData.h"

namespace mray
{
namespace game
{


IMPLEMENT_SETGET_PROP(ClothComponent,TargetNode,core::string,m_targetNode,"",,)

ClothComponent::ClothComponent(game::GameEntityManager* mngr)
{
	m_cloth=0;
	m_phyiscManage=mngr->GetPhysicsManager();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
	}
}
ClothComponent::~ClothComponent()
{

}

bool ClothComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;


	SceneComponent* comp=IGameComponent::RetriveComponent<SceneComponent>(m_ownerComponent,m_targetNode);
	if(!comp)
		return true;
	comp->InitComponent();

	if(!comp->GetSceneNode())
		return true;
	scene::IRenderable* r= comp->GetSceneNode()->GetAttachedNode(0);
	if(!r)
		return true;
	m_oldBuffer=r->getMesh()->getBuffer(0);
	m_desc.meshData=m_oldBuffer;
	comp->GetSceneNode()->updateAbsoluteTransformation();
	m_desc.globalPos=comp->GetSceneNode()->getAbsoluteTransformation();
	comp->GetSceneNode()->setPosition(0);
	comp->GetSceneNode()->setOrintation(math::quaternion::Identity);
	m_cloth=m_phyiscManage->createCloth(&m_desc);

	if(!m_cloth)
		return true;

	r->getMesh()->getBufferData(0)->setMeshBuffer(m_cloth->getMeshData());
	//m_cloth->AttachNode(r,0);

	for(int i=0;i<m_shapeAttachments.size();++i)
	{
		ShapeAttachmentInfo& ifo=m_shapeAttachments[i];
		PhysicsComponent* phcomp= this->RetriveComponent<PhysicsComponent>(m_ownerComponent, ifo.nodeName);
		if(!phcomp)
			continue;
		phcomp->InitComponent();
		physics::IPhysicalNode* node= phcomp->GetPhysicalNode();
		if(!node)
			continue;
		physics::IPhysicalShape* shape= node->GetShapeByName(ifo.shapeName);
		if(!shape)
			continue;
		for(int j=0;j<ifo.vertices.size();++j)
		{
			uint flags=(ifo.vertices[j].tearable?physics::ECAF_Tearable:0) | (ifo.vertices[j].twoWays?physics::ECAF_TwoWay:0 );
			m_cloth->attachVertexToShape(ifo.vertices[j].id,shape,ifo.vertices[j].localPos,flags);
		}
	}
	for(int i=0;i<m_coreAttachments.size();++i)
	{
		CoreAttachmentInfo& ifo=m_coreAttachments[i];
		PhysicsComponent* phcomp= this->RetriveComponent<PhysicsComponent>(m_ownerComponent, ifo.nodeName);
		if(!phcomp )
			continue;
		phcomp->InitComponent();
		if(!phcomp->GetPhysicalNode())
			continue;
		m_cloth->attachToCore(phcomp->GetPhysicalNode(),ifo.impulseThreshold,ifo.penetrationDepth,ifo.maxDeformationDistance);
	}
	return true;
}

void ClothComponent::Update(float dt)
{
	IGameComponent::Update(dt);
	m_cloth->update();
}

 xml::XMLElement* ClothComponent::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr;
	xml::XMLElement* e= IGameComponent::loadXMLSettings(elem);
	xml::XMLElement* desc=elem->getSubElement("ClothDesc");
	if(desc)
		m_desc.LoadFromXML(desc);
	xml::XMLElement* att=elem->getSubElement("Attachment");
	while(att)
	{
		ShapeAttachmentInfo ifo;
		attr=att->getAttribute("NodeName");
		if(attr)
			ifo.nodeName=attr->value;
		attr=att->getAttribute("ShapeName");
		if(attr)
			ifo.shapeName=attr->value;

		xml::XMLElement* ve=att->getSubElement("Vertex");
		while(ve)
		{
			VertexAttachmentInfo vifo;
			attr=ve->getAttribute("ID");
			if(attr)
				vifo.id=core::StringConverter::toInt(attr->value);
			attr=ve->getAttribute("LocalPos");
			if(attr)
				vifo.localPos=core::StringConverter::toVector3d(attr->value);

			attr=ve->getAttribute("TwoWays");
			if(attr)
				vifo.twoWays=core::StringConverter::toBool(attr->value);
			attr=ve->getAttribute("Tearable");
			if(attr)
				vifo.tearable=core::StringConverter::toBool(attr->value);
			ifo.vertices.push_back(vifo);
			ve=ve->nextSiblingElement("Vertex");
		}
		m_shapeAttachments.push_back(ifo);
		att=att->nextSiblingElement("Attachment");
	}
	att=elem->getSubElement("CoreAttachment");
	while(att)
	{
		CoreAttachmentInfo ifo;
		attr=att->getAttribute("NodeName");
		if(attr)
			ifo.nodeName=attr->value;
		attr=att->getAttribute("ImpulseThreshold");
		if(att)
			ifo.impulseThreshold=core::StringConverter::toFloat(attr->value);
		attr=att->getAttribute("PenterationDepth");
		if(att)
			ifo.penetrationDepth=core::StringConverter::toFloat(attr->value);
		attr=att->getAttribute("MaxDeformationDistance");
		if(att)
			ifo.maxDeformationDistance=core::StringConverter::toFloat(attr->value);
		m_coreAttachments.push_back(ifo);
		att=att->nextSiblingElement("CoreAttachment");
	}
	return e;
}



}
}


