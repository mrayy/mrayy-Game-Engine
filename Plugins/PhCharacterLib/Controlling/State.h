/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   4:12
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\State.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	State
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef State_h__
#define State_h__

namespace mray
{	
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{
		class Character;
		class Character;
		class PoseController;
		class Segment;
		class Trigger;
		class Actuator;
		class PoseController;
		class BalanceParams;
		class ActuatorParams;

		class State
		{
			public:
				State(xml::XMLElement*elem, Character* character, PoseController* controller);
				virtual~State();
				virtual int getID();
				virtual bool isEndState();


				BalanceParams* balanceParams;
				std::list<std::pair<Segment*, math::vector3d>> targetPose;
				std::list<std::pair<Actuator*, ActuatorParams*>> actuatorsParams;
				std::list<Trigger*> triggers;

			protected:
				bool m_endState;
				int m_id;
			private:
		};
	}
}
#endif // State_h__