#include "stdafx.h"

#include "MorphTrackGroup.h"


namespace mray{
namespace animation{


MorphTrackGroup::MorphTrackGroup(const core::string&name):
AnimationTrackGroup(name)
{
}
MorphTrackGroup::~MorphTrackGroup(){
}

void MorphTrackGroup::addTrack(MorphAnimationTrack* track){
	AnimationTrackGroup::addTrack(track);
	//TransformationAnimationTrack *at=dynamic_cast<TransformationAnimationTrack*>(track.pointer());
}


AnimationTrackGroup* MorphTrackGroup::duplicate(bool contextOnly,bool playingTracks){
	MorphTrackGroup* o=new MorphTrackGroup(m_name);
	copy(o,contextOnly,playingTracks);
	return o;
}
void MorphTrackGroup::apply(float time,float weight)
{
	IAnimationTrackList::iterator p=m_playingTracks.begin();
	IAnimationTrackList::iterator end=m_playingTracks.end();
	for(;p!=end;++p)
	{
		MorphAnimationTrack* t=(MorphAnimationTrack*)*p;
		t->apply(time,weight);
	}
}

void MorphTrackGroup::copy(AnimationTrackGroup* o,bool contextOnly,bool playingTracks){
	AnimationTrackGroup::copy(o,contextOnly,playingTracks);
/*	MorphTrackGroup*ao=o;
	if(!ao)
		return;*/
}

}
}

