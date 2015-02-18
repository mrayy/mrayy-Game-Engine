#include "stdafx.h"

#include "Skeleton.h"
#include "ILogManager.h"

#include "BoneNode.h"
#include "TransformationAnimationMixer.h"
#include "TransformationAnimationTrack.h"


namespace mray{
namespace scene{

Skeleton::Skeleton(const core::string&name)
{
	m_lastID=0;
	setResourceName(name);
	m_animationMixer=new animation::TransformationAnimationMixer();
}
Skeleton::~Skeleton()
{

// 	std::list<BoneNodePtr>::iterator it= m_bones.begin();
// 	for(;it!=m_bones.end();++it)
// 		delete *it;
	m_bones.clear();
	m_roots.clear();

	delete m_animationMixer;
}

uint Skeleton::calcSizeInternal(){
	return sizeof(Skeleton);
}

void Skeleton::registerNodes(){
/*
	BonesList::iterator it=m_bones.begin();
	for (;it!=m_bones.end();++it)
	{
		(*it)->preRender();
	}
	*/
}


animation::TransformationAnimationMixer* Skeleton::getAnimationMixer()
{
	return m_animationMixer;
}
BoneNodePtr Skeleton::createBone(const core::string& name,BoneNode*parent){
	BoneNodePtr bone=new BoneNode(name,0,this,parent);
	if(parent)
		parent->addBone(bone);
	/*
	else
		m_bones.push_back(bone);*/

	addBone(bone);
	return bone;
}

void Skeleton::addBone(BoneNodeCRef bone){
	BonesList::iterator it=m_bones.begin();
	for (;it!=m_bones.end();++it)
	{
		if((*it)==bone)
			return;
	}
	bone->setID(++m_lastID);
	bone->setSkeleton(this);
	m_bones.push_back(bone);
}

void Skeleton::update(float dt)
{
	m_animationMixer->update(dt);
	/*
	{
		BonesList::iterator it=m_bones.begin();
		for (;it!=m_bones.end();++it)
		{
			BoneNode*b=*it;
			b->lockBone();
			m_animationMixer->applyOnNode(b,b->getID());
			b->unlock();
		}
	}*/
	{
		for (int i=0;i<m_roots.size();++i)
		{
			m_roots[i]->update(dt);
		}
	}

	BonesList::const_iterator it= m_bones.begin();
	for(int i=0;it!=m_bones.end() ; ++i, ++it){
		math::matrix4x4 abb;
		BoneNode*b=*it;
		int id=b->getID()-1;//bones id start at 1
		FATAL_ERROR(id<0 || id>=m_bonesMatricies.size(),mT("Bone ID out of bounds!"));
		b->getFinalBoneTransformation(m_bonesMatricies[id]);
	}
}

void Skeleton::refreshRoots(){
	if(!m_bones.size()){
		gLogManager.log(mT("Skeleton::refreshRoots() , skeleton hase no bones to create roots list!"),ELL_WARNING);
		return;
	}
	m_roots.clear();
	int max_id=0;
	BonesList::iterator it=m_bones.begin();
	for (;it!=m_bones.end();++it)
	{
		if((*it)->getParent()==0){
			m_roots.push_back(*it);
		}
		if((*it)->getID()>max_id)
			max_id=(*it)->getID();
	}

	m_bonesMatricies.resize(max_id+1);
}
void Skeleton::addAnimationTrackGroup(animation::AnimationTrackGroup*group){
	animation::TransAnimationGroup* tgroup=dynamic_cast<animation::TransAnimationGroup*>(group);
	if(!tgroup)
		return;
	animation::AnimationController*c=m_animationMixer->addTrackGroup(tgroup);
	//connect tracks to bones
	animation::IAnimationTrackList::const_iterator p=group->getTracks().begin();
	animation::IAnimationTrackList::const_iterator end=group->getTracks().end();
	for(;p!=end;++p)
	{
		animation::IAnimationTrack* t=*p;
		animation::TransformationAnimationTrack* tt= dynamic_cast<animation::TransformationAnimationTrack*>(t);
		if(tt)
		{
			BoneNode* b=getBone(t->getName());
			if(b){
				tt->setAffectedNodeID(b->getID());
					//tt->SetNode(b);
				b->AttachAnimationTrack(tt,c);
			}
		}
	}
	group->refreshAffectIDMap();

}
int Skeleton::getRootBonesCount(){
	return m_roots.size();
}
BoneNode*Skeleton::getRootBone(int i){
	if(i>=m_roots.size())
		return 0;
	return m_roots[i];
}
int Skeleton::getBonesCount(){
	return m_bones.size();
}
BoneNode* Skeleton::getBone(int i){
	BonesList::iterator it=m_bones.begin();
	std::advance(it,i);
	if(it==m_bones.end())
		return 0;
	return *it;
}
BoneNode*Skeleton::getBone(const core::string& name){
	BonesList::iterator it=m_bones.begin();
	for (;it!=m_bones.end();++it)
	{
		if((*it)->getNodeName().equals_ignore_case(name))
			return *it;
	}
	return 0;
}

const BonesList& Skeleton::getBones(){
	return m_bones;
}


BoneNode* Skeleton::duplicateBone(BoneNode* b,Skeleton* sk)
{
	BoneNode* node=b->getDuplicate();
	node->setSkeleton(sk);

	const BonesList& subBones=b->getSubBones();
	BonesList::const_iterator p=subBones.begin();
	for (;p!=subBones.end();++p)
	{
		BoneNode* c=(*p);
		BoneNode* cd=duplicateBone(c,sk);
		if(cd)
		{
			//we need to save old bone id
			uint id=cd->getID();
			node->addBone(cd);
			cd->setID(id);
		}
	}
	return node;
}
Skeleton* Skeleton::getDuplicate()
{
	Skeleton* sk=new Skeleton(m_name+mT("_Dup"));
	refreshRoots();

	
	for(int i=0;i<m_roots.size();++i)
	{
		BoneNode*b=m_roots[i];
		BoneNode*node=duplicateBone(b,sk);
		if(sk){
			uint id=node->getID();
			sk->addBone(node);
			node->setID(id);
		}
	}
	sk->m_lastID=m_lastID;
	sk->refreshRoots();
	return sk;
}

}
}
