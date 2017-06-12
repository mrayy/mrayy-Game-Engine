


#ifndef ___PLANES___
#define ___PLANES___

#include "Point3d.h"
#include "mMath.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL Plane
{
public:
	float D;
	vector3d Normal;

	Plane();
	Plane(const vector3d& point,const vector3d& normal);
	Plane(const vector3d& p1,const vector3d& p2,const vector3d& p3);
	Plane(float x,float y,float z,float nx,float ny,float nz);

	void calcPlane(const vector3d& v1,const vector3d& v2,const vector3d& v3);

	void set(const vector3d& normal,float d);
	void set(const vector3d& nVec,const vector3d& p);
	void recalcD(const vector3d& v);

	float getDistance(const vector3d& v)const;

	bool getIntersectionWithLine(const vector3d&linePoint,
		const vector3d&lineVec,vector3d&out)const ;

	bool getIntersectionWithPlane(const Plane&other,vector3d&outLinePoint,
		vector3d&outLineVec)const ;
	bool getIntersectionWithPlanes( const Plane&p1, const Plane&p2,
		vector3d&outPoint)const ;
	//<0 : behind the plane
	//==0: on the plane
	//<1 :infront of the plane
	int getVertLocation(const vector3d &p)const;
	int getVertLocation(const vector3d &p,const vector3d& extent)const;

	vector3d getMemberPoint() const;


	static const Plane XYPlane;
	static const Plane YZPlane;
	static const Plane XZPlane;

};


};//math
};//mray


#endif

