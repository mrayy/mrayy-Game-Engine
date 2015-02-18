#include "stdafx.h"

#include "AnimationMixer.h"
#include "ILogManager.h"
#include "AnimationController.h"
#include "ObjectFactoryManager.h"


namespace mray{
namespace animation{


AnimationMixer::AnimationMixer(){

}
AnimationMixer::~AnimationMixer(){

	{
		ATGroupList::iterator it= m_groups.begin();
		for (;it!=m_groups.end();++it)
		{
			delete it->second;
		}
	}
	ControllersList::iterator p=m_controllers.begin();
	ControllersList::iterator end=m_controllers.end();
	for(;p!=end;++p)
	{
		delete (*p);
	}
	m_controllersMap.clear();
	m_groups.clear();
}
AnimationController* AnimationMixer::addTrackGroup(AnimationTrackGroup* group)
{
	m_groups.insert(ATGroupList::value_type(group->getName(),group));
	AnimationController* controller=new AnimationController();
	controller->SetStartEnd(group->getStart(),group->getEnd());
	m_controllers.push_back(controller);
	m_controllersMap[group->getName()]=m_controllers.size()-1;
	return controller;
}
AnimationController* AnimationMixer::getAnimationController(const core::string&name)
{
	ControllersMap::iterator it= m_controllersMap.find(name);
	if(it==m_controllersMap.end())
		return 0;
	return m_controllers[it->second];
}
AnimationTrackGroup *AnimationMixer::getTrackGroup(const core::string&name){
	ATGroupList::iterator it= m_groups.find(name);
	if(it==m_groups.end())
		return 0;
	return it->second;
}

void AnimationMixer::removeTrackGroup(const core::string&name){
	m_groups.erase(name);
}

const IAnimationMixer::ATGroupList&  AnimationMixer::getTrackGroups(){
	return m_groups;
}

void AnimationMixer::update(float dt){
	ControllersList::iterator p=m_controllers.begin();
	ControllersList::iterator end=m_controllers.end();
	for(;p!=end;++p)
	{
		(*p)->update(dt);
	}
}
xml::XMLElement* AnimationMixer::loadXMLSettings(xml::XMLElement* e)
{
	xml::XMLElement*elem=e->getSubElement(mT("AnimationMixer"));
	return elem;
}
xml::XMLElement*  AnimationMixer::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("AnimationMixer"));
	ATGroupList::iterator it=m_groups.begin();
	for(;it!=m_groups.end();++it){
		xml::XMLElement* tag=new xml::XMLElement(mT("AnimationTrackGroupTag"));
		tag->addAttribute(mT("Type"),(*it).second->getObjectType());
		(*it).second->exportXMLSettings(tag);
		e->addSubElement(tag);
	}
	elem->addSubElement(e);
	return e;
}



}
}

