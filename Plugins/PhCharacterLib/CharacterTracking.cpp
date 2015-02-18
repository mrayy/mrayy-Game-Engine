#include "stdafx.h"
#include "CharacterTracking.h"
#include "Character.h"
namespace mray
{
namespace PhysicsBasedCharacter
{


CharacterTracking::CharacterTracking( xml::XMLElement*elem, Character* character )
{
	m_character = character;
	m_slot = core::StringConverter::toInt(elem->getAttribute(mT("slot_num"))->value);
	m_controller = elem->getAttribute(mT("controller"))->value;
	isTracking = false;
}

CharacterTracking::~CharacterTracking()
{

}

scene::Skeleton* CharacterTracking::getSkeleton()
{
	return m_character->getDuplicatedSkeleton();
}

void CharacterTracking::update( float dt )
{

}

void CharacterTracking::startTracking()
{
	m_character->playController(m_controller,m_slot);
	isTracking = true;
}

void CharacterTracking::stopTracking()
{
	m_character->forceStopController(m_slot);
	isTracking = false;
}

void CharacterTracking::pauseTracking()
{
	if (isTracking)
	{
		m_character->forceStopController(m_slot);
	}
}

void CharacterTracking::resumeTracking()
{
	if (isTracking)
	{
		m_character->playController(m_controller,m_slot);
	}
}
}
}