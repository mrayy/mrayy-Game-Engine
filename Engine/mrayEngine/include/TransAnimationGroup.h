
/********************************************************************
	created:	2009/03/12
	created:	12:3:2009   21:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TransAnimationGroup.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TransAnimationGroup
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	"Transformation Animation Group"
*********************************************************************/

#ifndef ___TransAnimationGroup___
#define ___TransAnimationGroup___

#include "AnimationTrackGroup.h"
#include "TransformKeyFrame.h"

namespace mray{
namespace scene
{
	class IMovable;
}
namespace animation{

	class TransformationAnimationTrack;

class MRAY_DLL TransAnimationGroup:public AnimationTrackGroup
{
	DECLARE_RTTI;
public:
	TransAnimationGroup(const core::string &name);
	virtual~TransAnimationGroup();

	virtual void addTrack( TransformationAnimationTrack* track);

	void applyOnNode(float time,scene::IMovable* node,int id,float weight=1);
	bool getTransformation(float time,int id,TransformKeyFrame&result);

	virtual AnimationTrackGroup* duplicate(bool contextOnly,bool playingTracks);
	virtual void copy(AnimationTrackGroup* other,bool contextOnly,bool playingTracks);
};

MakeSharedPtrType(TransAnimationGroup)

}
}


#endif //___TransAnimationGroup___
