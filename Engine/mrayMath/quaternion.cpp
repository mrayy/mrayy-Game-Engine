
#include "quaternion.h"

namespace mray{
namespace math{

const quaternion quaternion::Identity=quaternion();
	
quaternion::quaternion():w(1),x(0),y(0),z(0)
{}
quaternion::quaternion(float _w,float _x,float _y,float _z):w(_w),x(_x),y(_y),z(_z)
{
}
quaternion::quaternion(float _x,float _y,float _z)
{
	fromEulerAngles(_x,_y,_z);
}
quaternion::quaternion(const vector3d &eulerAngles)
{
	fromEulerAngles(eulerAngles.x,eulerAngles.y,eulerAngles.z);
}
quaternion::quaternion(float angle,const vector3d& axis)
{
	fromAngle(angle,axis);
}
quaternion::quaternion(const quaternion& o):w(o.w),x(o.x),y(o.y),z(o.z)
{}
quaternion::quaternion(const matrix4x4& mat)
{
	fromMatrix(mat);
}

bool quaternion::operator==(const quaternion&o)const{
	return x==o.x&&
		y==o.y&&
		z==o.z&&
		w==o.w;
}
bool quaternion::operator!=(const quaternion&o)const{
	return x!=o.x||
		y!=o.y||
		z!=o.z||
		w!=o.w;
}
quaternion quaternion::operator+(const quaternion&other)const{
	return quaternion(w+other.w,x+other.x,y+other.y,z+other.z);
}
/*
quaternion quaternion::operator+(const vector3d&v)const{
	quaternion q(0,v.x,v.y,v.z);
	q*=*this;
	return *this + q * 0.5;
}*/
/*
quaternion &quaternion::operator+=(const vector3d&v){
	quaternion q(0,v.x,v.y,v.z);
	q*=*this;
	w+=q.w*0.5f;
	x+=q.x*0.5f;
	y+=q.y*0.5f;
	z+=q.z*0.5f;
	Normalize();
	return *this;
}*/
quaternion &quaternion::operator=(const vector3d&v){
	quaternion q(0,v.x,v.y,v.z);
	*this=q;
	Normalize();
	return *this;
}
quaternion quaternion::operator-(const quaternion&other)const{
	return quaternion(w-other.w,x-other.x,y-other.y,z-other.z);
}
quaternion& quaternion::operator+=(const quaternion&other){
	w+=other.w;
	x+=other.x;
	y+=other.y;
	z+=other.z;
	return *this;
}
quaternion quaternion::operator*(const quaternion&other)const{
	return quaternion(
		w*other.w - x*other.x - y*other.y - z*other.z,
		w*other.x + x*other.w + y*other.z - z*other.y,
		w*other.y + y*other.w + z*other.x - x*other.z,
		w*other.z + z*other.w + x*other.y - y*other.x);
}

quaternion quaternion::operator*=(const quaternion&other){
	*this=quaternion(
		w*other.w - x*other.x - y*other.y - z*other.z,
		w*other.x + x*other.w + y*other.z - z*other.y,
		w*other.y + y*other.w + z*other.x - x*other.z,
		w*other.z + z*other.w + x*other.y - y*other.x);
	return *this;
}
quaternion quaternion::operator*(const float f)const{
	return quaternion(f*w,f*x,f*y,f*z);
}
quaternion quaternion::operator-()const{
	return quaternion(-w,-x,-y,-z);
}

vector3d quaternion::operator*(const vector3d &v)const{
	/*
	quaternion p,r,q,inv;
	vector3d res;
	p.x=v.x;
	p.y=v.y;
	p.z=v.z;
	p.w=0;

	q=*this;
	inv.x=-x;
	inv.y=-y;
	inv.z=-z;
	inv.w=w;

	r=q*p*inv;
	res.x=r.x;
	res.y=r.y;
	res.z=r.z;
	return res;
*/
	
	vector3d uv,uuv;
	vector3d qvec(x,y,z);
	uv=qvec.crossProduct(v);
	uuv=qvec.crossProduct(uv);
	uv*=2*w;
	uuv*=2;
	return v+uv+uuv;
}

float quaternion::dotProduct(const quaternion&other)const{
	return w*other.w + x*other.x + y*other.y + z*other.z;
}

float quaternion::LengthSQ(){
	return w*w + x*x + y*y + z*z;
}
float quaternion::Normalize(){
	float len=LengthSQ();
	if(!len){
		w=1;
		return 0;
	}
	float f=1.0f/sqrt(len);
	*this=*this*f;
	return len;
}

quaternion quaternion::inverse()const{
	float n=w*w + x*x + y*y + z*z;
	if(n>0){
		float inv=1.0f/n;
		return quaternion(w*inv,-x*inv,-y*inv,-z*inv);
	}
	return quaternion(0,0,0,0);
}
quaternion quaternion::unitIUnverse()const{
	return quaternion(w,-x,-y,-z);
}

void quaternion::fromAngle(float angle,const vector3d &axis){
	//quat=cos(angle/2) + sin(angle/2)*(ix+jy+kz)
	float hAngle=angle*0.5f;
	float sd=(float)::sin(toRad(hAngle));
	w=(float)::cos(toRad(hAngle));
	x=sd*axis.x;
	y=sd*axis.y;
	z=sd*axis.z;
}

void quaternion::toAngle(float &angle,vector3d &axis){
	float sqrtLen=x*x+y*y+z*z;
	if(sqrtLen>0){
		angle=toDeg(2*acosf(w));
		float invLen=1.0f/sqrtLen;
		axis.x=x*invLen;
		axis.y=y*invLen;
		axis.z=z*invLen;
	}else{
		angle=0;
		axis.set(0,1,0);
	}
}

void quaternion::fromAxis(const vector3d &xAxis,const vector3d &yAxis,const vector3d &zAxis){
	matrix4x4 mat;
	mat[0][0]=xAxis.x;
	mat[0][1]=xAxis.y;
	mat[0][2]=xAxis.z;

	mat[1][0]=yAxis.x;
	mat[1][1]=yAxis.y;
	mat[1][2]=yAxis.z;
	
	mat[2][0]=zAxis.x;
	mat[2][1]=zAxis.y;
	mat[2][2]=zAxis.z;

	fromMatrix(mat);
}
void quaternion::ToAxis(vector3d *axis)const {
	matrix4x4 mat;
	toMatrix(mat);

	for(int i=0;i<3;++i){
		axis[i].x=mat(i,0);
		axis[i].y=mat(i,1);
		axis[i].z=mat(i,2);
	}
}

vector3d quaternion::getXAxis()const{
//	float tx=2*x;
	float ty=2*y;
	float tz=2*z;
	float twy=ty*w;
	float twz=tz*w;
	float txy=ty*x;
	float txz=tz*x;
	float tyy=ty*y;
	float tzz=tz*z;
	return vector3d(1-(tyy+tzz),txy+twz,txz-twy);
}

vector3d quaternion::getYAxis()const{
	float tx=2*x;
	float ty=2*y;
	float tz=2*z;
	float twx=tx*w;
	float twz=tz*w;
	float txx=tx*x;
	float txy=ty*x;
	float tyz=tz*y;
	float tzz=tz*z;
	return vector3d(txy-twz,1-(txx+tzz),tyz+twx);
}

vector3d quaternion::getZAxis()const{
	float tx=2*x;
	float ty=2*y;
	float tz=2*z;
	float twx=tx*w;
	float twy=ty*w;
	float txx=tx*x;
	float txz=tz*x;
	float tyy=ty*y;
	float tyz=tz*y;
	return vector3d(txz+twy,tyz-twx,1-(txx+tyy));
}

void quaternion::fromMatrix(const matrix4x4 &mat){
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	float diag=mat[0][0]+mat[1][1]+mat[2][2];
	float root=0;

	if(diag>0){
		root=sqrt(diag+1);
		w=0.5f*root;
		root=0.5f/root;//1/4w
		x=(mat[2][1]-mat[1][2])*root;
		y=(mat[0][2]-mat[2][0])*root;
		z=(mat[1][0]-mat[0][1])*root;
	}else{

		// |w| <= 1/2
		static int s_next[3] = { 1, 2, 0 };
		int i = 0;
		if ( mat[1][1] > mat[0][0] )
			i = 1;
		if ( mat[2][2] > mat[i][i] )
			i = 2;
		int k = s_next[i];
		int j = s_next[k];

		root = (float)sqrt(mat[i][i]-mat[j][j]-mat[k][k] + 1.0);
		float* apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*root;
		root = 0.5f/root;
		w = (mat[k][j]-mat[j][k])*root;
		*apkQuat[j] = (mat[j][i]+mat[i][j])*root;
		*apkQuat[k] = (mat[k][j]+mat[i][k])*root;
	}
		/*
	if ( mat(0,0) > mat(1,1) && mat(0,0) > mat(2,2))
	{
		// 1st element of diag is greatest value
		// find scale according to 1st element, and double it
		root = sqrtf( 1.0f + mat(0,0) - mat(1,1) - mat(2,2)) * 2.0f;

		x = 0.25f * root;
		y = (mat(0,1) + mat(1,0)) / root;
		z = (mat(2,0) + mat(0,2)) / root;
		w = (mat(2,1) - mat(1,2)) / root;
	}
	else if ( mat(1,1) > mat(2,2))
	{
		// 2nd element of diag is greatest value
		// find scale according to 2nd element, and double it
		root = sqrtf( 1.0f + mat(1,1) - mat(0,0) - mat(2,2)) * 2.0f;

		x = (mat(0,1) + mat(1,0) ) / root;
		y = 0.25f * root;
		z = (mat(1,2) + mat(2,1) ) / root;
		w = (mat(0,2) - mat(2,0) ) / root;
	}
	else
	{
		// 3rd element of diag is greatest value
		// find scale according to 3rd element, and double it
		root  = sqrtf( 1.0f + mat(2,2) - mat(0,0) - mat(1,1)) * 2.0f;

		x = (mat(0,2) + mat(2,0)) / root;
		y = (mat(1,2) + mat(2,1)) / root;
		z = 0.25f * root;
		w = (mat(1,0) - mat(0,1)) / root;
	}
	Normalize();*/
}

void quaternion::toMatrix(matrix4x4&mat)const
{
	
    float fTx  = 2.0f*x;
    float fTy  = 2.0f*y;
    float fTz  = 2.0f*z;
    float fTwx = fTx*w;
    float fTwy = fTy*w;
    float fTwz = fTz*w;
    float fTxx = fTx*x;
    float fTxy = fTy*x;
    float fTxz = fTz*x;
    float fTyy = fTy*y;
    float fTyz = fTz*y;
    float fTzz = fTz*z;

    mat[0][0] = 1.0f-(fTyy+fTzz);
    mat[0][1]= fTxy-fTwz;
    mat[0][2] = fTxz+fTwy;

    mat[1][0] = fTxy+fTwz;
    mat[1][1] = 1.0f-(fTxx+fTzz);
    mat[1][2] = fTyz-fTwx;

    mat[2][0] = fTxz-fTwy;
    mat[2][1] = fTyz+fTwx;
    mat[2][2] = 1.0f-(fTxx+fTyy);

} 

float quaternion::getPitch(bool shortPath)const{
	if(shortPath)
		return (float)(atan2(2*(y*z+w*x),w*w-x*x-y*y + z*z))*GRAD_PI32;
	else
	{
		float tx  = 2.0f*x;
		float tz  = 2.0f*z;
		float twx = tx*w;
		float txx = tx*x;
		float tyz = tz*y;
		float tzz = tz*z;

		return atan2(tyz+twx, 1.0f-(txx+tzz))*GRAD_PI32;
	}
}
float quaternion::getYaw(bool shortPath)const{
	if(shortPath)
		return (float)(asin(2*(w*y-x*z)))*GRAD_PI32;
	else
	{
		float tx  = 2.0f*x;
		float ty  = 2.0f*y;
		float tz  = 2.0f*z;
		float twy = ty*w;
		float txx = tx*x;
		float txz = tz*x;
		float tyy = ty*y;

		return atan2(txz+twy, 1.0f-(txx+tyy))*GRAD_PI32;
	}
}
float quaternion::getRoll(bool shortPath)const{
	if(shortPath)
		return (float)(atan2(2*(x*y+w*z),w*w+x*x-y*y - z*z))*GRAD_PI32;
	else
	{
		float ty  = 2.0f*y;
		float tz  = 2.0f*z;
		float twz = tz*w;
		float txy = ty*x;
		float tyy = ty*y;
		float tzz = tz*z;

		return atan2(txy+twz, 1.0f-(tyy+tzz))*GRAD_PI32;
	}
}


bool quaternion::Equals(const quaternion& q, float epsilon) const
{
	float cs = dotProduct(q);
    float angle = acos(cs);

	return (fabs(angle) <= epsilon)	|| math::fEqual(angle, epsilon);


}

quaternion quaternion::exp () const
{
	// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
	// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

	float fAngle ( (float)sqrt(x*x+y*y+z*z) );
	float fSin = (float)sinr(fAngle);

	quaternion kResult;
	kResult.w = (float)cosr(fAngle);

	if ( abs(fSin) >= Epsilon )
	{
		float fCoeff = fSin/(fAngle);
		kResult.x = fCoeff*x;
		kResult.y = fCoeff*y;
		kResult.z = fCoeff*z;
	}
	else
	{
		kResult.x = x;
		kResult.y = y;
		kResult.z = z;
	}

	return kResult;
}
quaternion quaternion::log () const
{
	// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
	// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

	quaternion kResult;
	kResult.w = 0.0;

	if ( abs(w) < 1.0 )
	{
		double fAngle ( acos(w) );
		double fSin = math::sinr(fAngle);
		if ( abs(fSin) >= math::Epsilon )
		{
			float fCoeff = (float)(fAngle/fSin);
			kResult.x = fCoeff*x;
			kResult.y = fCoeff*y;
			kResult.z = fCoeff*z;
			return kResult;
		}
	}

	kResult.x = x;
	kResult.y = y;
	kResult.z = z;

	return kResult;
}

void quaternion::fromEulerAngles(float pitch,float yaw,float roll){

	/*
	double angle;

	angle = toRad(_z) * 0.5;
	double sr = (double)sin(angle);
	double cr = (double)cos(angle);

	angle = toRad(_x) * 0.5;
	double sp = (double)sin(angle);
	double cp = (double)cos(angle);

	angle = toRad(_y) * 0.5;
	double sy = (double)sin(angle);
	double cy = (double)cos(angle);

	double cpcy = cp * cy;
	double spcy = sp * cy;
	double cpsy = cp * sy;
	double spsy = sp * sy;


	x = (float)(sr * cpcy - cr * spsy);
	y = (float)(cr * spcy + sr * cpsy);
	z = (float)(cr * cpsy - sr * spcy);
	w = (float)(cr * cpcy + sr * spsy);
	*/
	float num = toRad(roll * 0.5f);
	float num2 = (float)sin((double)num);
	float num3 = (float)cos((double)num);
	float num4 = toRad(pitch * 0.5f);
	float num5 = (float)sin((double)num4);
	float num6 = (float)cos((double)num4);
	float num7 = toRad(yaw * 0.5f);
	float num8 = (float)sin((double)num7);
	float num9 = (float)cos((double)num7);
	x = num9 * num5 * num3 + num8 * num6 * num2;
	y = num8 * num6 * num3 - num9 * num5 * num2;
	z = num9 * num6 * num2 - num8 * num5 * num3;
	w = num9 * num6 * num3 + num8 * num5 * num2;

	/*	x = (float)(cr * cpcy + sr * spsy);
	y = (float)(sr * cpcy - spsy * cr);
	z = (float)(cr * spcy + sy *cp*sr);
	w = (float)(sy * cp* cr - cy* sp * sr);*/

	Normalize();
}

void quaternion::toEulerAngles(vector3d &angles)const{
	double sqw = w*w;
	double sqx = x*x;
	double sqy = y*y;
	double sqz = z*z;


	// rotation about x-axis
	
	angles.x = (float) (atan2(2.0 * (y*z +x*w),(-sqx - sqy + sqz + sqw)));

	// rotation about y-axis
	angles.y = (float) (asin(clamp<double>(2.0 * (x*z + y*w),-1,1)));

	// rotation about z-axis
	angles.z = (float) (atan2(2.0 * (x*y +z*w),(sqx - sqy - sqz + sqw)));
	/*
	angles.y = (float)(asin(clamp<double>(2.0 * (x*z - y*w), -1, 1)));

// rotation about y-axis

	angles.x = (float)(atan2(2.0 * (y*z + x*w), 1-2*(sqz + sqw)));

	
	// rotation about z-axis
	angles.z = (float)(atan2(2.0 * (x*y + z*w), 1 - 2 * (sqy + sqz)));*/

	angles.x=(float)toDeg(angles.x);
	angles.y=(float)toDeg(angles.y);
	angles.z=(float)toDeg(angles.z);
}

void quaternion::Slerp(const quaternion& rkP,const quaternion& rkQ,float t,bool shortPath){

	float fCos = rkP.dotProduct(rkQ);
	quaternion rkT;

	// Do we need to invert rotation?
	if (fCos < 0.0f && shortPath)
	{
		fCos = -fCos;
		rkT = -rkQ;
	}
	else
	{
		rkT = rkQ;
	}

	if (abs(fCos) < 1 - math::Epsilon)
	{
		// Standard case (slerp)
		float fSin = sqrt(1 - math::sqr(fCos));
		double fAngle = atan2(fSin, fCos);
		float fInvSin = 1.0f / fSin;
		float fCoeff0 = (float)sin((1.0f - t) * fAngle) * fInvSin;
		float fCoeff1 = (float)sin(t * fAngle) * fInvSin;
		*this= rkP * fCoeff0 + rkT *fCoeff1 ;
	}
	else
	{
		// There are two situations:
		// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
		//    interpolation safely.
		// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
		//    are an infinite number of possibilities interpolation. but we haven't
		//    have method to fix this case, so just use linear interpolation here.
		*this = rkP * (1.0f - y)+ rkT *t;
		// taking the complement requires renormalisation
		Normalize();
	}
	/*
	float angle=q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	
	if(angle<0){
		q2=-q2;
		angle=-angle;
	}
	float scale0,scale1;

	if((angle+1)>0.05f){
		if((1-angle)>=0.05f){
			float theta=(float)acos(angle);
			float invsintheta=1.0f/(float)sin(theta);
			scale0=(float)sin(theta*(1-t))*invsintheta;
			scale1=(float)sin(theta*t)*invsintheta;
		}else{
			scale0=1-t;
			scale1=t;
		}
	}else{
		q2 = quaternion(-q1.y,q1.x,-q1.w,q1.z);
		scale0=(float)sin(PI64*(0.5-t));
		scale1=(float)sin(PI64*t);
	}
	*this=q1*scale0 + q2*scale1;
*/
}
void quaternion::Squad(float t,const quaternion&p,const quaternion&a,const quaternion&b,const quaternion&q,bool useShortPath)
{
	float slerpT=2*t*(1-t);
	quaternion slerpP;
	slerpP.Slerp(p,q,t,useShortPath);
	quaternion slerpQ;
	slerpQ.Slerp(a,b,t);
	
	Slerp(slerpP,slerpQ,slerpT);
}

}
}