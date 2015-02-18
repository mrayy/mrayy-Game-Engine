
#include "matrix4x4.h"

#include <memory.h>


namespace mray{
namespace math{

	const matrix4x4 matrix4x4::Identity;

matrix4x4::matrix4x4(const matrix4x4& other):isIdentityMatrix(0)//,DimVisArray(0)
{
	*this = other;
}

matrix4x4::matrix4x4(	float a11,float a12,float a13,float a14,
		  float a21,float a22,float a23,float a24,
		  float a31,float a32,float a33,float a34,
		  float a41,float a42,float a43,float a44):
		f11(a11),f12(a12),f13(a13),f14(a14),
		f21(a21),f22(a22),f23(a23),f24(a24),
		f31(a31),f32(a32),f33(a33),f34(a34),
		f41(a41),f42(a42),f43(a43),f44(a44),
		isIdentityMatrix(0)
{
}

matrix4x4::matrix4x4():isIdentityMatrix(0)//,DimVisArray(0)
{
	makeIdentity();
//	createDimArrays();
}

float matrix4x4::getElement(int r,int c)const
{
	if(r>=4 || c>=4)
		return 0;
	return mat[r*4+c];
}
void matrix4x4::setElement(int r,int c,float val){
	if(r>=4 || c>=4)
		return;
	mat[r*4+c]=val;

	isIdentityMatrix=0;
}
void matrix4x4::loadMatrix(float *m)
{
	memcpy(mat,m,16*sizeof(float));
	isIdentityMatrix=0;
}


float* matrix4x4::getMatPointer(){return mat;}
const float* matrix4x4::getMatPointer()const{return mat;}

void matrix4x4::makeIdentity()
{
	//if(isIdentityMatrix)return;
	memset(mat,0,4*4*sizeof(float));
	for(int i=0;i<4;++i)
			mat[i*4+i]=1;
	//isIdentityMatrix=1;
}
bool matrix4x4::checkIdentity()
{
	//if(isIdentityMatrix)return 1;
	for(int i=0;i<4;++i)
	for(int j=0;j<4;j++)
		if(i!=j)
			if(abs(mat[i*4+j])>math::Epsilon)return 0;
		else if(abs(mat[i*4+j])>1+math::Epsilon|| abs(mat[i*4+j])<1-math::Epsilon)
			return 0;
	//isIdentityMatrix=1;
	return 1;
}
bool matrix4x4::isIdentity()const
{
	return isIdentityMatrix;
}


math::matrix4x4 matrix4x4::getTransposed()const
{
	math::matrix4x4 m;
	getTransposeMatrix(m);
	return m;
}


bool matrix4x4::getInverse(matrix4x4& out)const
{
	const matrix4x4 &m = *this;

	float d = calcDim();
	if (d == 0.0f)
		return false;

	d = 1.0f / d;

	out.mat[0] = d * (m.mat[5] * (m.mat[10] * m.mat[15] - m.mat[11] * m.mat[14]) + m.mat[6] * (m.mat[11] * m.mat[13] - m.mat[9] * m.mat[15]) + m.mat[7] * (m.mat[9] * m.mat[14] - m.mat[10] * m.mat[13]));
	out.mat[1] = d * (m.mat[9] * (m.mat[2] * m.mat[15] - m.mat[3] * m.mat[14]) + m.mat[10] * (m.mat[3] * m.mat[13] - m.mat[1] * m.mat[15]) + m.mat[11] * (m.mat[1] * m.mat[14] - m.mat[2] * m.mat[13]));
	out.mat[2] = d * (m.mat[13] * (m.mat[2] * m.mat[7] - m.mat[3] * m.mat[6]) + m.mat[14] * (m.mat[3] * m.mat[5] - m.mat[1] * m.mat[7]) + m.mat[15] * (m.mat[1] * m.mat[6] - m.mat[2] * m.mat[5]));
	out.mat[3] = d * (m.mat[1] * (m.mat[7] * m.mat[10] - m.mat[6] * m.mat[11]) + m.mat[2] * (m.mat[5] * m.mat[11] - m.mat[7] * m.mat[9]) + m.mat[3] * (m.mat[6] * m.mat[9] - m.mat[5] * m.mat[10]));
	out.mat[4] = d * (m.mat[6] * (m.mat[8] * m.mat[15] - m.mat[11] * m.mat[12]) + m.mat[7] * (m.mat[10] * m.mat[12] - m.mat[8] * m.mat[14]) + m.mat[4] * (m.mat[11] * m.mat[14] - m.mat[10] * m.mat[15]));
	out.mat[5] = d * (m.mat[10] * (m.mat[0] * m.mat[15] - m.mat[3] * m.mat[12]) + m.mat[11] * (m.mat[2] * m.mat[12] - m.mat[0] * m.mat[14]) + m.mat[8] * (m.mat[3] * m.mat[14] - m.mat[2] * m.mat[15]));
	out.mat[6] = d * (m.mat[14] * (m.mat[0] * m.mat[7] - m.mat[3] * m.mat[4]) + m.mat[15] * (m.mat[2] * m.mat[4] - m.mat[0] * m.mat[6]) + m.mat[12] * (m.mat[3] * m.mat[6] - m.mat[2] * m.mat[7]));
	out.mat[7] = d * (m.mat[2] * (m.mat[7] * m.mat[8] - m.mat[4] * m.mat[11]) + m.mat[3] * (m.mat[4] * m.mat[10] - m.mat[6] * m.mat[8]) + m.mat[0] * (m.mat[6] * m.mat[11] - m.mat[7] * m.mat[10]));
	out.mat[8] = d * (m.mat[7] * (m.mat[8] * m.mat[13] - m.mat[9] * m.mat[12]) + m.mat[4] * (m.mat[9] * m.mat[15] - m.mat[11] * m.mat[13]) + m.mat[5] * (m.mat[11] * m.mat[12] - m.mat[8] * m.mat[15]));
	out.mat[9] = d * (m.mat[11] * (m.mat[0] * m.mat[13] - m.mat[1] * m.mat[12]) + m.mat[8] * (m.mat[1] * m.mat[15] - m.mat[3] * m.mat[13]) + m.mat[9] * (m.mat[3] * m.mat[12] - m.mat[0] * m.mat[15]));
	out.mat[10] = d * (m.mat[15] * (m.mat[0] * m.mat[5] - m.mat[1] * m.mat[4]) + m.mat[12] * (m.mat[1] * m.mat[7] - m.mat[3] * m.mat[5]) + m.mat[13] * (m.mat[3] * m.mat[4] - m.mat[0] * m.mat[7]));
	out.mat[11] = d * (m.mat[3] * (m.mat[5] * m.mat[8] - m.mat[4] * m.mat[9]) + m.mat[0] * (m.mat[7] * m.mat[9] - m.mat[5] * m.mat[11]) + m.mat[1] * (m.mat[4] * m.mat[11] - m.mat[7] * m.mat[8]));
	out.mat[12] = d * (m.mat[4] * (m.mat[10] * m.mat[13] - m.mat[9] * m.mat[14]) + m.mat[5] * (m.mat[8] * m.mat[14] - m.mat[10] * m.mat[12]) + m.mat[6] * (m.mat[9] * m.mat[12] - m.mat[8] * m.mat[13]));
	out.mat[13] = d * (m.mat[8] * (m.mat[2] * m.mat[13] - m.mat[1] * m.mat[14]) + m.mat[9] * (m.mat[0] * m.mat[14] - m.mat[2] * m.mat[12]) + m.mat[10] * (m.mat[1] * m.mat[12] - m.mat[0] * m.mat[13]));
	out.mat[14] = d * (m.mat[12] * (m.mat[2] * m.mat[5] - m.mat[1] * m.mat[6]) + m.mat[13] * (m.mat[0] * m.mat[6] - m.mat[2] * m.mat[4]) + m.mat[14] * (m.mat[1] * m.mat[4] - m.mat[0] * m.mat[5]));
	out.mat[15] = d * (m.mat[0] * (m.mat[5] * m.mat[10] - m.mat[6] * m.mat[9]) + m.mat[1] * (m.mat[6] * m.mat[8] - m.mat[4] * m.mat[10]) + m.mat[2] * (m.mat[4] * m.mat[9] - m.mat[5] * m.mat[8]));

	out.isIdentityMatrix=0;

	return true;
}

math::vector3d matrix4x4::operator *(const math::vector3d&v)const
{
	return math::vector3d(
		f11*v.x+f12*v.y+f13*v.z+f14,
		f21*v.x+f22*v.y+f23*v.z+f24,
		f31*v.x+f32*v.y+f33*v.z+f34
		);
}
math::vector4d matrix4x4::operator *(const math::vector4d&v)const 
{
	return math::vector4d(
		f11*v.x+f12*v.y+f13*v.z+f14*v.w,
		f21*v.x+f22*v.y+f23*v.z+f24*v.w,
		f31*v.x+f32*v.y+f33*v.z+f34*v.w,
		f41*v.x+f42*v.y+f43*v.z+f44*v.w
		);
}
void matrix4x4::multWithVec4(float*vec)const
{
	float vmat[4];
	vmat[0]=vec[0];
	vmat[1]=vec[1];
	vmat[2]=vec[2];
	vmat[3]=vec[3];
	vec[0]=f11*vmat[0]+f12*vmat[1]+f13*vmat[2]+f14*vmat[3];
	vec[1]=f21*vmat[0]+f22*vmat[1]+f23*vmat[2]+f24*vmat[3];
	vec[2]=f31*vmat[0]+f32*vmat[1]+f33*vmat[2]+f34*vmat[3];
	vec[3]=f41*vmat[0]+f42*vmat[1]+f43*vmat[2]+f44*vmat[3];
}

float& matrix4x4::operator () (int r,int c){
	isIdentityMatrix=0;
	return mat[r*4+c];
}
float matrix4x4::operator () (int r,int c)const{
	return mat[r*4+c];
}

float& matrix4x4::operator () (int e){
	isIdentityMatrix=0;
	return mat[e];
}
float matrix4x4::operator () (int e)const{
	return mat[e];
}



float* matrix4x4::operator [](int r)
{
	if(r>=4)return 0;
	return m4x4[r];
}
const float* matrix4x4::operator [](int r)const
{
	if(r>=4)return 0;
	return m4x4[r];
}

matrix4x4& matrix4x4::operator=(const matrix4x4&other) 
{
	memcpy(mat,other.mat,4*4*sizeof(float));
	isIdentityMatrix=other.isIdentityMatrix;
	return *this;
}
matrix4x4  matrix4x4::operator +(const matrix4x4&o)const 
{
	matrix4x4 out(
		f11+o.f11,f12+o.f12, f13+o.f13,f14+o.f14,
		f21+o.f21,f22+o.f22, f23+o.f23,f24+o.f24,
		f31+o.f31,f32+o.f32, f33+o.f33,f34+o.f34,
		f41+o.f41,f42+o.f42, f43+o.f43,f44+o.f44);

	out.isIdentityMatrix=isIdentityMatrix || o.isIdentityMatrix;
	return out;
}
matrix4x4  matrix4x4::operator -(const matrix4x4&o)const 
{
	matrix4x4 out(
		f11-o.f11,f12-o.f12, f13-o.f13,f14-o.f14,
		f21-o.f21,f22-o.f22, f23-o.f23,f24-o.f24,
		f31-o.f31,f32-o.f32, f33-o.f33,f34-o.f34,
		f41-o.f41,f42-o.f42, f43-o.f43,f44-o.f44);

	out.isIdentityMatrix=isIdentityMatrix || o.isIdentityMatrix;
	return out;
}


matrix4x4  matrix4x4::operator *(const matrix4x4&o)const 
{
	matrix4x4 out(
		f11*o.f11+f12*o.f21+f13*o.f31+f14*o.f41,
		f11*o.f12+f12*o.f22+f13*o.f32+f14*o.f42,
		f11*o.f13+f12*o.f23+f13*o.f33+f14*o.f43,
		f11*o.f14+f12*o.f24+f13*o.f34+f14*o.f44,

		f21*o.f11+f22*o.f21+f23*o.f31+f24*o.f41,
		f21*o.f12+f22*o.f22+f23*o.f32+f24*o.f42,
		f21*o.f13+f22*o.f23+f23*o.f33+f24*o.f43,
		f21*o.f14+f22*o.f24+f23*o.f34+f24*o.f44,

		f31*o.f11+f32*o.f21+f33*o.f31+f34*o.f41,
		f31*o.f12+f32*o.f22+f33*o.f32+f34*o.f42,
		f31*o.f13+f32*o.f23+f33*o.f33+f34*o.f43,
		f31*o.f14+f32*o.f24+f33*o.f34+f34*o.f44,

		f41*o.f11+f42*o.f21+f43*o.f31+f44*o.f41,
		f41*o.f12+f42*o.f22+f43*o.f32+f44*o.f42,
		f41*o.f13+f42*o.f23+f43*o.f33+f44*o.f43,
		f41*o.f14+f42*o.f24+f43*o.f34+f44*o.f44);	
	return out;
}
matrix4x4  matrix4x4::operator *(float o)const 
{
	matrix4x4 out(
		f11*o,f12*o, f13*o,f14*o,
		f21*o,f22*o, f23*o,f24*o,
		f31*o,f32*o, f33*o,f34*o,
		f41*o,f42*o, f43*o,f44*o);

	out.isIdentityMatrix=false;
	return out;
}
bool matrix4x4::operator == (const matrix4x4&o)const
{
	return	mat[0]==o.mat[0] && mat[1]==o.mat[1] &&
			mat[2]==o.mat[2] && mat[3]==o.mat[3] &&
			mat[4]==o.mat[4] && mat[5]==o.mat[5] &&
			mat[6]==o.mat[6] && mat[7]==o.mat[7] &&
			mat[8]==o.mat[8] && mat[9]==o.mat[9] &&
			mat[10]==o.mat[10] && mat[11]==o.mat[11] &&
			mat[12]==o.mat[12] && mat[13]==o.mat[13] &&
			mat[14]==o.mat[14] && mat[15]==o.mat[15];
}

//! inverse Transforms a vector by this matrix
//! it's only used if the matrix was a rotation and translation matrix
//! if it has scale then it well gives wrong results
vector3d matrix4x4::inverseTransformVector(const vector3d& vec)const
{
	vector3d tmp=vec;
	tmp-=vector3d(f14,f24,f34);
	return vector3d(tmp.x*mat[0]+tmp.y*mat[1]+tmp.z*mat[2],
					tmp.x*mat[4]+tmp.y*mat[5]+tmp.z*mat[6],
					tmp.x*mat[8]+tmp.y*mat[9]+tmp.z*mat[10]);
}

vector3d matrix4x4::transformNormal(const vector3d &vec)const 
{
	return vector3d(
		f11*vec.x+f12*vec.y+f13*vec.z,
		f21*vec.x+f22*vec.y+f23*vec.z,
		f31*vec.x+f32*vec.y+f33*vec.z);
}

//! Transforms a box by this matrix
void matrix4x4::transformBox(box3d &box)const 
{
	/*vector3d edges[8];
	box.getEdges(edges);
	transformVectPre(edges[0]);
	box.reset(edges[0]);
	for(int i=1;i<8;++i){
		transformVectPre(edges[i]);
		box.AddPoint(edges[i]);
	}
	vector3d v;
	v.x=mat[0]*box.MinP.x+mat[4]*box.MinP.y+mat[8]*box.MinP.z+mat[12];
	v.y=mat[1]*box.MinP.x+mat[5]*box.MinP.y+mat[9]*box.MinP.z+mat[13];
	v.z=mat[2]*box.MinP.x+mat[6]*box.MinP.y+mat[10]*box.MinP.z+mat[14];
	box.MinP=v;

	v.x=mat[0]*box.MaxP.x+mat[4]*box.MaxP.y+mat[8]*box.MaxP.z+mat[12];
	v.y=mat[1]*box.MaxP.x+mat[5]*box.MaxP.y+mat[9]*box.MaxP.z+mat[13];
	v.z=mat[2]*box.MaxP.x+mat[6]*box.MaxP.y+mat[10]*box.MaxP.z+mat[14];
	box.MaxP=v;
	*/
	/*
	box.MinP=inversetransformVectPre(box.MinP);
	box.MaxP=inversetransformVectPre(box.MaxP);
	box+=getTranslation();*/


	box.MinP=*this*box.MinP;
	box.MaxP=*this*box.MaxP;
	box.repair();
}
//! Transforms a plane by this matrix
void matrix4x4::transformPlane( Plane &plane) const
{
	vector3d member=plane.getMemberPoint();
	plane.Normal=transformNormal(plane.Normal);
	plane.D = - member.dotProduct(plane.Normal);
}

float matrix4x4::calcDim()const 
{
	return (mat[0] * mat[5] - mat[1] * mat[4]) * (mat[10] * mat[15] - mat[11] * mat[14]) -
		(mat[0] * mat[6] - mat[2] * mat[4]) * (mat[9] * mat[15] - mat[11] * mat[13]) +
		(mat[0] * mat[7] - mat[3] * mat[4]) * (mat[9] * mat[14] - mat[10] * mat[13]) +
		(mat[1] * mat[6] - mat[2] * mat[5]) * (mat[8] * mat[15] - mat[11] * mat[12]) -
		(mat[1] * mat[7] - mat[3] * mat[5]) * (mat[8] * mat[14] - mat[10] * mat[12]) +
		(mat[2] * mat[7] - mat[3] * mat[6]) * (mat[8] * mat[13] - mat[9] * mat[12]);
}

matrix4x4 & matrix4x4::rotateX(float angle)
{
	float ca=(float)cos(toRad(angle));
	float sa=(float)sin(toRad(angle));

	matrix4x4 m;
	m(5)=ca;
	m(6)=sa;
	m(9)=-sa;
	m(10)=ca;
	m.isIdentityMatrix=0;
	*this=*this*m;

	isIdentityMatrix=0;
	return *this;
}
matrix4x4 & matrix4x4::rotateY(float angle)
{
	float ca=(float)cos(toRad(angle));
	float sa=(float)sin(toRad(angle));

	matrix4x4 m;
	m(0)=ca;m(2)=-sa;
	m(8)=sa;m(10)=ca;
	m.isIdentityMatrix=0;
	*this=*this*m;

	isIdentityMatrix=0;
	return *this;
}
matrix4x4 & matrix4x4::rotateZ(float angle)
{
	float ca=(float)cos(toRad(angle));
	float sa=(float)sin(toRad(angle));

	matrix4x4 m;
	m(0)=ca;m(1)=sa;
	m(4)=-sa;m(5)=ca;
	m.isIdentityMatrix=0;
	*this=*this*m;

	isIdentityMatrix=0;
	return *this;
}

vector3d matrix4x4::getRotation()const 
{
	double Y = -asin(mat[2]);
	double C = cos(Y);
	Y *= GRAD_PI64;

	double rotx, roty, X, Z;

	if (fabs(C)>0.0005f)
	{
		rotx = mat[10] / C;
		roty = mat[6] / C;
		X = atan2( roty, rotx ) * GRAD_PI64;
		rotx = mat[0] / C;
		roty = mat[1] / C;
		Z = atan2( roty, rotx ) * GRAD_PI64;
	}
	else
	{
		X = 0.0f;
		rotx = mat[5];
		roty = -mat[4];
		Z = atan2( roty, rotx ) * GRAD_PI64;
	}
/*
	// fix values that get below zero
	// before it would set (!) values to 360
	// that where above 360:
	if (X < 0.0) X += 360.0;
	if (Y < 0.0) Y += 360.0;
	if (Z < 0.0) Z += 360.0;
*/
	return vector3d((float)X,(float)Y,(float)Z);
}

matrix4x4& matrix4x4::setTranslation(const vector3d& trans)
{
	f14=trans.x;
	f24=trans.y;
	f34=trans.z;

	isIdentityMatrix=0;
	return *this;
}
matrix4x4& matrix4x4::setScale(const vector3d& scale)
{
	mat[0]=scale.x;
	mat[5]=scale.y;
	mat[10]=scale.z;

	isIdentityMatrix=0;
	return *this;
}
vector3d matrix4x4::getTranslation()const 
{
	return vector3d(f14,f24,f34);
}

matrix4x4& matrix4x4::setRotationRadians(const vector3d& angles)
{
	float cx=(float)cos(angles.x);
	float sx=(float)sin(angles.x);
	float cy=(float)cos(angles.y);
	float sy=(float)sin(angles.y);
	float cz=(float)cos(angles.z);
	float sz=(float)sin(angles.z);

	mat[0]=cy*cz;
	mat[1]=cy*sz;
	mat[2]=-sy;
	
	float sxsy=sx*sy;
	float cxsy=cx*sy;

	mat[4]=(cz*sxsy -cx*sz);
	mat[5]=(sz*sxsy +cx*cz);
	mat[6]=sx*cy;
	
	mat[8]=(cxsy*cz + sx*sz);
	mat[9]=(cxsy*sz - sx*cz);
	mat[10]=cx*cy;

	isIdentityMatrix=0;
	return *this;
}
matrix4x4& matrix4x4::setRotationDegrees(const vector3d& angles)
{
	return setRotationRadians(angles/GRAD_PI32);
}

vector3d matrix4x4::getScale()const 
{
	return vector3d(mat[0],mat[5],mat[10]);
}

void matrix4x4::setComponent(const vector3d& a,const vector3d& b,const vector3d& c){
	mat[0]=a.x;
	mat[1]=b.x;
	mat[2]=c.x;
	mat[3]=0;

	mat[4]=a.y;
	mat[5]=b.y;
	mat[6]=c.y;
	mat[7]=0;

	mat[8]=a.z;
	mat[9]=b.z;
	mat[10]=c.z;
	mat[11]=0;
	
	mat[12]=mat[13]=mat[14]=0;
	mat[15]=1;
	isIdentityMatrix=0;
}


float* matrix4x4::pointer(){
	return (float*)mat;
}

}
}
