

#ifndef ___QUATERNION___
#define ___QUATERNION___

#include "compileConfig.h"
#include "Point3d.h"
//#include "mTypes.h"
#include "mMath.h"
#include "matrix4x4.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL quaternion
{
public:
	quaternion();
	quaternion(float _w,float _x,float _y,float _z);
	quaternion(float _x,float _y,float _z);
	quaternion(const vector3d &eulerAngles);
	quaternion(float angle,const vector3d& axis);
	quaternion(const quaternion& o);
	quaternion(const matrix4x4& mat);

	quaternion operator+(const quaternion&other)const;
	
//	quaternion operator+(const vector3d&v)const;
	
//	quaternion &operator+=(const vector3d&v);

	bool operator==(const quaternion&o)const;
	bool operator!=(const quaternion&o)const;

	quaternion &operator=(const vector3d&v);
	quaternion operator-(const quaternion&other)const;
	quaternion& operator+=(const quaternion&other);
	quaternion operator*(const quaternion&other)const;
	
	quaternion operator*=(const quaternion&other);

	quaternion operator*(const float f)const;
	quaternion operator-()const;

	vector3d operator*(const vector3d &v)const;

	float dotProduct(const quaternion&other)const;

	float LengthSQ();
	float Normalize();
	quaternion inverse()const;
	quaternion unitIUnverse()const;

	void fromAngle(float angle,const vector3d &axis);

	void toAngle(float &angle,vector3d &axis);


	quaternion log()const;

	quaternion exp()const;

	void fromAxis(const vector3d &xAxis,const vector3d &yAxis,const vector3d &zAxis);
	void ToAxis(vector3d *axis)const ;

	vector3d getXAxis()const;

	vector3d getYAxis()const;

	vector3d getZAxis()const;

	void fromMatrix(const matrix4x4 &mat);

	void toMatrix(matrix4x4&mat)const;

	float getPitch(bool shortPath=false)const;
	float getYaw(bool shortPath=false)const;
	float getRoll(bool shortPath=false)const;

	bool Equals(const quaternion& q, float epsilon=math::Epsilon) const;

	void fromEulerAngles(float _x,float _y,float _z);

	void toEulerAngles(vector3d &angles)const;

	void Slerp(const quaternion &q1,const quaternion &q2,float t,bool shortPath=0);
	
	void Squad(float t,const quaternion&p,const quaternion&a,const quaternion&b,const quaternion&q,bool ueShortPath=0);



	float w,x,y,z;


	static const quaternion Identity;
};


};//math
};//mray



#endif











