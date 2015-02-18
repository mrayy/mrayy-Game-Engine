

#ifndef ___triangle3d___
#define ___triangle3d___

#include "Point3d.h"

#include "box3d.h"
#include "Plane.h"
#include "line3d.h"
#include "compileConfig.h"

namespace mray
{
namespace math
{

class MRAY_MATH_DLL triangle3d
{
public:
	vector3d pA,pB,pC;
	triangle3d();
	triangle3d(const vector3d& a,const vector3d& b,const vector3d& c);

	bool operator==(const triangle3d &t)const;
	bool operator!=(const triangle3d &t)const;

	void set(const vector3d& a,const vector3d& b,const vector3d& c);

	bool isInsideBox(const box3d&b)const;
	bool isPointInside(const vector3d&v)const;
	bool isPointInsideFast(const vector3d&v)const;
	bool isIntersectWithLimitedLine(const line3d &ln,vector3d &out)const;
	bool isIntersectWithLine(const vector3d &start,const vector3d &dir,
		vector3d &out)const;
	bool getIntersectPlaneWithLine(const vector3d &start,const vector3d &dir,
		vector3d &out)const;
	
	bool isOnSameSide(const vector3d &p,const vector3d &a,const vector3d &b,
		const vector3d &c)const;

	vector3d closestPointOnTriangle(const vector3d&v)const;

	vector3d getNormal()const;

	bool isFrontFacing(const vector3d& dir)const;
	Plane getPlane()const;

	float area();

};


};//math
};//mray


#endif












