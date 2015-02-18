#include "stdafx.h"


//#include "IMovable.h"
#include "TransformationAnimationMixer.h"
#include "ILogManager.h"
#include "AnimationController.h"
#include "AnimationLoggerSystem.h"

#include "DefaultObjects.h"

namespace mray{
namespace animation{



TransformationAnimationMixer::TransformationAnimationMixer(){

}
TransformationAnimationMixer::~TransformationAnimationMixer(){
	m_groups.clear();
}

AnimationController* TransformationAnimationMixer::addTrackGroup(TransAnimationGroup* group){
	return AnimationMixer::addTrackGroup(group);
}


 /*
void TransformationAnimationMixer::applyOnNodes()
{
	//first,normalize weights
	float totalWeight=0;
	for (int i=0;i<m_controllers.size();++i)
	{
		AnimationController* controller=m_controllers[i];
		
		if(!controller->isPlaying())
			continue;
		totalWeight+=controller->getWeight();
	}
	if(totalWeight==0)
		return;
	float invTotalWeight=1.0f/totalWeight;
	TransformKeyFrame kf;
	math::vector3d resultPos;
	math::vector3d resultScale(1);
	math::quaternion resultRot;
	bool first=true;
	for (int i=0;i<m_controllers.size();++i)
	{
		AnimationController* controller=m_controllers[i];
		if(!controller->isPlaying())
			continue;
		TransAnimationGroup*group=(TransAnimationGroup*) controller->getGroup();
		float w=controller->getWeight()*invTotalWeight;
		if(group->getTransformation(controller->getTime(),id,kf))
		{
			resultPos+=kf.getPos()*w;
			resultScale*=math::lerp(math::vector3d(1),kf.getScale(),w);
			
// 			math::quaternion q;
// 			q.Slerp(math::quaternion::Identity,kf.getOrintation(),w,true);
// 			resultRot=resultRot*q;
			if(first)
			{
				first=false;
				resultRot=kf.getOrintation();
			}else
			{
				resultRot.Slerp(resultRot,kf.getOrintation(),w,true);
			}
		}
	}
	node->setPosition(resultPos);
	node->setScale(resultScale);
	node->setOrintation(resultRot);
}*/
xml::XMLElement* TransformationAnimationMixer::loadXMLSettings(xml::XMLElement* rootE)
{
	xml::XMLElement* elem=AnimationMixer::loadXMLSettings(rootE);
	if(!elem)return 0;
	xml::XMLElement* e= elem->getSubElement(mT("AnimationTrackGroupTag"));
	for(;e;e=e->nextSiblingElement(mT("AnimationTrackGroupTag"))){
		xml::XMLAttribute*attr= e->getAttribute(mT("Type"));
		if(!attr){
			gAnimationLoggerSystem.log(mT("no type for AnimationTrackGroup."),ELL_WARNING);
			continue;
		}
		TransAnimationGroup* group=(TransAnimationGroup*)DefaultObjects::getInstance().CreateObject(attr->value);
		if(group==0){
			gAnimationLoggerSystem.log(mT("Unkown AnimationTrackGroup type: ")+attr->value,ELL_WARNING);
			continue;
		}
		group->loadXMLSettings(e);
		addTrackGroup(group);
	}
	return elem;
}


}
}
