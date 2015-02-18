/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   4:11
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\FinitStateMachine.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	FinitStateMachine
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef FinitStateMachine_h__
#define FinitStateMachine_h__

#include "State.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
	namespace PhysicsBasedCharacter
	{
		class FinitStateMachine
		{
			public:

				State* m_currentState;

				FinitStateMachine(xml::XMLElement*elem, Character* character, PoseController* controller);
				virtual~FinitStateMachine();
				void reset();
				bool isStoppable();
				bool isStateChanged();
				void update(float dt);
			protected:
				int m_numOfStates;
				bool m_stoppable;
				bool m_stateChanged;
				int m_startState;
				std::vector<State*> m_states;
			private:
				void goToState(int id);
		};
	}
}
#endif // FinitStateMachine_h__