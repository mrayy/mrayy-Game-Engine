#include "stdafx.h"
#include "FinitStateMachine.h"
#include "Trigger.h"
#include "PoseController.h"
#include "../Character.h"


namespace mray
{
	namespace PhysicsBasedCharacter
	{
		FinitStateMachine::FinitStateMachine(xml::XMLElement*elem, Character* character, PoseController* controller)
		{
			int numOfStates = core::StringConverter::toInt(elem->getAttribute(mT("numOfStates"))->value);
			int startState = core::StringConverter::toInt(elem->getAttribute(mT("startState"))->value);
			m_states .resize(numOfStates,0);

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

				if(elemName=="state")
				{
					State* state = new State(e, character, controller);
					 m_states[state->getID()] = state;
				}
			}

			m_currentState = 0;
			m_stoppable = false;
			m_stateChanged = false;
			m_startState = startState;
		}

		FinitStateMachine::~FinitStateMachine()
		{
			for(int i=0;i<m_states.size();++i)
			{
				delete m_states[i];
			}
		}

		void FinitStateMachine::reset()
		{
			m_currentState = 0;
			m_stoppable = false;
			m_stateChanged = false;
		}


		bool FinitStateMachine::isStoppable()
		{
			return m_stoppable;
		}

		bool FinitStateMachine::isStateChanged()
		{
			return m_stateChanged;
		}

		void FinitStateMachine::update(float dt)
		{
			m_stateChanged = false;
			if (m_currentState == 0)
			{
				goToState(m_startState);
			}
			for (std::list<Trigger*>::iterator it = m_currentState->triggers.begin(); it != m_currentState->triggers.end();++it)
			{
				(*it)->update(dt);
				if ((*it)->isTriggered())
				{
					goToState((*it)->getState());
					break;
				}
			}
		}
		void FinitStateMachine::goToState(int id)
		{
			m_stateChanged = true;

			if (m_currentState!=0)
			{
				for (std::list<Trigger*>::iterator it = m_currentState->triggers.begin(); it!=m_currentState->triggers.end();++it)
				{
					(*it)->off();
				}
				
				m_stoppable = m_currentState->isEndState();
			}

			m_currentState = m_states[id];

			for (std::list<Trigger*>::iterator it = m_currentState->triggers.begin(); it!=m_currentState->triggers.end();++it)
			{
				(*it)->on();
			}
		}
	}
}