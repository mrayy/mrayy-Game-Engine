#include "stdafx.h"
#include "MeshBufferData.h"
#include "MorphAnimationTrack.h"
#include "IMorphAnimator.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "MaterialResourceManager.h"

namespace mray{
namespace scene{

MeshBufferData::MeshBufferData():m_isVisible(1)
{
	m_material= gMaterialResourceManager.getMaterial("Default");
	if (!m_material)
	{
		m_material=new video::RenderMaterial();
		m_material->CreateTechnique(mT("Default"))->CreatePass(mT("Default"));
		gMaterialResourceManager.addResource(m_material,mT("Default"));
	}

	shaderCallback=new video::IShaderConstantsCallback();

	m_animationMixer=new animation::MorphAnimationMixer();

}
MeshBufferData::~MeshBufferData(){
}

void MeshBufferData::setMeshBuffer(const IMeshBufferPtr& buffer){
	m_buffer=buffer;
}
const IMeshBufferPtr& MeshBufferData::getMeshBuffer(){
	return m_buffer;
}

void MeshBufferData::setMaterial(const video::RenderMaterialPtr& mtrl){
	m_material=mtrl;
}
const video::RenderMaterialPtr& MeshBufferData::getMaterial(){
	return m_material;
}
void MeshBufferData::setShaderCallback(const video::IShaderConstantsCallbackPtr& callback){
	shaderCallback=callback;
}
const video::IShaderConstantsCallbackPtr& MeshBufferData::getShaderCallback(){
	return shaderCallback;
}

void MeshBufferData::setVisible(bool v){
	m_isVisible=v;
}
bool MeshBufferData::isVisible(){
	return m_isVisible;
}

void MeshBufferData::setMorphAnimator(const IMorphAnimatorPtr& anim)
{
	m_morphAnimator=anim;
}
const IMorphAnimatorPtr& MeshBufferData::getMorphAnimator()
{
	return m_morphAnimator;
}

const animation::MorphAnimationMixerPtr& MeshBufferData::getAnimationMixer()
{
	return m_animationMixer;
}
void MeshBufferData::addAnimationTrack(animation::MorphAnimationTrack* track)
{
	animation::MorphTrackGroup* g=new animation::MorphTrackGroup(track->getName());
	g->addTrack(track);

	if(m_morphAnimator)
		track->setAnimator(m_morphAnimator);
	m_animationMixer->addTrackGroup(g);
}

void MeshBufferData::setAnimator(const IMeshAnimatorPtr& anim){
	m_animator=anim;
}
const IMeshAnimatorPtr& MeshBufferData::getAnimator(){
	return m_animator;
}

void MeshBufferData::update(float dt)
{


	IMeshAnimator* a=getAnimator();
	if(a){
		a->update(dt);
	}else if(m_morphAnimator){
		m_animationMixer->update(dt);
		m_animationMixer->apply(0,0);
		m_morphAnimator->update(dt);
	}
}

MeshBufferData* MeshBufferData::getDuplicate(){
	MeshBufferData* bd=new MeshBufferData();
	bd->m_buffer=m_buffer->getDuplicate();
	bd->m_material=m_material;
	bd->shaderCallback=shaderCallback;
	if(m_animator){
		bd->m_animator=m_animator->duplicate();
		bd->m_animator->setMeshBuffer(bd->m_buffer);
	}
	if(m_morphAnimator){
		bd->m_morphAnimator=(IMorphAnimator*)m_morphAnimator->duplicate();
		bd->m_morphAnimator->setMeshBuffer(bd->m_buffer);
	}

	animation::IAnimationMixer::ATGroupList::const_iterator p=m_animationMixer->getTrackGroups().begin();
	animation::IAnimationMixer::ATGroupList::const_iterator end=m_animationMixer->getTrackGroups().end();
	for(;p!=end;++p)
	{
		animation::AnimationTrackGroup* t=(*p).second->duplicate(false,true);

		animation::IAnimationTrackList::const_iterator p2=t->getTracks().begin();
		animation::IAnimationTrackList::const_iterator end2=t->getTracks().end();
		for(;p2!=end2;++p2)
		{
			((animation::MorphAnimationTrack*)((*p2)))->setAnimator(bd->m_morphAnimator);
		}
		bd->m_animationMixer->addTrackGroup((animation::MorphTrackGroup*)t);
	}
	
	return bd;
}
MeshBufferData* MeshBufferData::duplicateContext(){
	MeshBufferData* bd=new MeshBufferData();
	bd->m_buffer=m_buffer;
	bd->m_material=m_material;
	bd->shaderCallback=shaderCallback;
	bd->m_animator=m_animator;
	//bd->m_animationMixer=m_animationMixer;
	bd->m_morphAnimator=m_morphAnimator;

	animation::IAnimationMixer::ATGroupList::const_iterator p=m_animationMixer->getTrackGroups().begin();
	animation::IAnimationMixer::ATGroupList::const_iterator end=m_animationMixer->getTrackGroups().end();
	for(;p!=end;++p)
	{
		bd->m_animationMixer->addTrackGroup((animation::MorphTrackGroup*)(*p).second->duplicate(true,true));
	}
	return bd;
}
uint MeshBufferData::calcSizeInternal(){
	return m_buffer->calcSizeInternal();
}


}
}
