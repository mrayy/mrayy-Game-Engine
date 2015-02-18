#include "stdafx.h"

#include "IMovable.h"
#include "IMeshBuffer.h"
#include "MorphAnimationTrack.h"
#include "IMorphAnimator.h"


namespace mray{
namespace animation{



xml::XMLElement* MorphAnimationTrack::MorphKeyFrame::loadXMLSettings(xml::XMLElement* elem)
{
	return 0;
}
xml::XMLElement* MorphAnimationTrack::MorphKeyFrame::exportXMLSettings(xml::XMLElement* elem){
	if(m_value){
		xml::XMLElement* e=IKeyFrame::exportXMLSettings(elem);
		e->addAttribute(mT("Target"),m_value->getMeshName());
	}
	return elem;
}


MorphAnimationTrack::MorphAnimationTrack(scene::IMorphAnimator* anim,const core::string& name,ulong catagoryBits,ulong priority):
KeyAnimationTrack(name,0,catagoryBits,priority)
{
	m_resTime=0;
	m_anim=anim;
	m_res1=0;
	m_res2=0;
}
MorphAnimationTrack::~MorphAnimationTrack(){
}


void MorphAnimationTrack::setAnimator(scene::IMorphAnimator* anim)
{
	m_anim=anim;
}
scene::IMorphAnimator* MorphAnimationTrack::getAnimator()
{
	return m_anim;
}

IKeyFrame* MorphAnimationTrack::createKeyFrame(float time,scene::IMeshBuffer* b){
	return new MorphKeyFrame(time,b);
}
void MorphAnimationTrack::animate(float time,float weight){
	IKeyFrame*kf1,*kf2;
	m_resTime=getKeyAtTime(time,&kf1,&kf2);

	m_res1=0;
	m_res2=0;

	if(kf1){
		m_res1=((MorphKeyFrame*)kf1)->getValue();
		if(kf2){
			m_res2=((MorphKeyFrame*)kf2)->getValue();
		}
	}

}
uint MorphAnimationTrack::getTransformationType(){
	return 0;
}

bool MorphAnimationTrack::apply(float time,float weight){
	animate(time,weight);

	if(!m_anim || !m_res1 && !m_res2)
		return false;

	if(m_res1 )
	{
		float t=1-m_resTime;
		if(!m_res2)
			t=1;
		m_anim->setTarget(0,m_res1,t);
	}
	if(m_res2 )
	{
		m_anim->setTarget(1,m_res2,m_resTime);
	}
	return true;
}


IAnimationTrack* MorphAnimationTrack::duplicate(){
	MorphAnimationTrack* anim=new MorphAnimationTrack(m_anim,mT(""),0,0);
	copy(anim);
	return anim;
}
void MorphAnimationTrack::copy(IAnimationTrack* o){
	KeyAnimationTrack::copy(o);
	MorphAnimationTrack* ao=dynamic_cast<MorphAnimationTrack*>(o);
	if(!ao)
		return;
}

void MorphAnimationTrack::loadKeyFrame(xml::XMLElement*elem){
	GCPtr<IKeyFrame> kf=createKeyFrame(0,0);
	kf->loadXMLSettings(elem);
}

}
}
