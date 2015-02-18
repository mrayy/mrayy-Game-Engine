/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   9:51
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\GaitParams.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	GaitParams
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef GaitParams_h__
#define GaitParams_h__
#include "ControllingParams.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class GaitParams:public ControllingParams
		{
			public:
				float facingDirection, turningSpeed;
		};
	}
}
#endif // GaitParams_h__