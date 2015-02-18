

#ifndef ___SAnimatorRotate___
#define ___SAnimatorRotate___


#include "IAnimators.h"


namespace mray{
namespace scene{

class ISceneNode;

class MRAY_DLL SAnimatorRotate:public IAnimators
{
	math::vector3d Axis;
	float Speed;
	
public:
	SAnimatorRotate(math::vector3d axis,float speed);

	virtual void animate(IMovable* node,float dt);
};


}
}


#endif





