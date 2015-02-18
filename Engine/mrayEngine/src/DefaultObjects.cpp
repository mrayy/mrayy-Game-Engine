#include "stdafx.h"


#include "IMovable.h"

#include "DefaultObjects.h"
#include "ObjectFactoryManager.h"
#include "TransAnimationGroup.h"
#include "TransformationAnimationMixer.h"
#include "TransformationAnimationTrack.h"

namespace mray{

class TransAnimationMixerFactory:public ObjectFactory<IObject>
{
public:
	virtual const core::string& GetType(){
		return animation::TransAnimationGroup::getClassType();
	}
	virtual IObject* CreateObject(){
		return new animation::TransformationAnimationMixer();
	}
};
class AnimationTrackGroupFactory:public ObjectFactory<IObject>
{
public:
	virtual const core::string& GetType(){
		return animation::AnimationTrackGroup::getClassType();
	}
	virtual IObject* CreateObject(){
		return new animation::AnimationTrackGroup(mT(""));
	}
};

class TransAnimationGroupFactory:public ObjectFactory<IObject>
{
public:
	virtual const core::string& GetType(){
		return animation::TransAnimationGroup::getClassType();
	}
	virtual IObject* CreateObject(){
		return new animation::TransAnimationGroup(mT(""));
	}
};

class transformationAnimationTrackFactory:public ObjectFactory<IObject>
{
public:
	virtual const core::string& GetType(){
		return animation::TransformationAnimationTrack::getClassType();
	}
	virtual IObject* CreateObject(){
		return new animation::TransformationAnimationTrack(mT(""),0,0,0,0);
	}
};

class AnimationMixerFactory:public ObjectFactory<IObject>
{
public:
	virtual const core::string& GetType(){
		return animation::AnimationMixer::getClassType();
	}
	virtual IObject* CreateObject(){
		return new animation::AnimationMixer();
	}
};


DefaultObjects::DefaultObjects()
{
}
DefaultObjects::~DefaultObjects()
{
}

void DefaultObjects::registerDefaultObjects()
{
	DefaultObjects::getInstance().RegisterFactory(new AnimationMixerFactory());
	DefaultObjects::getInstance().RegisterFactory(new AnimationTrackGroupFactory());
	DefaultObjects::getInstance().RegisterFactory(new TransAnimationGroupFactory());
	DefaultObjects::getInstance().RegisterFactory(new transformationAnimationTrackFactory());
}
void DefaultObjects::removeDefaultObjects()
{
	DefaultObjects::getInstance().UnregisterFactory(animation::AnimationMixer::getClassType());
	DefaultObjects::getInstance().UnregisterFactory(animation::AnimationTrackGroup::getClassType());
	DefaultObjects::getInstance().UnregisterFactory(animation::TransAnimationGroup::getClassType());
	DefaultObjects::getInstance().UnregisterFactory(animation::TransformationAnimationTrack::getClassType());
}

}

