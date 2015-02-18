#include "stdafx.h"


#include "SAnimatorRotate.h"
#include "ISceneNode.h"

namespace mray{
namespace scene{
SAnimatorRotate::SAnimatorRotate(math::vector3d axis,float speed)
:Axis(axis),Speed(speed)
{
	Axis.Normalize();
#ifdef ___DEBUG___
	setDebugName("SAnimatorRotate");
#endif
}

void SAnimatorRotate::animate(IMovable* node,float dt)
{
	if(node){
		node->rotate(Speed*dt,Axis,TS_Local);
	}
}


}
}

