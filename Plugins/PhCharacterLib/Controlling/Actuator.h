/********************************************************************
	created:	2010/06/18
	created:	18:6:2010   20:30
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Actuator.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Actuator
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Actuator_h__
#define Actuator_h__



namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
	namespace PhysicsBasedCharacter
	{
		class ActuatorParams;
		class Character;

		class Actuator abstract
		{
			public:
				Actuator(){};
				virtual~Actuator(){};
				virtual ActuatorParams* createParams(xml::XMLElement*elem)=0;
				virtual void setParams(ActuatorParams* params)=0;
				virtual void applyForces(float dt)=0;
				virtual void load(xml::XMLElement*elem, Character* character)=0;
				virtual const core::string& getName()=0;
			protected:
				
			private:
		};
	}
}
#endif // Actuator_h__