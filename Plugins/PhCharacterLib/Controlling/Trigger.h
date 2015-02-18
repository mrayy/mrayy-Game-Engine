/********************************************************************
	created:	2010/05/24
	created:	24:5:2010   2:42
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Trigger.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Trigger
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Trigger_h__
#define Trigger_h__

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{

		class Character;
		class Trigger
		{
		public:			
			Trigger()
			{
				m_state = -1;
			}
			virtual~Trigger(){}
			virtual void load(xml::XMLElement*elem, Character* character)=0;
			virtual bool isTriggered()=0;
			virtual void update(float dt)=0;
			virtual void on()
			{
				m_active = true;
			}
			virtual void off()
			{
				m_active = false;
			}
			virtual bool isActive()
			{
				return m_active;
			}
			virtual int getState()
			{
				return m_state;
			}
		protected:
			int m_state;
			bool m_active;
		private:
		};
	}
}
#endif // Trigger_h__