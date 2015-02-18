#include "stdafx.h"
#include "TimeTrigger.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		TimeTrigger::TimeTrigger()
		{
			m_active = false;
			elapsed_time = 0;
		}
		TimeTrigger::~TimeTrigger()
		{
		}

		void TimeTrigger::load( xml::XMLElement*elem, Character* character )
		{
			m_state = core::StringConverter::toInt(elem->getAttribute(mT("nextState"))->value);
			target_time = core::StringConverter::toFloat(elem->getAttribute(mT("duration"))->value);
		}

		bool TimeTrigger::isTriggered()
		{
			return  m_active&&(elapsed_time>=target_time);
		}

		void TimeTrigger::update( float dt )
		{
			if (m_active)
			{
				elapsed_time += dt;
			}
		}

		void TimeTrigger::on()
		{
			Trigger::on();
			elapsed_time = 0;
		}
	}
}