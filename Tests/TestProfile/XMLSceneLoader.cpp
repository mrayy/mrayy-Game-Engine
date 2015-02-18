
#include "stdafx.h"
#include "XMLSceneLoader.h"
#include "TransformationAnimationTrack.h"
#include "XMLTree.h"
#include "MeshFileCreatorManager.h"
#include "AnimationController.h"



namespace mray
{

XMLSceneLoader::XMLSceneLoader(scene::ISceneManager* manager,physics::IPhysicManager* phManager)
{
	m_phManager=phManager;
	m_manager=manager;
}
bool XMLSceneLoader::Load(OS::IStream* stream,scene::ISceneNode*parent,
	std::vector<scene::ISceneNode*>& snodes,std::vector<physics::IPhysicalNode*>& phNodes)
{
	if(!stream)
		return false;

	xml::XMLTree tree;
	if(!tree.load(stream))
		return false;
	xml::XMLElement* root= tree.getSubElement(mT("Nodes"));
	if(!root)
		return false;
	xml::XMLElement* node=root->getSubElement(mT("Node"));
	while(node)
	{
		scene::ISceneNode* snode=loadNode(node,parent,snodes,phNodes);
		node=node->nextSiblingElement(mT("Node"));
	}
	node=root->getSubElement(mT("Collision"));
	while(node)
	{
		physics::IPhysicalNode* phnode=loadPhNode(node,0);
		if(phnode)
		{
			phNodes.push_back(phnode);
		}
		node=node->nextSiblingElement(mT("Collision"));
	}

	return true;
}

scene::ISceneNode* XMLSceneLoader::loadNode(xml::XMLElement* elem,scene::ISceneNode*parent,std::vector<scene::ISceneNode*>& snodes,std::vector<physics::IPhysicalNode*>& phNodes)
{
	if(!elem)
		return 0;
	xml::XMLAttribute* attr;
	attr=elem->getAttribute(mT("Name"));
	scene::ISceneNode* node=new scene::ISceneNode(attr->value,0,0);
	if(parent)
		parent->addChild(node);
	else m_manager->addSceneNode(node);

	node->SetSceneManager(m_manager);
	//node->addChild(snode2);
	snodes.push_back(node);

	attr=elem->getAttribute(mT("Position"));
	if(attr)
		node->setPosition(core::StringConverter::toVector3d(attr->value));
	attr=elem->getAttribute(mT("Rotation"));
	if(attr)
		node->setOrintation(core::StringConverter::toVector3d(attr->value));
	attr=elem->getAttribute(mT("Scale"));
	if(attr)
		node->setScale(core::StringConverter::toVector3d(attr->value));

	attr=elem->getAttribute(mT("MeshFile"));
	if(attr)
	{
		GCPtr<scene::MeshRenderableNode> mesh=scene::MeshFileCreatorManager::getInstance().LoadFromFile(attr->value,true);
		if(mesh)
			node->AttachNode(mesh);
	}
	node->updateAbsoluteTransformation();
	xml::XMLElement* tracks=elem->getSubElement(mT("AnimationTrackGroup"));
	if(tracks)
	{
		xml::XMLElement* tr=tracks->getSubElement(mT("AnimationTrack"));
		while(tr)
		{
			animation::TransformationAnimationTrack* t=
				new animation::TransformationAnimationTrack(mT(""),0,0,0,0);
			t->loadXMLSettings(tr);
			animation::AnimationController*c=new animation::AnimationController();
			c->SetStartEnd(t->getStart(),t->getEnd());
			node->AttachAnimationTrack(t,c);

			tr=tr->nextSiblingElement(mT("AnimationTrack"));
		}
	}


	xml::XMLElement* xmlnode=elem->getSubElement(mT("Node"));
	while(xmlnode)
	{
		scene::ISceneNode* snode2=loadNode(xmlnode,node,snodes,phNodes);
		xmlnode=xmlnode->nextSiblingElement(mT("Node"));
	}


	xmlnode=elem->getSubElement(mT("Collision"));
	while(xmlnode)
	{
		physics::IPhysicalNode* phnode=loadPhNode(xmlnode,node);
		if(phnode)
		{
			phNodes.push_back(phnode);
		}
		xmlnode=xmlnode->nextSiblingElement(mT("Collision"));
	}

	return node;

}

physics::IPhysicalNode* XMLSceneLoader::loadPhNode(xml::XMLElement* elem,scene::ISceneNode* parent)
{
	if(!elem)
		return 0;
	xml::XMLAttribute* attr;
	physics::PhysicalNodeDesc phDesc;

	attr=elem->getAttribute(mT("Name"));
	if(attr)
		phDesc.name=attr->value;
	math::vector3d pos;
	math::quaternion ori;
	math::vector3d scale(1);
	attr=elem->getAttribute(mT("Rotation"));
	if(attr)
	{
		math::vector3d a=core::StringConverter::toVector3d(attr->value);
		ori.fromEulerAngles(a.x,a.y,a.z);
	}
	attr=elem->getAttribute(mT("Position"));
	if(attr)
		pos=core::StringConverter::toVector3d(attr->value);
	attr=elem->getAttribute(mT("Scale"));
	if(attr)
		scale=core::StringConverter::toVector3d(attr->value);



	attr=elem->getAttribute(mT("MeshFile"));
	if(!attr)
		return 0;
	GCPtr<scene::MeshRenderableNode> mesh=scene::MeshFileCreatorManager::getInstance().LoadFromFile(attr->value,true);

	if(!mesh || !mesh->getMesh()->getBuffersCount())
		return 0;

	math::matrix4x4 m;
	ori.toMatrix(m);
	m.setTranslation(pos);
	if(scale!=1)
	{
		math::matrix4x4 m2;
		m2.setScale(scale);
		m=m2*m;
	}
	if(parent)
		m=parent->getAbsoluteTransformation()*m;

	pos=parent->getAbsoluteTransformation()*pos;
	ori=parent->getAbsoluteOrintation()*ori;

	std::list<physics::TriangleShapeDesc*> shDesc;
	for(int i=0;i<mesh->getMesh()->getBuffersCount();++i)
	{
		physics::TriangleShapeDesc* desc=new physics::TriangleShapeDesc();
		desc->autoDelete=true;
		desc->mesh=mesh->getMesh()->getBuffer(i);
		desc->name=mesh->getMesh()->getBuffer(i)->getMeshName();
		scene::SMeshManipulator::getInstance().applyTransformationOnMesh(desc->mesh,m);

		//shDesc[i].localSpace=m;
		desc->flags=0;

		phDesc.shapes.push_back(desc);
	}

	physics::IPhysicalNode* ret=m_phManager->createNode(&phDesc);
	//ret->setGlobalPosition(pos);
	//ret->setGlobalOrintation(ori);

	return ret;
}

}
