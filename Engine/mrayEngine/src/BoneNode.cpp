#include "stdafx.h"


#include "BoneNode.h"
#include "Skeleton.h"

namespace mray{
namespace scene{

BoneNode::BoneNode(const core::string&name,uint id,Skeleton*skeleton,BoneNode*parent)
{
	m_transformationDirty=false;
	m_locked=false;
	m_parent=0;
	m_parentBone=0;
	setParentBone(parent);

	m_name=name;
	m_skeleton=skeleton;
	m_id=id;

	m_updateChilds=true;
	m_inheritTransformation = true;

}
BoneNode::~BoneNode(){
	m_subBones.clear();
}

Skeleton* BoneNode::getSkeleton(){
	return m_skeleton;
}
void BoneNode::setSkeleton(Skeleton*s)
{
	m_skeleton=s;
}

const math::box3d& BoneNode::getBoundingBox(){
	return math::box3d::Empty;
}
math::box3d BoneNode::getTransformedBoundingBox(){
	return math::box3d::Empty;
}

void BoneNode::removeChild(IMovable*elem, bool parentSpace){
	if (elem && elem->getParent() == this)
	{
		if (parentSpace)
		{
			elem->setPosition(elem->getAbsolutePosition());
			elem->setOrintation(elem->getAbsoluteOrintation());
			elem->setScale(elem->getAbsoluteScale());
		}
		MovableNodeList::iterator it = m_children.begin();
		for (; it != m_children.end();++it)
		{
			if ((*it).pointer() == elem)
			{
				m_children.erase(it);
				break;;
			}
		}
		if (dynamic_cast<BoneNode*>(elem))
		{
			removeBone(dynamic_cast<BoneNode*>(elem));
		}
		elem->setParent(0);
	}
}
void BoneNode::addChild(IMovableCRef elem,bool parentSpace){
	if(elem)
	{
		if(elem->getParent())
			elem->removeFromParent();

		elem->setParent(this);
		m_children.push_back(elem);
		if(parentSpace)
		{
			elem->setPosition(m_orintation.inverse()*elem->getPosition()-m_position);
			elem->setOrintation(m_orintation.inverse()*elem->getOrintation());
			if(m_scale.x!=0 && m_scale.y!=0 && m_scale.z!=0)
				elem->setScale(elem->getScale()/m_scale);
		}
/*
		BoneNode*b=dynamic_cast<BoneNode*>(elem);
		if(b)
		{
			b->updateBaseTransformation();
			addBone(b)
		}*/
	}
}
void BoneNode::removeFromParent(bool parentSpace){
}
const MovableNodeList& BoneNode::getChildren()
{
	return m_children;
}
const core::string& BoneNode::getNodeName()const
{
	return m_name;
}
bool BoneNode::setNodeName(const core::string&name){
	m_name=name;
	return true;
}

BoneNode*BoneNode::getParentBone(){
	return m_parentBone;
}
void BoneNode::setParentBone(BoneNode*p){
	if(m_parentBone){
		m_parentBone->removeBone(this);
	}
	m_parentBone=p;
	m_parent=p;
	m_transformationDirty=true;
	updateBaseTransformation();
}

IMovable*BoneNode::getParent(){
	return m_parent;
}
void BoneNode::setParent(IMovable*p){
	if(dynamic_cast<BoneNode*>(p))
	{
		if(m_parentBone){
			m_parentBone->removeBone(this);
		}
		m_parentBone=dynamic_cast<BoneNode*>(p);
	}
	m_parent=p;
	updateBaseTransformation();
}

void BoneNode::removeBone(BoneNode*bone){
	{
		BonesList::iterator it=m_subBones.begin();
		for(;it!=m_subBones.end();++it){
			if((*it).pointer()==bone){
				m_subBones.erase(it);
				break;
			}
		}
	}
	/*
	{
		NodeList::iterator it=m_children.begin();
		for(;it!=m_children.end();++it){
			if((*it).pointer()==bone){
				m_children.remove(it);
				break;
			}
		}
	}*/
}
void BoneNode::removeBone(const  core::string&name){

	
	{
		BonesList::iterator it=m_subBones.begin();
		for(;it!=m_subBones.end();++it){
			if((*it)->getNodeName()==name){
				m_subBones.erase(it);
				break;
			}
		}
	}/*
	{
		NodeList::iterator it=m_children.begin();
		for(;it!=m_children.end();++it){
			if((*it)->getNodeName()==name){
				m_children.remove(it);
				break;
			}
		}
	}*/
}


BoneNode*BoneNode::createChild(const core::string&name){
	BoneNode* bone=m_skeleton->createBone(name,this);
	m_subBones.push_back(bone);
	return bone;
}
void BoneNode::addBone(BoneNode*bone){
	bone->setParent(this);
	m_skeleton->addBone(bone);
	m_subBones.push_back(bone);
}


math::vector3d BoneNode::getAbsolutePosition()const{
	return m_absTransformation.getTranslation();
}
math::quaternion BoneNode::getAbsoluteOrintation()const{
	return m_absOrintation;
}
math::vector3d BoneNode::getAbsoluteScale()const{
	return 1;
}
const math::vector3d& BoneNode::getPosition()const{
	return m_position;
}
const math::quaternion& BoneNode::getOrintation()const{
	return m_orintation;
}
const math::vector3d& BoneNode::getScale()const{
	return m_scale;
}


void BoneNode::preRender(){
// 	{
// 		core::IteratorPair<BonesList> it(m_subBones);
// 		for(;!it.done();++it)
// 		{
// 			(*it)->preRender();
// 		}
// 	}
	/*
	{
		core::IteratorPair<NodeList> it(m_children);
		for(;!it.done();++it)
		{
			//(*it)->preRender();
		}
	}*/
}

bool BoneNode::setPosition(const math::vector3d& v)
{
	m_transformationDirty=true;
	if(m_locked){
		m_tmpPosition+=v;
		m_posChange=true;
	}else
		m_position=v;
	return true;
}
bool BoneNode::setScale(const math::vector3d& v)
{
	return false;
}
bool BoneNode::setOrintation(const math::quaternion& ori){
	m_transformationDirty=true;
	if(m_locked){
		m_tmpOri*=ori;
		m_oriChange=true;
	}else
		m_orintation=ori;
	return true;

}

const math::vector3d& BoneNode::getBasePosition()const
{
	return m_basePosition;
}
const math::vector3d& BoneNode::getAbsoluteBasePosition()const
{
	return m_absBasePosition;
}
void BoneNode::updateBaseTransformation()
{
	
	if (m_parent && m_inheritTransformation){
		if(m_parentBone){
			m_absBaseOrintation=m_parentBone->getAbsoluteBaseOrintation()*getBaseOrintation();
			m_absBasePosition=m_parentBone->getBaseTransformation()*(m_basePosition);


		}else{
			m_absBaseOrintation=m_parent->getAbsoluteOrintation()*getBaseOrintation();
			m_absBasePosition=m_parent->getAbsoluteOrintation()*m_basePosition + m_parent->getAbsolutePosition();
		}
		
	}
	else{
		m_absBasePosition=m_basePosition;
		m_absBaseOrintation=m_baseOrintation;
	}

	m_absBaseOrintation.toMatrix(m_baseTransformation);
	m_baseTransformation.setTranslation(m_absBasePosition);
/*
	math::matrix4x4 relBaseTrans;
	m_baseOrintation.toMatrix(relBaseTrans);
	relBaseTrans.setTranslation(m_basePosition);

	if(m_parent){
		if(m_parentBone){
			m_baseTransformation=m_parentBone->getBaseTransformation()*relBaseTrans;
		}else{
			m_baseTransformation=m_parent->getAbsoluteTransformation()*relBaseTrans;
		}
	}
	else{
		m_baseTransformation=relBaseTrans;
	}*/
	/*
	math::matrix4x4 m;
	if(m_parent)
	{
		if(m_parentBone){
			//math::vector3d tt=m_parentBone->getAbsoluteBaseOrintation()*m_basePosition;
			
			//m_parentBone->getBaseTransformation().transformVectPost(m_basePosition,tt);
			m_baseTransformation=m_parentBone->getBaseTransformation()*m;
		}else{
			m_baseTransformation=m;
		}
	}else{
		m_baseTransformation=m;
	}*/
	m_baseTransformation.getInverse(m_invBaseTransformation);


	BonesList::iterator it=m_subBones.begin();
	for(;it!=m_subBones.end();++it){
		(*it)->updateBaseTransformation();
	}
}

void BoneNode::captureTransformation(){
	m_basePosition=getPosition();
	m_baseOrintation=getOrintation();

	updateBaseTransformation();
}

void BoneNode::setBasePosition(const math::vector3d& v){
	m_basePosition=v;
	updateBaseTransformation();
}

void BoneNode::setBaseOrintation(const math::quaternion& q)
{
	m_baseOrintation=q;
	updateBaseTransformation();
}
const math::quaternion& BoneNode::getBaseOrintation()const
{
	return m_baseOrintation;
}
const math::quaternion& BoneNode::getAbsoluteBaseOrintation()const
{
	return m_absBaseOrintation;
}

const math::matrix4x4& BoneNode::getBaseTransformation()const	{
	return m_baseTransformation;
}
const math::matrix4x4& BoneNode::getInvBaseTransformation()const{
	return m_invBaseTransformation;
}

math::matrix4x4 BoneNode::getFinalBoneTransformation()const
{
	return getAbsoluteTransformation()*getInvBaseTransformation();
}
void BoneNode::getFinalBoneTransformation(math::matrix4x4& m)const
{
	m=getAbsoluteTransformation()*getInvBaseTransformation();
}

const BonesList& BoneNode::getSubBones()
{
	return m_subBones;
}

BoneNode* BoneNode::getBone(const  core::string&name){
	if(m_name.equals_ignore_case(name))
		return this;

	BonesList::iterator it=m_subBones.begin();
	for(;it!=m_subBones.end();++it){
		BoneNode*tmp=(*it)->getBone(name);
		if(tmp)return tmp;
	}
	return 0;
}


bool BoneNode::NeedChildUpdate(){
	return m_updateChilds;
}
void BoneNode::updateRelativeTransformation()
{
	//if(!hasChanged())return;

	m_orintation.toMatrix(m_relMatrix);

	m_relMatrix.setTranslation(m_position);
	m_updateChilds=true;
}
void BoneNode::updateAbsoluteTransformation()
{

	bool shouldUpdate=m_transformationDirty;
	if(m_parent){
		shouldUpdate = shouldUpdate || m_parent->NeedChildUpdate() && m_inheritTransformation;
	}
	//if(!shouldUpdate)return ;

	updateRelativeTransformation();
	if (m_parent && m_inheritTransformation)
	{
		math::vector3d absPos;
		m_absOrintation=m_parent->getAbsoluteOrintation()*m_orintation;
		m_absTransformation=m_parent->getAbsoluteTransformation()*getRelativeTransformation();
		//m_absOrintation.fromMatrix(m_absTransformation);
		/*

		math::matrix4x4 m1;
		m_parent->getAbsoluteOrintation().toMatrix(m1);

		m1.transformVectPre(m_position,absPos);
		absPos=absPos+m_parent->getAbsolutePosition();
		//m_parent->getAbsoluteTransformation().transformVectPost(m_position,absPos); 

		m_absOrintation.toMatrix(m_absTransformation);
		m_absTransformation.setTranslation(absPos);*/
	}
	else{
		m_absTransformation=getRelativeTransformation();
		m_absOrintation=m_orintation;
	}
	m_updateChilds=true;

	m_transformationDirty=false;
}
math::matrix4x4& BoneNode::getAbsoluteTransformation(){
	updateAbsoluteTransformation();
	return m_absTransformation;
}
const math::matrix4x4& BoneNode::getAbsoluteTransformation()const{
	return m_absTransformation;
}

const math::matrix4x4& BoneNode::getPrevAbsoluteTransformation()const{
	return m_prevTransformation;
}

const math::matrix4x4& BoneNode::getRelativeTransformation()const{
	return m_relMatrix;
}
void BoneNode::lockBone(){
	m_locked=true;

	m_posChange=0;
	m_oriChange=0;
	m_tmpPosition=math::vector3d::Zero;
	m_tmpOri=math::quaternion::Identity;
}
void BoneNode::unlock(){
	if(!m_locked)return;
	m_locked=false;
	if(m_posChange)
		m_position=m_tmpPosition;
	if(m_oriChange){
		m_orintation=m_tmpOri;
	}
}

void BoneNode::update(float dt)
{
	m_prevTransformation=m_prevTransformation;
	lockBone();
	IMovable::update(dt);
	unlock();

 	BonesList::iterator it=m_subBones.begin();
 	for(;it!=m_subBones.end();++it){
 		(*it)->update(dt);
	}
	m_updateChilds=false;
	/*
	NodeList::iterator cit=m_children.begin();
	for(;cit!=m_children.end();c++it){
	//	(*cit)->update(dt);
	}*/
}

BoneNode* BoneNode::getDuplicate()
{
	BoneNode* node=new BoneNode(m_name,m_id,0,0);
	node->setBasePosition(getBasePosition());
	node->setBaseOrintation(getBaseOrintation());
	node->setPosition(getPosition());
	node->setOrintation(getOrintation());

	return node;
}

}

}
