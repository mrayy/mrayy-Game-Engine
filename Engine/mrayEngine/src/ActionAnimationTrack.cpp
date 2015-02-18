#include "stdafx.h"
 
#include "ActionAnimationTrack.h"

namespace mray
{
namespace animation
{


ActionKeyFrame::ActionKeyFrame()
{
}

ActionKeyFrame::ActionKeyFrame(const ActionKeyFrame*o)
{
	setTime(o->getTime());
	action=o->action;
	params=o->params;
}

IKeyFrame* ActionKeyFrame::duplicate()const
{
	return new ActionKeyFrame(this);
}

xml::XMLElement*  ActionKeyFrame::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* res=IKeyFrame::exportXMLSettings(elem);

	return res;
}
//////////////////////////////////////////////////////////////////////////


ActionAnimationTrack::ActionAnimationTrack(const core::string& name,AnimationTrackGroup*owner)
:KeyAnimationTrack(name,0,0,0,owner)
{
}
ActionAnimationTrack::~ActionAnimationTrack()
{
}


void ActionAnimationTrack::addKeyFrame(IKeyFrame*kf)
{
}
IKeyFrame* ActionAnimationTrack::createKeyFrame(float time,const core::string&action,const core::string&params)
{
	ActionKeyFrame* k=new ActionKeyFrame();
	k->setTime(time);
	k->action=action;
	k->params=params;
	return k;
}


IAnimationTrack* ActionAnimationTrack::duplicate()
{
	ActionAnimationTrack* res= new ActionAnimationTrack(mT(""),m_owner);

	return res;
}
void ActionAnimationTrack::copy(IAnimationTrack* o)
{
}
void ActionAnimationTrack::loadKeyFrame(xml::XMLElement*elem)
{
}


}
}

