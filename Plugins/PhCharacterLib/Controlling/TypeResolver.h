/********************************************************************
	created:	2010/05/31
	created:	31:5:2010   0:21
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\TypeResolver.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	TypeResolver
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ControllerTypeResolver_h__
#define ControllerTypeResolver_h__

#include "Controller.h"
#include "PoseModifier.h"
#include "Actuator.h"
#include "Trigger.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class TypeResolver
		{
			public:
				TypeResolver();
				virtual ~TypeResolver();
				static  Controller* resolveController(const core::string &controllerType);
				static PoseModifier* resolvePoseModifier(const core::string &poseModifierType);
				static Actuator* resolveActuator(const core::string &actuatorType);
				static Trigger* resolveTrigger(const core::string &triggerType);
			protected:
				
			private:
		};
	}
}
#endif // ControllerTypeResolver_h__