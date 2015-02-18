#include "stdafx.h"
#include "TypeResolver.h"
#include "PoseController.h"
#include "HumanGait.h"
#include "TimeTrigger.h"
#include "ContactTrigger.h"
#include "Tri_PD_Actuator.h"
#include "Parental_PD_Actuator.h"
#include "PositioningController.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		TypeResolver::TypeResolver()
		{
		}

		TypeResolver::~TypeResolver()
		{
		}

		Controller* TypeResolver::resolveController(const core::string &controllerType)
		{
			if (controllerType.equals_ignore_case(mT("PoseController")))
				return new PoseController();
			else if (controllerType.equals_ignore_case(mT("PositioningController")))
				return new PositioningController();
			return 0;
		}

		PoseModifier* TypeResolver::resolvePoseModifier(const core::string &poseModifierType)
		{
			if (poseModifierType.equals_ignore_case(mT("HumanGait")))
				return new HumanGait();
			return 0;
		}

		Actuator* TypeResolver::resolveActuator(const core::string &actuatorType)
		{
			if (actuatorType.equals_ignore_case(mT("Parental_PD")))
				return new Parental_PD_Actuator();
			else if (actuatorType.equals_ignore_case(mT("Tri_PD")))
				return new Tri_PD_Actuator();
			return 0;
		}

		Trigger* TypeResolver::resolveTrigger( const core::string &triggerType )
		{
			if (triggerType.equals_ignore_case(mT("Time")))
				return new TimeTrigger();
			else if (triggerType.equals_ignore_case(mT("Contact")))
				return new ContactTrigger();
			return 0;
		}

	}
}