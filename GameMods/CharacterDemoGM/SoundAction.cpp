


#include "stdafx.h"
#include "SoundAction.h"

#include "ValueProxy.h"
#include "GameLevel.h"

namespace mray
{
namespace GameMod
{

SoundAction::SoundAction():game::IGameAction(mT("SoundAction")),
	m_soundPath(mT("Path"),mT("")),m_soundVolume(mT("Volume"),0.4),m_minDistance(mT("MinDistance"),1),m_maxDistance(mT("MaxDistance"),100)
{
	AddAttribute(new ValueProxy(&m_soundPath));
	AddAttribute(new ValueProxy(&m_soundVolume));
	AddAttribute(new ValueProxy(&m_minDistance));
	AddAttribute(new ValueProxy(&m_maxDistance));
}
SoundAction::~SoundAction()
{
	if(m_owner && m_sound)
	{
		sound::ISoundManager* sndManager= m_owner->GetGameEntityManager()->GetGameLevel()->GetSoundManager();
		if(sndManager)
		{
			sndManager->removeSound(m_sound);
		}
	}
}

void SoundAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(m_owner && m_soundPath.value!=mT(""))
	{
		sound::ISoundManager* sndManager= m_owner->GetGameEntityManager()->GetGameLevel()->GetSoundManager();
		if(sndManager)
		{
			m_sound= sndManager->loadSound(m_soundPath.value,true);
			if(m_sound)
			{
				m_sound->setVolume(m_soundVolume.value);
				m_sound->setPosition(m_owner->GetShape()->getPosition());
				m_sound->setMinDistance(m_minDistance.value);
				m_sound->setMaxDistance(m_maxDistance.value);
				m_sound->setLooping(true);
				m_sound->play();
			}
		}
	}
}

void SoundAction::Update(float dt)
{
	if(m_sound)
	{
		if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode()){
			((scene::IRenderable*)m_owner->GetShape())->setVisible(true);
		}
		else{
			((scene::IRenderable*)m_owner->GetShape())->setVisible(false);
		}
		m_sound->setPosition(m_owner->GetShape()->getPosition());
	}
}



}
}

