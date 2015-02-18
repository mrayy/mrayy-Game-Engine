#include "stdafx.h"
#include "PoseController.h"
#include "..\Character.h"
#include "TypeResolver.h"
#include "Actuator.h"
#include "PoseModifier.h"
#include "FinitStateMachine.h"
#include "../PhysicalModel/Segment.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		PoseController::PoseController()
		{
			fsm = 0;
			poseModifier = 0;
			character=0;
		}
		PoseController::~PoseController()
		{
			delete poseModifier;
			delete fsm;
			std::map<core::string,Actuator*>::iterator it=actuators.begin();
			for(;it!=actuators.end();++it)
			{
				delete it->second;
			}
			actuators.clear();
		}

		void PoseController::load(const core::string &descFile, Character *character)
		{
			xml::XMLTree xmlTree;
			GCPtr<OS::IStream> stream=gFileSystem.openFile(descFile,OS::TXT_READ);
			if(!stream || !xmlTree.load(stream)){
				stream->close();
				gLogManager.log(mT("Couldn't parse XML Font File :\"")+descFile+mT("\""),ELL_WARNING);
				return;
			}
			stream->close();
			xml::XMLElement*e=xmlTree.getSubElement(mT("Controller"));

			if(!e)
			{
				gLogManager.log(mT("cann't find Controller tag :\"")+descFile+mT("\""),ELL_WARNING);
				return;
			}

			xml::xmlAttributesMapIT it= e->getAttributesBegin();
			xml::xmlAttributesMapIT end= e->getAttributesEnd();

			core::stringc attrName;
			core::stringc elemName;

			for(;it!=end;++it)
			{
				xml::XMLAttribute&attr= it->second;
				core::string_to_char(attr.name,attrName);
				attrName.make_lower();		
				if(attrName=="name")
				{
					m_name = attr.value;
				}
			}

			xml::xmlSubElementsMapIT eit=e->getElementsBegin();
			xml::xmlSubElementsMapIT eEnd=e->getElementsEnd();
			for(;eit!=eEnd;++eit)
			{
				if((*eit)->GetType()!=xml::ENT_Element)
					continue;
				xml::XMLElement* e2=dynamic_cast<xml::XMLElement*>(*eit);
				core::string_to_char(e2->getName(),elemName);
				elemName.make_lower();

				if(elemName=="actuator")
				{
					Actuator* actuator = TypeResolver::resolveActuator(e2->getAttribute(mT("type"))->value);

					actuator->load(e2, character);
					actuators[actuator->getName()]= actuator;
				}
				else if (elemName=="posemodifier")
				{
					poseModifier = TypeResolver::resolvePoseModifier(e2->getAttribute(mT("type"))->value);
					poseModifier->load(e2, character);
				}
				else if (elemName=="fsm")
				{
					fsm = new FinitStateMachine(e2, character, this);
				}
			}

		}

		void PoseController::reset()
		{
			fsm->reset();
			stopping = false;
			m_finished = false;
		}

		void PoseController::stop()
		{
			stopping = true;
		}

		bool PoseController::isFinished()
		{
			return m_finished;
		}

		void PoseController::update(float dt)
		{
			if (!m_finished)
			{
				fsm->update(dt);

				if (stopping && fsm->isStoppable())
				{
					m_finished = true;
				}
				else
				{
					if (fsm->isStateChanged())
					{
						if (fsm->m_currentState->balanceParams!=0 && poseModifier!=0)
						{
							poseModifier->setBalanceParams(fsm->m_currentState->balanceParams);
						}
						for (std::list<std::pair<Actuator*, ActuatorParams*>>::iterator it = fsm->m_currentState->actuatorsParams.begin();it!=fsm->m_currentState->actuatorsParams.end();++it)
						{
							Actuator* actuator = (*it).first;
							ActuatorParams* params = (*it).second;
							actuator->setParams(params);
						}
					}

					for (std::list<std::pair<Segment*, math::vector3d>>::iterator it = fsm->m_currentState->targetPose.begin();it!=fsm->m_currentState->targetPose.end();++it)
					{
						Segment* segment = (*it).first;
						segment->target = (*it).second;
					}

					if (poseModifier != 0)
					{
						poseModifier->modifyTargetPose(dt);
					}

					for (std::map<core::string,Actuator*>::iterator it = actuators.begin();it!=actuators.end();++it)
					{
						(*it).second->applyForces(dt);
					}
				}
			}
		}

		void PoseController::setControllingParams(ControllingParams* params)
		{
			poseModifier->setControllingParams(params);
		}

		const core::string& PoseController::getName()
		{
			return m_name;
		}

		Actuator* PoseController::getActuator(core::string& name)
		{
			return actuators.find(name)->second;
		}
	}
}