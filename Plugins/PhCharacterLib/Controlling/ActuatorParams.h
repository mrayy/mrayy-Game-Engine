/********************************************************************
	created:	2010/06/18
	created:	18:6:2010   20:30
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\ActuatorParams.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	ActuatorParams
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ActuatorParams_h__
#define ActuatorParams_h__

#include "Actuator.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{
		class Actuator;
		class ActuatorParams
		{
			public:
				ActuatorParams(){};
				virtual~ActuatorParams(){};
			protected:
				
			private:
		};
	}
}
#endif // ActuatorParams_h__