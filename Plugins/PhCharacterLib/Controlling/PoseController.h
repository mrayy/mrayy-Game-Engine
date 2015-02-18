/********************************************************************
	created:	2010/06/18
	created:	18:6:2010   0:52
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\PoseController.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	PoseController
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef PoseController_h__
#define PoseController_h__
#include "Controller.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class Character;
		class ControllingParams;
		class Actuator;
		class PoseModifier;
		class FinitStateMachine;

		class PoseController:public Controller
		{
			public:
				PoseController();
				virtual ~PoseController();			
				void load(const core::string& descFile, Character* character);
				void update(float dt);
				void setControllingParams(ControllingParams* params);
				void setPoseModifier(PoseModifier* poseModifier);
				Actuator* getActuator(core::string& name);
				const core::string& getName();
				void reset();
				void stop();
				bool isFinished();


				PoseModifier* poseModifier;
			private:
				FinitStateMachine* fsm;
				std::map<core::string,Actuator*> actuators;
				Character* character;
				bool stopping,m_finished;
				core::string m_name;
		};
	}
}
#endif // PoseController_h__