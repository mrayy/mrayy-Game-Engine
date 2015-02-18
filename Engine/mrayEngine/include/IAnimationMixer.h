
/********************************************************************
	created:	2009/06/28
	created:	28:6:2009   22:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IAnimationMixer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IAnimationMixer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IAnimationMixer___
#define ___IAnimationMixer___

#include "IResource.h"
#include "AnimationTrackGroup.h"



namespace mray{
namespace animation{

	class AnimationController;

class MRAY_DLL IAnimationMixer:public IResource
{

	DECLARE_RTTI;

private:
protected:
	virtual uint calcSizeInternal();
public:

	virtual~IAnimationMixer();

	typedef std::map<core::string,AnimationTrackGroup*> ATGroupList;

	//virtual void addTrackGroup(GCPtr<AnimationTrackGroup>group)=0;
	virtual IAnimationMixer::AnimationTrackGroup* getTrackGroup(const core::string&name)=0;
	virtual void removeTrackGroup(const core::string&name)=0;
	virtual const ATGroupList& getTrackGroups()=0;

	virtual AnimationController* getAnimationController(const core::string&name)=0;

	virtual void update(float dt)=0;

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem)=0;
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem)=0;
};

MakeSharedPtrType(IAnimationMixer);


}
}


#endif //___IAnimationMixer___
