
#ifndef ___LINE3D___
#define ___LINE3D___

#include "Point3d.h"

namespace mray{
namespace math{

class line3d
{
public:
	Point3df pStart;
	Point3df pEnd;

	line3d():pStart(0,0,0),pEnd(0,0,0){};
	line3d(const Point3df &p1,const Point3df &p2):pStart(p1),pEnd(p2){};
	line3d(const line3d &l):pStart(l.pStart),pEnd(l.pEnd){};
	line3d(float sX,float sY,float sZ,float eX,float eY,float eZ):pStart(sX,sY,sZ),pEnd(eX,eY,eZ){};

	line3d &operator+=(const Point3df&p){pStart+=p;pEnd+=p;return *this;}
	line3d &operator-=(const Point3df&p){pStart-=p;pEnd-=p;return *this;}

	line3d  operator+(const Point3df&p)const{return line3d(pStart+p,pEnd+p);}
	line3d  operator-(const Point3df&p)const{return line3d(pStart-p,pEnd-p);}

	bool operator ==(const line3d&l)const{return pStart==l.pStart && pEnd==l.pEnd;}
	bool operator !=(const line3d&l)const{return pStart!=l.pStart || pEnd!=l.pEnd;}

	double Length() const
	{
		return pStart.getDist(pEnd);
	}
	double LengthSQ() const
	{
		return pStart.getDistSQ(pEnd);
	}

	Point3df getVector() const
	{
		return (pEnd-pStart);
	}

	bool IntersctWithLine(const line3d&l,Point3df &out)const
	{
		float a1,a2,m1,m2;
		if(pStart.x==pEnd.x)
			m1=(float)1e+10;
		else
			m1=(pEnd.y-pStart.y)/(pEnd.x-pStart.x);

		if(l.pStart.x==l.pEnd.x)
			m2=(float)1e+10;
		else
			m2=(l.pEnd.y-l.pStart.y)/(l.pEnd.x-l.pStart.x);

		a1=pStart.y - m1*pStart.x;
		a2=l.pStart.y - m2*l.pStart.x;

		out.x=-(a1-a2)/(m1-m2);
		out.y=a1+ m1*out.x;
		if( (pStart.x  -out.x)*(out.x-pEnd.x  )>=0 &&
			(l.pStart.x-out.x)*(out.x-l.pEnd.x)>=0 &&
			(pStart.y  -out.y)*(out.y-pEnd.y  )>=0 &&
			(l.pStart.y-out.y)*(out.y-l.pEnd.y)>=0 )
		{
			return 1;
		}
		return 0;
	}

	Point3df UnitVector() const
	{
		return (pEnd-pStart).Normalize();
	}
	Point3df getMiddle() const
	{
		return (pStart + pEnd)* 0.5;
	}

	Point3df ClosestPoint(const Point3df&p)const
	{
		Point3df c=p-pStart;
		Point3df v=pEnd-pStart;

		float d=(float)v.Length();
		if(d)
			v /=d;
		float t=v.dotProduct(c);

		if(t<0.0)return pStart;
		if(t>d)return pEnd;

		v*=t;
		return pStart+v;
	}

	bool isBetween(const Point3df &p)const
	{
		return p.isBetween(pStart,pEnd);
	}

	bool IntersectSphere(const Point3df &spos,float r,double outD)const
	{
		Point3df q=spos-pStart;
		double c=q.Length();
		double v=q.dotProduct(getVector().Normalize());
		double d=r*r -(c*c -v*v);

		if(d<0.0)
			return 0;
		outD=v-sqrt(d);
		return 1;
	}
};


};//math
};//mray

#endif