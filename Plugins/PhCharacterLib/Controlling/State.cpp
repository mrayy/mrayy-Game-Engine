#include "stdafx.h"
#include "State.h"
#include "ActuatorParams.h"
#include "BalanceParams.h"
#include "../Character.h"
#include "PoseController.h"
#include "Trigger.h"
#include "Actuator.h"
#include "PoseModifier.h"
#include "TypeResolver.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		State::State(xml::XMLElement*elem, Character* character, PoseController* controller)
		{
			balanceParams = 0;
			xml::XMLAttribute* att = elem->getAttribute(mT("end"));
			if (att == 0)
			{
				m_endState = false;
			}
			else
				m_endState = core::StringConverter::toBool(att->value);
			m_id = core::StringConverter::toInt(elem->getAttribute(mT("id"))->value);

			xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
			xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
			core::stringc elemName;
			for(;eit!=eEnd;++eit)
			{
				if((*eit)->GetType()!=xml::ENT_Element)
					continue;
				xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
				core::string_to_char(e->getName(),elemName);
				elemName.make_lower();

				if(elemName=="target")
				{
					math::vector3d target = core::StringConverter::toVector3d(e->getAttribute(mT("value"))->value);
					core::string segmentName = e->getAttribute(mT("segment"))->value;
					targetPose.push_back(std::make_pair(character->getSegment(segmentName),target));
				}
				else if(elemName=="actuatorparams")
				{
					core::string actuatorName = e->getAttribute(mT("actuator"))->value;
					Actuator* actuator = controller->getActuator(actuatorName);
					ActuatorParams* params = actuator->createParams(e);
					actuatorsParams.push_back(std::pair<Actuator*, ActuatorParams*>(actuator, params));
				}
				else if(elemName=="balanceparams")
				{
					balanceParams = controller->poseModifier->createBalanceParams(e);
				}
				else if(elemName=="trigger")
				{
					Trigger* trigger = TypeResolver::resolveTrigger(e->getAttribute(mT("type"))->value);
					trigger->load(e, character);
					triggers.push_back(trigger);
				}
			}
		}
		State::~State()
		{
			for(std::list<Trigger*>::iterator it= triggers.begin();it!=triggers.end();++it)
			{
				delete *it;
			}
			for(std::list<std::pair<Actuator*, ActuatorParams*>>::iterator it= actuatorsParams.begin();it!=actuatorsParams.end();++it)
			{
				delete (*it).second;
			}

			triggers.clear();
			actuatorsParams.clear();
			targetPose.clear();

			delete balanceParams;
		}

		int State::getID()
		{
			return m_id;
		}

		bool State::isEndState()
		{
			return m_endState;
		}
	}
}