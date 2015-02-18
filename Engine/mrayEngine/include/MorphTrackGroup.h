

/********************************************************************
	created:	2010/07/17
	created:	17:7:2010   17:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\src\MorphTrackGroup.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\src
	file base:	MorphTrackGroup
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MorphTrackGroup_h__
#define MorphTrackGroup_h__


#include "AnimationTrackGroup.h"
#include "MorphAnimationTrack.h"

namespace mray
{
namespace animation
{

class MRAY_DLL MorphTrackGroup:public AnimationTrackGroup
{
	DECLARE_RTTI;
public:
	MorphTrackGroup(const core::string &name);
	virtual~MorphTrackGroup();

	virtual void addTrack(MorphAnimationTrack* track);

	void apply(float time,float weight=1);

	virtual AnimationTrackGroup* duplicate(bool contextOnly,bool playingTracks);
	virtual void copy(AnimationTrackGroup* other,bool contextOnly,bool playingTracks);
};


}
}


#endif // MorphTrackGroup_h__
