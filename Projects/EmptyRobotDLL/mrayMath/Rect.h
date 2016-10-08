
#ifndef ___RECT___
#define ___RECT___

#include "Point2d.h"
#include "mMath.h"


namespace mray{
namespace math{

template <class T>
class rect
{
public:
	Point2d<T> ULPoint,BRPoint;

	rect ():ULPoint(0,0),BRPoint(0,0){}
	rect (T x1,T y1,T x2,T y2):ULPoint(x1,y1),BRPoint(x2,y2){}
	rect (const Point2d<T> &p1,const Point2d<T> &p2):ULPoint(p1),BRPoint(p2){}
	rect (const rect<T> &r):ULPoint(r.ULPoint),BRPoint(r.BRPoint){}
	rect (const Point2d<T> &pos,T size):ULPoint(pos),
		BRPoint(pos.x+size,pos.y+size){};

	
	template <class B>
	rect(const rect<B> &p):ULPoint(p.ULPoint),BRPoint(p.BRPoint){}


	rect<T> operator +(const rect<T> &r)const{return rect<T>(ULPoint+r.ULPoint,BRPoint+r.BRPoint);};
	rect<T> operator -(const rect<T> &r)const{return rect<T>(ULPoint-r.ULPoint,BRPoint-r.BRPoint);};
	rect<T>&operator +=(const rect<T>&r){ULPoint+=r.ULPoint;BRPoint+=r.BRPoint;return *this;};
	rect<T>&operator -=(const rect<T>&r){ULPoint-=r.ULPoint;BRPoint-=r.BRPoint;return *this;};

	rect<T> operator +(const Point2d<T> &p)const{return rect<T>(ULPoint+p,BRPoint+p);};
	rect<T> operator -(const Point2d<T> &p)const{return rect<T>(ULPoint-p,BRPoint-p);};
	rect<T>&operator +=(const Point2d<T>&p){ULPoint+=p;BRPoint+=p;return *this;};
	rect<T>&operator -=(const Point2d<T>&p){ULPoint-=p;BRPoint-=p;return *this;};

	rect<T> operator +(T v)const{return rect<T>(ULPoint+v,BRPoint+v);};
	rect<T> operator -(T v)const{return rect<T>(ULPoint-v,BRPoint-v);};
	bool operator == (const rect<T>&r)const{return (ULPoint==r.ULPoint && BRPoint==r.BRPoint);};
	bool operator != (const rect<T>&r)const{return (ULPoint!=r.ULPoint || BRPoint!=r.BRPoint);};

	rect<T>& operator = (const rect<T>&r){ULPoint=r.ULPoint;BRPoint=r.BRPoint;return *this;};

	bool IsPointInside( const Point2d<T> &p)const
	{
		return (p.x>ULPoint.x && p.x<BRPoint.x  &&
			    p.y>ULPoint.y && p.y<BRPoint.y);
	}

	bool IsRectCollide( const rect<T> &r)const
	{
		return (IsPointInside(r.BRPoint) || IsPointInside(r.ULPoint) );
	}
	bool IsRectInside( const rect<T> &r)const
	{
		return (IsPointInside(r.BRPoint) && IsPointInside(r.ULPoint) );
	}

	void Repair()
	{
		if(ULPoint.x>BRPoint.x)Swap<T>(ULPoint.x,BRPoint.x);
		if(ULPoint.y>BRPoint.y)Swap<T>(ULPoint.y,BRPoint.y);
	}

	void clipRect(const rect<T> &rc)
	{
		if(ULPoint.y<rc.ULPoint.y)
			ULPoint.y=rc.ULPoint.y;
		if(ULPoint.x<rc.ULPoint.x)
			ULPoint.x=rc.ULPoint.x;
		if(BRPoint.y>rc.BRPoint.y)
			BRPoint.y=rc.BRPoint.y;
		if(BRPoint.x>rc.BRPoint.x)
			BRPoint.x=rc.BRPoint.x;
		if(ULPoint.y>BRPoint.y)ULPoint.y=BRPoint.y;
		if(ULPoint.x>BRPoint.x)ULPoint.x=BRPoint.x;
	}
	Point2d<T> getCenter()const
	{
		return Point2d<T>((BRPoint.x+ULPoint.x)/2,(BRPoint.y+ULPoint.y)/2);
	}

	T getHeight()const
	{
		return BRPoint.y-ULPoint.y;
	}
	T getWidth()const
	{
		return BRPoint.x-ULPoint.x;
	}

	Point2d<T> getSize()const
	{
		return Point2d<T>(getWidth(),getHeight());
	}

	bool isValid()const
	{
		return (getWidth()>0 && getHeight()>0 );
	}

	void reset(const Point2d<T>&p)
	{
		BRPoint=ULPoint=p;
	}
	void addPoint(const Point2d<T>&p)
	{
		if(p.x<ULPoint.x)ULPoint.x=p.x;
		if(p.x>BRPoint.x)BRPoint.x=p.x;
		if(p.y<ULPoint.y)ULPoint.y=p.y;
		if(p.y>BRPoint.y)BRPoint.y=p.y;
	}
};

typedef rect<int> recti;
typedef rect<float> rectf;


};//math
};//mray

#endif