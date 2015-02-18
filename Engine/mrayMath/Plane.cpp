
#include "Plane.h"

namespace mray{
namespace math{

const Plane Plane::XYPlane=Plane(0,vector3d(0,0,1));
const Plane Plane::XZPlane=Plane(0,vector3d(0,1,0));
const Plane Plane::YZPlane=Plane(0,vector3d(1,0,0));

Plane::Plane():Normal(0,1,0)
{
	recalcD(Point3df(0,0,0));
}
Plane::Plane(const vector3d& point,const vector3d& normal)
	:Normal(normal)
{
	recalcD(point);
}
Plane::Plane(const vector3d& p1,const vector3d& p2,const vector3d& p3)
{
	calcPlane(p1,p2,p3);
}
Plane::Plane(float x,float y,float z,float nx,float ny,float nz)
	:Normal(nx,ny,nz)
{
	recalcD(Point3df(x,y,z));
}

void Plane::calcPlane(const vector3d& v1,const vector3d& v2,const vector3d& v3)
{
	Normal=(v2-v1).crossProduct(v3-v1);
	Normal.Normalize();
	recalcD(v1);
}

void Plane::set(const vector3d& normal,float d)
{
	Normal=normal;
	D=d;
}
void Plane::set(const vector3d& nVec,const vector3d& p)
{
	Normal=nVec;
	Normal.Normalize();
	recalcD(p);
}
void Plane::recalcD(const vector3d& v)
{
	D=-v.dotProduct(Normal);
}

float Plane::getDistance(const vector3d& v)const{
	return Normal.dotProduct(v)+D;
}

bool Plane::getIntersectionWithLine(const vector3d&linePoint,
	const vector3d&lineVec,Point3df&out) const
{
	float t1=Normal.dotProduct(lineVec);
	if(t1==0)return 0;

	float t2=-(Normal.dotProduct(linePoint)+D)/t1;
	out=linePoint+lineVec*t2;
	return 1;
}

bool Plane::getIntersectionWithPlane(const Plane&other,vector3d&outLinePoint,
	Point3df&outLineVec) const
{
	double f1=Normal.Length();
	double f2=Normal.dotProduct(other.Normal);
	double f3=other.Normal.Length();

	double det=f1*f3-f2*f2;

	if(abs(det)<math::Zero)
		return 0;
	det=1.0/det;
	
	float fc0=(float)((f3*-D +f2*other.D)*det);
	float fc1=(float)((f1*-other.D +f2*D)*det);

	outLineVec   =Normal.crossProduct(other.Normal);
	outLinePoint =Normal*fc0+other.Normal*fc1;
	return 1;
}
bool Plane::getIntersectionWithPlanes(const  Plane&p1,const  Plane&p2,
	Point3df&outPoint) const
{
	Point3df linePoint,lineVec;
	if(getIntersectionWithPlane(p1,linePoint,lineVec))
		return p2.getIntersectionWithLine(linePoint,lineVec,outPoint);
	return 0;
}
//<0 : behind the plane
//==0: on the plane
//<1 :infront of the plane
int Plane::getVertLocation(const vector3d& p)const{
	float d=getDistance(p);
	if(d<-math::RoundingValue)return -1;
	if(d>math::RoundingValue )return  1;
	return 0;
}
int Plane:: getVertLocation(const vector3d& p,const vector3d& extent)const{
	float d=getDistance(p);
	float maxDist=Normal.dotProduct(extent);
	if(d<-maxDist)return -1;
	if(d>maxDist )return  1;
	return 0;
}

Point3df Plane::getMemberPoint() const
{
	return Normal * -D;
}




};//math
};//mray


