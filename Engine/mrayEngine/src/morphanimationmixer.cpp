#include "stdafx.h"

#include "MorphAnimationMixer.h"
#include "AnimationController.h"


namespace mray
{
namespace animation{

	MorphAnimationMixer::MorphAnimationMixer()
{
}
MorphAnimationMixer::~MorphAnimationMixer()
{
}

void MorphAnimationMixer::addTrackGroup(MorphTrackGroup*group)
{
	AnimationMixer::addTrackGroup(group);
}

void MorphAnimationMixer::apply(float time,float weight)
{
	float totalWeight=0;
	for(int i=0;i<m_controllers.size();++i)
	{
		AnimationController* controller=m_controllers[i];

		if(!controller->isPlaying())
			continue;
		totalWeight+=controller->getWeight();
	}
	if(totalWeight==0)
		return;
	float invTotalWeight=1.0f/totalWeight;
	ATGroupList::iterator it=m_groups.begin();
	for(;it!=m_groups.end();++it)
	{
		MorphTrackGroup*group=(MorphTrackGroup*) it->second;
		AnimationController* controller=m_controllers[m_controllersMap[group->getName()]];
		if(!controller->isPlaying())
			continue;
		group->apply(controller->getTime(),controller->getWeight()*invTotalWeight);
	}
}
xml::XMLElement* MorphAnimationMixer::loadXMLSettings(xml::XMLElement* elem)
{
	return 0;
}

}
}
