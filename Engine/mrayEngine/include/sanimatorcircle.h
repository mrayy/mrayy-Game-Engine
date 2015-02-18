
#ifndef ___SANIMATOR_CIRCLE___
#define ___SANIMATOR_CIRCLE___

#include "IAnimators.h"

#include "GCPtr.h"


namespace mray{
namespace scene{

class ISceneNode;

class MRAY_DLL SAnimatorCircle:public IAnimators
{
	math::vector3d pos;
	math::vector3d Direction;
	math::vector3d Center;
	float Radius;
	float Speed;
	float angle;
	
	IMovable*  parent;
public:
	SAnimatorCircle(math::vector3d center,math::vector3d dir,float radius,float speed);

	void setParent(IMovable* p){parent=p;}
	virtual void animate(IMovable*  node,float dt);
};

}
}

#endif





