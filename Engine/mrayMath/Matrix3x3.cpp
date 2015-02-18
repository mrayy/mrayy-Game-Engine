#include "matrix3x3.h"


namespace mray{
namespace math{

const matrix3x3 matrix3x3::Identity;

matrix3x3::matrix3x3(){
	makeIdentity();
}
matrix3x3::matrix3x3(float e11,float e12,float e13,
					 float e21,float e22,float e23,
					 float e31,float e32,float e33):
		f11(e11),f12(e12),f13(e13),
		f21(e21),f22(e22),f23(e23),
		f31(e31),f32(e32),f33(e33),
		m_isIdentity(0)

{
	
}


float*matrix3x3::pointer(){
	return mat;
}

float* matrix3x3::operator[](int row){
	if(row>2)row=2;
	return mat3x3[row];
}
const float* matrix3x3::operator[](int row)const{
	if(row>2)row=2;
	return mat3x3[row];
}
float& matrix3x3::operator () (int r,int c){
	return mat3x3[r][c];
}
float matrix3x3::operator () (int r,int c)const{
	return mat3x3[r][c];
}

matrix3x3 matrix3x3::operator+(const matrix3x3&o)const{
// 	if(m_isIdentity)
// 		return o;
// 	if(o.isIdentity())
// 		return *this;
	return matrix3x3(
		f11+o.f11,f12+o.f12,f13+o.f13,
		f21+o.f21,f22+o.f22,f23+o.f23,
		f31+o.f31,f32+o.f32,f33+o.f33
		);
}
matrix3x3 matrix3x3::operator-(const matrix3x3&o)const{
// 	if(m_isIdentity)
// 		return o;
// 	if(o.isIdentity())
// 		return *this;
	return matrix3x3(
		f11-o.f11,f12-o.f12,f13-o.f13,
		f21-o.f21,f22-o.f22,f23-o.f23,
		f31-o.f31,f32-o.f32,f33-o.f33
		);
}
matrix3x3 matrix3x3::operator*(const matrix3x3&o)const{
	//if(m_isIdentity)
	//	return o;
	//if(o.isIdentity())
	//	return *this;

	matrix3x3 m(
		f11*o.f11+f12*o.f21+f13*o.f31,
		f11*o.f12+f12*o.f22+f13*o.f32,
		f11*o.f13+f12*o.f23+f13*o.f33,

		f21*o.f11+f22*o.f21+f23*o.f31,
		f21*o.f12+f22*o.f22+f23*o.f32,
		f21*o.f13+f22*o.f23+f23*o.f33,

		f31*o.f11+f32*o.f21+f33*o.f31,
		f31*o.f12+f32*o.f22+f33*o.f32,
		f31*o.f13+f32*o.f23+f33*o.f33);

	return m;
}

matrix3x3 matrix3x3::operator-()const{
	return matrix3x3(
		-f11,-f12,-f13,
		-f21,-f22,-f23,
		-f31,-f32,-f33);
}

matrix3x3 matrix3x3::operator*(float v)const{
	return matrix3x3(
		f11*v,f12*v,f13*v,
		f21*v,f22*v,f23*v,
		f31*v,f32*v,f33*v);
}

vector2d matrix3x3::operator*(const vector2d& v)const{
	if(m_isIdentity)
		return v;
	vector2d res=0;
	res.x=f11*v.x+f12*v.y+f13;
	res.y=f21*v.x+f22*v.y+f23;
	return res;
}

void matrix3x3::makeIdentity(){
	f11=1;f12=0;f13=0;
	f21=0;f22=1;f23=0;
	f31=0;f32=0;f33=1;

	m_isIdentity=true;
}

bool matrix3x3::isIdentity()const{
	return m_isIdentity;
}
bool matrix3x3::calcIsIdentity(){
	m_isIdentity=false;
	for(int i=0;i<3;++i){
	for(int j=0;j<3;j++){
		if(i==j){
			if(!fEqual(mat3x3[i][j],1))
				return false;
		}else if(!isZero(mat3x3[i][j]))
			return false;
	}
	}
	m_isIdentity=true;
	return true;
}


void matrix3x3::setAngle(double angle){
	float sn=0,cs=1;
	if(angle!=0){
		cs=(float)cosd(angle);
		sn=(float)sind(angle);
		m_isIdentity=false;
	}

	f11=cs;f12=-sn;
	f21=sn;f22=cs;

}
double matrix3x3::getAngle()const{
	return toDeg(atan2f(f21,f11));
}

void matrix3x3::translate(const vector2d& v){
	f13=v.x;
	f23=v.y;
	if(v!=0)
		m_isIdentity=false;
}
vector2d matrix3x3::getTranslate()const{
	return vector2d(f13,f23);
}

}
}


