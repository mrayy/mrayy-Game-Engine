/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   2:58
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Parental_PD_Actuator.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Parental_PD_Actuator
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Parental_PD_Actuator_h__
#define Parental_PD_Actuator_h__
#include "Actuator.h"
#include "Parental_PD_Params.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class Segment;
		class Parental_PD_Actuator:public Actuator
		{
		public:
			Parental_PD_Actuator();
			virtual~Parental_PD_Actuator();
			void load(xml::XMLElement*elem, Character* character);
			virtual ActuatorParams* createParams(xml::XMLElement*elem);
			virtual void setParams(ActuatorParams* params);
			virtual void applyForces(float dt);
			virtual const core::string& getName();
		protected:
			Segment* m_segment;
			Parental_PD_Params* m_params;
			core::string m_name;
		};
	}
}
#endif // Parental_PD_Actuator_h__