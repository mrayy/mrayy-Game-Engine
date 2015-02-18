#include "stdafx.h"

#include "IMovable.h"
#include "TransAnimationGroup.h"
#include "TransformationAnimationTrack.h"


namespace mray{
namespace animation{


TransAnimationGroup::TransAnimationGroup(const core::string&name):
	AnimationTrackGroup(name)
{
}
TransAnimationGroup::~TransAnimationGroup(){
}

void TransAnimationGroup::addTrack(TransformationAnimationTrack* track){
	AnimationTrackGroup::addTrack(track);
	//TransformationAnimationTrack *at=dynamic_cast<TransformationAnimationTrack*>(track.pointer());
}


AnimationTrackGroup* TransAnimationGroup::duplicate(bool contextOnly,bool playingTracks){
	TransAnimationGroup* o=new TransAnimationGroup(m_name);
	copy(o,contextOnly,playingTracks);
	return o;
}
void TransAnimationGroup::applyOnNode(float time,scene::IMovable* node,int id,float weight)
{
	IAnimationTrackList::iterator it= m_playingTracks.begin();
	for(;it!=m_playingTracks.end();++it){
		TransformationAnimationTrack* t=(TransformationAnimationTrack*)(*it);
		if(t->applyOnObject(time,node,id,weight))
		{
			break;
		}
	}
}
bool TransAnimationGroup::getTransformation(float time,int id,TransformKeyFrame&result)
{
	IAnimationTrackList::iterator it= m_playingTracks.begin();
	for(;it!=m_playingTracks.end();++it){
		TransformationAnimationTrack* t=(TransformationAnimationTrack*)(*it);
		if(t->getAffectedNodeID()==id)
		{
			t->interpolate(time,result);
			return true;
		}
	}
	return false;
}

void TransAnimationGroup::copy(AnimationTrackGroup* o,bool contextOnly,bool playingTracks){
	AnimationTrackGroup::copy(o,contextOnly,playingTracks);
/*	GCPtr<TransAnimationGroup> ao=o;
	if(!ao)
		return;*/
}

}
}
