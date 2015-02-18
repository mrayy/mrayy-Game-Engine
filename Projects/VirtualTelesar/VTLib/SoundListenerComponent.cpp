

#include "stdafx.h"
#include "SoundListenerComponent.h"
#include "GameEntity.h"
#include "GameEntityManager.h"
#include "ISoundManager.h"


namespace mray
{
namespace VT
{


SoundListenerComponent::SoundListenerComponent(game::GameEntityManager*mngr):MountableComponent(mngr)
{
	m_gain=PropertyTypeGain::instance.GetDefaultValue();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeGain::instance);
	}
}
SoundListenerComponent::~SoundListenerComponent()
{

	sound::ISoundManager* mngr=m_ownerEntity->GetCreator()->GetSoundManager();
	if(mngr && mngr->getActiveListener()==m_listener)
		mngr->setActiveListener(0);
	delete m_listener;
}

IMPLEMENT_SETGET_PROP(SoundListenerComponent,Gain,float,m_gain,0.5,core::StringConverter::toFloat,core::StringConverter::toString);


bool SoundListenerComponent::InitComponent()
{
	if(!MountableComponent::InitComponent())
		return false;
	sound::ISoundManager* mngr= m_ownerEntity->GetCreator()->GetSoundManager();
	if(mngr)
		m_listener=mngr->createListener();
	return true;
}

void SoundListenerComponent::SetActive()
{
	sound::ISoundManager* mngr=m_ownerEntity->GetCreator()->GetSoundManager();
	if(mngr)
	{
		mngr->setActiveListener(m_listener);
		mngr->setVolume(m_gain);
	}
}


void SoundListenerComponent::Update(float dt)
{
	MountableComponent::Update(dt);
	sound::ISoundManager* mngr=m_ownerEntity->GetCreator()->GetSoundManager();
	if(m_listener && mngr && mngr->getActiveListener()==m_listener)
	{
		m_listener->setPosition(GetAbsolutePosition());
		m_listener->setOreintation(GetAbsoluteOrientation());
	}
}

}
}

