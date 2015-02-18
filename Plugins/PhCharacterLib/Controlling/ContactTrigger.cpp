#include "stdafx.h"
#include "ContactTrigger.h"
#include "../Character.h"
#include "../PhysicalModel/Segment.h"
#include "CharacterManager.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		ContactTrigger::ContactTrigger()
		{
			m_actor = 0;
			m_environment = 0;
		}
		ContactTrigger::~ContactTrigger()
		{
			m_environment->unRegisterActor(m_actor);
		}

		void ContactTrigger::load( xml::XMLElement*elem, Character* character )
		{
			m_state = core::StringConverter::toInt(elem->getAttribute(mT("nextState"))->value);
			m_environment = character->GetCreator()->GetEnvironment();
			core::string str = elem->getAttribute(mT("object"))->value;
			m_actor = character->getSegment(str)->getActor();
			m_environment->registerActor(m_actor);
		}

		bool ContactTrigger::isTriggered()
		{
			return m_environment->isContacted(m_actor);
		}

		void ContactTrigger::update( float dt )
		{

		}
	}
}