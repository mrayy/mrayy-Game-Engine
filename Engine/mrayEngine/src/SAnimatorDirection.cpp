#include "stdafx.h"


#include "SAnimatorDirection.h"
#include "ISceneNode.h"

namespace mray{
namespace scene{

SAnimatorDirection::SAnimatorDirection(math::vector3d center,math::vector3d dir,float speed)
:Center(center),Direction(dir),Speed(speed)
{
#ifdef ___DEBUG___
	setDebugName("SAnimatorDirection");
#endif
	Direction.Normalize();
	pos=0;
}

void SAnimatorDirection::animate(IMovable*  node,float dt)
{
	if(node){
		pos+=Direction*Speed*dt;
		node->setPosition(pos+Center);
	}
}

}
}






