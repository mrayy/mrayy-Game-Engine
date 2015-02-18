/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   16:18
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\ContactTrigger.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	ContactTrigger
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ContactTrigger_h__
#define ContactTrigger_h__
#include "Trigger.h"
#include "../Environment.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class ContactTrigger:public Trigger
		{
			public:
				ContactTrigger();
				virtual~ContactTrigger();
				virtual void load(xml::XMLElement*elem, Character* character);
				virtual bool isTriggered();
				virtual void update(float dt);
			protected:
				physics::IPhysicalNode* m_actor;
				Environment* m_environment;
			private:
		};
	}
}
#endif // ContactTrigger_h__