/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   10:34
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Tri_PD_Params.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Tri_PD_Params
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Tri_PD_Params_h__
#define Tri_PD_Params_h__
#include "ActuatorParams.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class Tri_PD_Params:public ActuatorParams
		{
		public:
			float parent_ks, parent_kd;
			float child1_ks, child1_kd;
			float child2_ks, child2_kd;
			int freeChild;
		};
	}
}
#endif // Tri_PD_Params_h__