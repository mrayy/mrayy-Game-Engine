


#ifndef ___MATRIX___
#define ___MATRIX___

#include "mMath.h"
#include "box3d.h"
#include "point3d.h"
#include "point4d.h"
#include "Plane.h"


namespace mray{
namespace math{


	/*
	* matrix uses column vectors when applying matrix multiplications
	*/

class MRAY_MATH_DLL matrix4x4
{
	
	bool isIdentityMatrix;
public:
	union
	{
		struct
		{
			float f11, f12, f13, f14;
			float f21, f22, f23, f24;
			float f31, f32, f33, f34;
			float f41, f42, f43, f44;
		};
		float m4x4[4][4];
		float mat[16];
	};

	matrix4x4(const matrix4x4& other);
	matrix4x4(	float a11,float a12,float a13,float a14,
				float a21,float a22,float a23,float a24,
				float a31,float a32,float a33,float a34,
				float a41,float a42,float a43,float a44);
	matrix4x4();

	float getElement(int r,int c)const;
	void setElement(int r,int c,float val);
	void loadMatrix(float *m);
	
	void flagNotIdentity()
	{
		isIdentityMatrix=false;
	}

	float* getMatPointer();
	const float* getMatPointer()const;

	void makeIdentity();
	bool checkIdentity();
	bool isIdentity()const;

	bool getInverse(matrix4x4& out)const;

	void multWithVec4(float*vec)const;

	float& operator () (int r,int c);
	float operator () (int r,int c)const;

	float& operator () (int e);
	float operator () (int e)const;
	matrix4x4& operator=(const matrix4x4&other) ;
	matrix4x4  operator +(const matrix4x4&other)const ;
	matrix4x4  operator -(const matrix4x4&other)const ;
	matrix4x4  operator *(const matrix4x4&other)const ;
	matrix4x4 operator *(const float val)const;
	math::vector3d operator *(const math::vector3d&v)const ;
	math::vector4d operator *(const math::vector4d&v)const ;
	bool operator==(const matrix4x4&other) const;

	float* operator [](int r);
	const float* operator [](int r)const;

	inline void getTransposeMatrix(matrix4x4 &out)const 
	{
		out.mat[0]=mat[0];
		out.mat[1]=mat[4];
		out.mat[2]=mat[8];
		out.mat[3]=mat[12];

		out.mat[4]=mat[1];
		out.mat[5]=mat[5];
		out.mat[6]=mat[9];
		out.mat[7]=mat[13];

		out.mat[8]=mat[2];
		out.mat[9]=mat[6];
		out.mat[10]=mat[10];
		out.mat[11]=mat[14];

		out.mat[12]=mat[3];
		out.mat[13]=mat[7];
		out.mat[14]=mat[11];
		out.mat[15]=mat[15];

		out.isIdentityMatrix=isIdentityMatrix;
	}
	math::matrix4x4 getTransposed()const;

	inline vector3d getAxisVectorT(int i)const {
		if(i>=4)
			return vector3d::Zero;
		return vector3d(m4x4[0][i],m4x4[1][i],m4x4[2][i]);
	}
	inline vector3d getAxisVector(int i)const {
		if(i>=4)
			return vector3d::Zero;
		return vector3d(m4x4[i][0],m4x4[i][1],m4x4[i][2]);
	}

	vector3d inverseTransformVector(const vector3d& vec)const;
	vector3d transformNormal(const vector3d &vec)const ;

	//! Transforms a box by this matrix
	void transformBox(box3d &box)const;
	//! Transforms a plane by this matrix
	void transformPlane( Plane &plane) const;

	float calcDim()const ;

	matrix4x4 & rotateX(float angle);
	matrix4x4 & rotateY(float angle);
	matrix4x4 & rotateZ(float angle);

	vector3d getRotation()const ;

	matrix4x4& setTranslation(const vector3d& trans);
	matrix4x4& setScale(const vector3d& scale);
	vector3d getTranslation()const ;

	matrix4x4& setRotationRadians(const vector3d& angles);
	matrix4x4& setRotationDegrees(const vector3d& angles);
 
	vector3d getScale()const ;


	void setComponent(const vector3d& a,const vector3d& b,const vector3d& c);


	/*
	
	void setSkewSymmetric(const Point3d &v){
		memset(mat,0,sizeof(float)*16);
		mat[15]=0;
		mat[1]=v.z;
		mat[2]=v.y;
		mat[4]=-v.z;
		mat[6]=v.x;
		mat[8]=v.y;
		mat[9]=-v.x;
	}
*/

	float* pointer();

	static const matrix4x4 Identity;
};


inline vector4d operator*(const vector4d& v, const matrix4x4& mat)
{
	return vector4d(
		v.x*mat.f11+v.y*mat.f21+v.z*mat.f31+v.w*mat.f41,
		v.x*mat.f12+v.y*mat.f22+v.z*mat.f32+v.w*mat.f42,
		v.x*mat.f13+v.y*mat.f23+v.z*mat.f33+v.w*mat.f43,
		v.x*mat.f14+v.y*mat.f24+v.z*mat.f34+v.w*mat.f44
		);
}


};//math
};//mray


#endif






