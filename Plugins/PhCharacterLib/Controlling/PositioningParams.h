/********************************************************************
	created:	2010/06/28
	created:	28:6:2010   20:23
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\PositioningParams.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	PositioningParams
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef PositioningParams_h__
#define PositioningParams_h__
#include "ControllingParams.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class PositioningParams:public ControllingParams
		{
			public:
				math::vector3d pos;
				math::quaternion orient;
		};
	}
}
#endif // PositioningParams_h__