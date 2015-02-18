

#include "triangle3d.h"
//#include "mTypes.h"

namespace mray{
namespace math{

triangle3d::triangle3d():pA(0),pB(0),pC(0)
{
}

triangle3d::triangle3d(const vector3d& a,const vector3d& b,const vector3d& c):
pA(a),pB(b),pC(c)
{
}

void triangle3d::set(const vector3d& a,const vector3d& b,const vector3d& c){
	pA=a;
	pB=b;
	pC=c;
}

bool triangle3d::operator==(const triangle3d &t)const{
	return (pA==t.pA &&pB==t.pB && pC==t.pC);
}
bool triangle3d::operator!=(const triangle3d &t)const{
	return (pA!=t.pA || pB!=t.pB || pC!=t.pC);
}

bool triangle3d::isInsideBox(const box3d&b)const{
	return b.isPointInSide(pA) && b.isPointInSide(pB) && b.isPointInSide(pC) ;
}

bool triangle3d::isPointInside(const vector3d&v)const{
	return isOnSameSide(v,pA,pB,pC)&&
		isOnSameSide(v,pB,pA,pC)&&
		isOnSameSide(v,pC,pA,pB);
}
bool triangle3d::isPointInsideFast(const vector3d&v)const{
	vector3d f=pB-pA;
	vector3d g=pC-pA;

	float a=f.dotProduct(f);
	float b=f.dotProduct(g);
	float c=g.dotProduct(g);

	float ac_bb=a*c-b*b;
	vector3d vp=v-pA;
	
	float d = vp.dotProduct(f);
	float e = vp.dotProduct(g);
	float x = (d*c)-(e*b);
	float y = (e*a)-(d*b);
	float z = x+y-ac_bb;

	return (( ((unsigned int&)z)& ~(((unsigned int&)x)|((unsigned int&)y))) & 0x80000000)!=0;
}


bool triangle3d::isIntersectWithLimitedLine(const line3d &ln,vector3d &out)const{
	return isIntersectWithLine(ln.pStart,ln.getVector(),out) &&
		out.isBetween(ln.pStart,ln.pEnd);
}

bool triangle3d::isIntersectWithLine(const vector3d &start,const vector3d &dir,
									 vector3d &out)const{
	return getIntersectPlaneWithLine(start,dir,out)&&
		isPointInsideFast(out);
}

bool triangle3d::getIntersectPlaneWithLine(const vector3d &start,const vector3d &dir,
										   vector3d &out)const{

	vector3d n=getNormal().Normalize();

	vector3d vec=dir;

	vec.Normalize();

	float t=n.dotProduct(vec);
	if(t==0)return 0;

	float d=pA.dotProduct(n);
	float t2=-(n.dotProduct(start)-d)/t;
	out=start+vec*t2;
	return 1;
}

bool triangle3d::isOnSameSide(const vector3d &p,const vector3d &a,const vector3d &b,
		const vector3d &c)const
{
	vector3d cmb=c-b;
	vector3d cp1=cmb.crossProduct(p-b);
	vector3d cp2=cmb.crossProduct(a-b);
	return cp1.dotProduct(cp2)>=0;
}




vector3d triangle3d::closestPointOnTriangle(const vector3d&v)const{
	vector3d rab = line3d(pA, pB).ClosestPoint(v);
	vector3d rbc = line3d(pB, pC).ClosestPoint(v);
	vector3d rca = line3d(pC, pA).ClosestPoint(v);

	float d1 = (float)rab.getDist(v);
	float d2 = (float)rbc.getDist(v);
	float d3 = (float)rca.getDist(v);

	if (d1 < d2)
		return d1 < d3 ? rab : rca;

	return d2 < d3 ? rbc : rca;
}

vector3d triangle3d::getNormal()const{
	return (pB-pA).crossProduct(pC-pA);
}
bool triangle3d::isFrontFacing(const vector3d& dir)const{
	math::vector3d n=getNormal();
	n.Normalize();
	return n.dotProduct(dir)<=0;
}

Plane triangle3d::getPlane()const{
	return Plane(pA,pB,pC);
}

float triangle3d::area(){
	return ((float)(pB-pA).crossProduct(pC-pA).Length())*0.5f;
}



}
}

