


/********************************************************************
created:	2010/07/17
created:	17:7:2010   17:26
filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AnimationMixer.h
file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
file base:	AnimationMixer
file ext:	h
author:		Mohamad Yamen Saraiji

purpose:	
*********************************************************************/

#ifndef AnimationMixer_h__
#define AnimationMixer_h__

#include "IAnimationMixer.h"
#include "AnimationTrackGroup.h"



namespace mray
{
namespace animation{

class MRAY_DLL AnimationMixer:public IAnimationMixer
{
	DECLARE_RTTI;
private:
protected:
public:
	AnimationMixer();
	virtual~AnimationMixer();

	virtual AnimationController* addTrackGroup(AnimationTrackGroup* group);
	virtual AnimationController* getAnimationController(const core::string&name);

	virtual AnimationTrackGroup* getTrackGroup(const core::string&name);
	virtual void removeTrackGroup(const core::string&name);
	virtual const ATGroupList&  getTrackGroups();

	virtual void update(float dt);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*   exportXMLSettings(xml::XMLElement* elem);

protected:

	typedef std::vector<AnimationController*> ControllersList;
	typedef std::map<core::string,int> ControllersMap;
	ATGroupList m_groups;
	ControllersMap m_controllersMap;
	ControllersList m_controllers;
};

}
}
#endif // AnimationMixer_h__

