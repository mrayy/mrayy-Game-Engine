

#ifndef ___matrix3x3___
#define ___matrix3x3___

#include "mMath.h"
#include "Point2d.h"

namespace mray{
namespace math{


class MRAY_MATH_DLL matrix3x3
{
	bool m_isIdentity;
public:

	union
	{
		struct
		{
			float f11, f12, f13; 
			float f21, f22, f23; 
			float f31, f32, f33; 
		};
		float mat3x3[3][3];
		float mat[9];
	};

	matrix3x3();
	matrix3x3(	float e11,float e12,float e13,
				float e21,float e22,float e23,
				float e31,float e32,float e33);

	float& operator () (int r,int c);
	float operator () (int r,int c)const;

	float* operator[](int row);
	const float* operator[](int row)const;

	matrix3x3 operator+(const matrix3x3&o)const;
	matrix3x3 operator-(const matrix3x3&o)const;
	matrix3x3 operator*(const matrix3x3&o)const;

	matrix3x3 operator-()const;
	
	matrix3x3 operator*(float v)const;
	
	vector2d operator*(const vector2d& v)const;

	void makeIdentity();

	bool isIdentity()const;
	bool calcIsIdentity();

	float*pointer();

	void setAngle(double angle);
	double getAngle()const;

	void translate(const vector2d& v);
	vector2d getTranslate()const;

	static const matrix3x3 Identity;
};

}
}






#endif





