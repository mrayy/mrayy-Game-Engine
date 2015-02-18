

#ifndef ___SANIMATOR_DIRECTION___
#define ___SANIMATOR_DIRECTION___

#include "IAnimators.h"



namespace mray{
namespace scene{
class ISceneNode;

class MRAY_DLL SAnimatorDirection:public IAnimators
{
	math::vector3d pos;
	math::vector3d Direction;
	math::vector3d Center;
	float Speed;
	
public:
	SAnimatorDirection(math::vector3d center,math::vector3d dir,float speed);

	virtual void animate(IMovable*  node,float dt);
};
}
}


#endif








