#include "stdafx.h"


#include "SAnimatorCircle.h"
#include "ISceneNode.h"

namespace mray{
namespace scene{

SAnimatorCircle::SAnimatorCircle(math::vector3d center,math::vector3d dir,float radius,float speed)
:Center(center),Direction(dir),Radius(radius),Speed(speed),parent(0)
{
	Direction.Normalize();
	pos=math::vector3d(1,0,0);
	angle=0;
}

//from math we know that:
//cos(x+dx)=cos(x)*cos(dx)-sin(x)*sin(dx)
//sin(x+dx)=cos(x)*sin(dx)+sin(x)*cos(dx)
//and:
//pos.x=r*cos(x+dx);
//pos.y=r*sin(x+dx)
//pos.x=oldPos.x*cos(dx)-oldPos.y*sin(dx)
//pos.y=oldPos.x*sin(dx)+oldPos.y*cos(dx)

void SAnimatorCircle::animate(IMovable* node,float dt)
{
	if(node){
		math::vector3d P2=pos;
// 		float st=math::sind(dt*Speed);
// 		float ct=math::cosd(dt*Speed);

		float st=math::sind(angle);
		float ct=math::cosd(angle);

// 		pos.x=P2.x*ct-P2.z*st;
// 		pos.z=P2.x*st+P2.z*ct;

		pos.x=ct;
		pos.z=st;

		P2=pos.crossProduct(Direction);
 		P2=pos;
 		P2*=Radius;
			
		node->setPosition(P2+Center);
		if(node->getParent())
			node->setPosition(node->getPosition()+ node->getParent()->getAbsolutePosition());

		angle+=dt*Speed;
		if(angle>180)angle-=360;
		else if(angle<-180)angle+=360;
	}
}






}
}
