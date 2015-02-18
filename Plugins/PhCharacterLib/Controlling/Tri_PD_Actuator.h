/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   3:20
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Tri_PD_Actuator.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Tri_PD_Actuator
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Tri_PD_Actuator_h__
#define Tri_PD_Actuator_h__
#include "Actuator.h"
#include "Tri_PD_Params.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class Tri_PD_Params;
		class Segment;
		class Tri_PD_Actuator: public Actuator
		{
			public:
				Tri_PD_Actuator();
				virtual~Tri_PD_Actuator();
				void load(xml::XMLElement*elem, Character* character);
				virtual ActuatorParams* createParams(xml::XMLElement*elem);
				virtual void setParams(ActuatorParams* params);
				virtual void applyForces(float dt);
				virtual const core::string& getName();
			protected:
				Segment* m_parent;
				Segment* m_child1;
				Segment* m_child2;
				Tri_PD_Params* m_params;
				core::string m_name;
			private:
		};
	}
}
#endif // Tri_PD_Actuator_h__