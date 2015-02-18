#include "stdafx.h"


#include "SAnimatorSpline.h"
#include "ISceneNode.h"


namespace mray{
namespace scene{

SAnimatorSpline::SAnimatorSpline(std::vector<math::vector3d> &p,float speed,float tightness):Speed(speed),Tightness(tightness),
points(p),currTim(0)
{
#ifdef ___DEBUG___
	setDebugName("SAnimatorDirection");
#endif
}


void SAnimatorSpline::animate(IMovable* node,float dt)
{
	if(node){
		math::vector3d p0,p1,p2,p3;
		currTim+=dt;
		float cT=currTim*Speed;
		float u=cT-(int)cT;
		int index=((int)cT)% points.size();

		p0=points[math::cycle<float>(index-1,0,points.size())];
		p1=points[index];
		p2=points[math::cycle<float>(index+1,0,points.size())];
		p3=points[math::cycle<float>(index+2,0,points.size())];

		float h1=2*u*u*u -3*u*u +1; 
		float h2=-2*u*u*u +3*u*u ;
		float h3=u*u*u -2*u*u +u;
		float h4=u*u*u -u*u ;

		//Tangents
		math::vector3d t1=(p2-p0)*Tightness;
		math::vector3d t2=(p3-p0)*Tightness;

		node->setPosition(p1*h1 + p2*h2 + t1*h3 + t2*h4);
	}
}








}
}
