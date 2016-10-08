
/********************************************************************
	created:	2008/06/17
	created:	17:6:2008   15:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Intersection.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	Intersection
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Intersection___
#define ___Intersection___

#include "Ray3d.h"
#include "Plane.h"
#include "box3d.h"
#include "Sphere.h"

namespace mray{
namespace math{

enum EInterResult
{
	EIR_None,
	EIR_Contained,
	EIR_Contains,
	EIR_Partial
};
class InterRayResult
{
public:
	bool hit;
	float distance;

	InterRayResult(bool h,float d=0){
		hit=h;
		distance=d;
	}
	vector3d getHitLocation(const Ray3d&r){
		return r.Start + r.Dir*distance;
	}
};


class Intersection
{
private:
protected:
public:

	static EInterResult intersect(const box3d&b1,const box3d&b2);
	static bool intersect(const Sphere&s,const box3d&b);

	static InterRayResult intersect(const Ray3d&s,const box3d&b);
	static bool intersect(const Plane&s,const box3d&b);

	static InterRayResult intersect(const Ray3d&r,const Sphere&s);
	static bool intersect(const Plane&p,const Sphere&s);

	static InterRayResult intersect(const Ray3d&r,const Plane&p);

};

}
}


#endif //___Intersection___
