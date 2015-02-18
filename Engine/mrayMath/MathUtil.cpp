
#include "MathUtil.h"



namespace mray
{
namespace math
{



float MathUtil::GaussianDistribution(float x,float offset,float scale)
{
	float nom=exp(-sqr(x-offset)/(2*sqr(scale)));
	float denom=scale*sqr(2*math::PI32);
	return nom/denom;
}



math::matrix4x4 MathUtil::CreateRefelectionMatrix(const math::Plane&p)
{/*
	math::matrix4x4 res,res2;
	float* ptr=res.pointer();

	ptr[0]=-2*p.Normal.x*p.Normal.x +1;
	ptr[1]=-2*p.Normal.x*p.Normal.y ;
	ptr[2]=-2*p.Normal.x*p.Normal.z ;
	ptr[3]=-2*p.Normal.x*p.D ;

	ptr[4]=-2*p.Normal.y*p.Normal.x ;
	ptr[5]=-2*p.Normal.y*p.Normal.y +1;
	ptr[6]=-2*p.Normal.y*p.Normal.z ;
	ptr[7]=-2*p.Normal.y*p.D ;

	ptr[8]=-2*p.Normal.z*p.Normal.x ;
	ptr[9]=-2*p.Normal.z*p.Normal.y ;
	ptr[10]=-2*p.Normal.z*p.Normal.z +1;
	ptr[11]=-2*p.Normal.z*p.D ;

	res.setTranslation(0);

	res.getTransposeMatrix(res2);

	return res2;*/

	return matrix4x4(
		-2*p.Normal.x*p.Normal.x +1	,-2*p.Normal.x*p.Normal.y	,-2*p.Normal.x*p.Normal.z	,-2*p.Normal.x*p.D,
		-2*p.Normal.y*p.Normal.x 	,-2*p.Normal.y*p.Normal.y+1	,-2*p.Normal.y*p.Normal.z	,-2*p.Normal.y*p.D,
		-2*p.Normal.z*p.Normal.x 	,-2*p.Normal.z*p.Normal.y	,-2*p.Normal.z*p.Normal.z+1	,-2*p.Normal.z*p.D,
		0							,0							,0							,1
		);
}


matrix4x4 MathUtil::CreateProjectionMatrixPerspectiveFov(float fov,float aspect,float znear,float zfar)
{
	float left, right, top, bottom;

	float tThetaY=tanf(fov * 0.5f);
	float tThetaX=tThetaY* aspect;

	left = -znear * tThetaX;
	right = -left;

	bottom = -znear * tThetaY;
	top = -bottom;


	return CreateProjectionMatrixPerspective(left,right,bottom,top,znear,zfar);
}
#if 1
matrix4x4 MathUtil::CreateProjectionMatrixOrtho(float left,float right,float bottom,float top,float znear,float zfar)
{
	matrix4x4 m;
	float invW=1.0f/(right-left);
	float invH=1.0f/(bottom-top);
	float invZ=1.0f/(zfar-znear);

	/*
	m.f11= 2.0f*invW;
		m.f14= -(right+left)*invW;
	
	
		m.f22= 2.0f*invH;
		m.f24= -(bottom+top)*invH;
	
		m.f33= -2*invZ;
		m.f34= - (zfar+znear)*invZ;
	
		m.f44=  1;*/
	m.f11= invW;
	m.f22= invH;
	m.f33= 2*invZ;
	m.f34= - (zfar+znear)*invZ;

	m.f44=  1;

	m.flagNotIdentity();
	return m;
}
#elif 0

matrix4x4 m;
float invW=1.0f/(right-left);
float invH = 1.0f / (top - bottom);
float invZ=1.0f/(zfar-znear);

/*
m.f11= 2.0f*invW;
m.f14= -(right+left)*invW;


m.f22= 2.0f*invH;
m.f24= -(bottom+top)*invH;

m.f33= -2*invZ;
m.f34= - (zfar+znear)*invZ;

m.f44=  1;*/
m.f11 = 2 * invW;
m.f14 = -(right+left) * invW;
m.f22 = 2 * invH;
m.f24 = -(top + bottom)* invH;
m.f33 = -2 * invZ;
m.f34= - (zfar+znear)*invZ;

m.f44=  1;
#else

matrix4x4 MathUtil::CreateProjectionMatrixOrtho(float left, float right, float bottom, float top, float znear, float zfar)
{
	matrix4x4 m;
	float A = 2 * znear / (right - left);
	float B = 2 * znear / ( top- bottom);
	float C = (right+left) / (right - left);
	float D = (top+bottom) / (top-bottom);
	float q = -(zfar+znear) / (zfar - znear);
	float qn = -2*(zfar*znear) / (zfar - znear);

	m.f11 = A;
	m.f13 = C;
	m.f22 = B;
	m.f23 = D;
	m.f33 = q;
	m.f34 = qn;

	m.f44 = 0;
	m.f43 = -1;

	m.flagNotIdentity();
	return m;
}
#endif
matrix4x4 MathUtil::CreateProjectionMatrixPerspective(float left,float right,float bottom,float top,float znear,float zfar)
{
	matrix4x4 m;
	float invW=1.0f/(right-left);
	float invH=1.0f/(top-bottom);
	float invZ=1.0f/(zfar-znear);

	m.f11= 2.0f*znear*invW;
	m.f13= (right+left)*invW;


	m.f22= 2.0f*znear*invH;
	m.f23= (bottom+top)*invH;

	m.f33= -(zfar+znear)*invZ;
	m.f34= -2* znear * zfar*invZ;

	m.f43= -1.0f;
	m.f44=  0.0f;

	m.flagNotIdentity();
	return m;
}

matrix4x4 MathUtil::CreateLookAtMatrix(const vector3d&pos,const quaternion&ori)
{
	matrix4x4 res;
	vector3d trans;
	ori.toMatrix(res);
	res=res.getTransposed();
	res.setTranslation(-(res*pos));
/*
	quaternion q=ori.inverse();
	ori.toMatrix(res);
	res.setTranslation(q*pos);*/
	return res;
}

matrix4x4 MathUtil::CreateLookAtMatrix(const vector3d& pos,const vector3d& target,const vector3d& up)
{
	vector3d zAxis=pos-target;
	zAxis.Normalize();

	vector3d xAxis=up.crossProduct(zAxis);
	xAxis.Normalize();

	vector3d yAxis=zAxis.crossProduct(xAxis);

	return matrix4x4(
		xAxis.x,xAxis.y,xAxis.z,-xAxis.dotProduct(pos),
		yAxis.x,yAxis.y,yAxis.z,-yAxis.dotProduct(pos),
		zAxis.x,zAxis.y,zAxis.z,-zAxis.dotProduct(pos),
		0,0,0,1

	);
}

matrix4x4 MathUtil::CreateSkewSymmetric(const vector3d &v)
{
	return matrix4x4(
		0	,-v.z, v.y	,0	,
		v.z	,0	 , -v.x	,0	,
		-v.y,v.x ,  0	,0	,
		0,0,0,0);

}

}
}

