/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   10:35
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Parental_PD_Params.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Parental_PD_Params
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Parental_PD_Params_h__
#define Parental_PD_Params_h__
#include "ActuatorParams.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class Parental_PD_Params:public ActuatorParams
		{
		public:
			float ks, kd;
		};
	}
}
#endif // Parental_PD_Params_h__