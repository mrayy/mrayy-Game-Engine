/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   16:18
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\TimeTrigger.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	TimeTrigger
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef TimeTrigger_h__
#define TimeTrigger_h__
#include "Trigger.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class TimeTrigger:public Trigger
		{
			public:
				TimeTrigger();
				virtual~TimeTrigger();
				virtual void load(xml::XMLElement*elem, Character* character);
				virtual bool isTriggered();
				virtual void update(float dt);
				virtual void on();
			protected:
				float elapsed_time;
				float target_time;
			private:
		};
	}
}
#endif // TimeTrigger_h__