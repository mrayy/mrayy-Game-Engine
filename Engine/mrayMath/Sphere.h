
/********************************************************************
	created:	2008/06/17
	created:	17:6:2008   15:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Sphere.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	Sphere
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Sphere___
#define ___Sphere___

#include "Point3d.h"

namespace mray{
namespace math{

class Sphere
{
private:
protected:
public:

	vector3d center;
	float radius;

	Sphere(){
		center=0;
		radius=0;
	}
	Sphere(float r){
		center=0;
		radius=r;
	}
	Sphere(const vector3d& c,float r){
		center=c;
		radius=r;
	}
	virtual~Sphere(){}

	bool IsPointInside(const vector3d& p)const
	{
		return center.getDistSQ(p)<radius*radius;
	}

	math::vector3d GetClosestPoint(const vector3d&p)const
	{
		math::vector3d diff=p-center;
		if(diff.LengthSQ()<radius*radius)// is it inside the sphere
			return p;
		diff.Normalize();
		return center+diff*radius;
	}

};

}
}


#endif //___Sphere___
