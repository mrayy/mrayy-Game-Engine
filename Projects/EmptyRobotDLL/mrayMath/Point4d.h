
#ifndef ___POINT4d___
#define ___POINT4d___

#include "mMath.h"
#include "Point3d.h"
#include <assert.h>

namespace mray{
namespace math{

template <class T>
class Point4d
{
public:
	T x;
	T y;
	T z;
	T w;


	Point4d():x(0),y(0),z(0),w(0){}

	Point4d(T _x,T _y,T _z,T _w):x(_x),y(_y),z(_z),w(_w){}

	Point4d(T v):x(v),y(v),z(v),w(v){}

	template <class B>
	Point4d(const Point4d<B> &p):x(p.x),y(p.y),z(p.z),w(p.w){}

	Point4d<T>& operator += (const Point4d<T> &p){x+=p.x; y+=p.y;z+=p.z;w+=p.w;return *this;}
	Point4d<T>& operator -= (const Point4d<T> &p){x-=p.x; y-=p.y;z-=p.z;w-=p.w;return *this;}
	Point4d<T>  operator + (T v)const{return Point4d<T>(x+v,y+v,z+v,w+v);}
	Point4d<T>  operator - (T v)const{return Point4d<T>(x-v,y-v,z-v,w-v);}
	Point4d<T>  operator / (T v)const{return Point4d<T>(x/v,y/v,z/v,w/v);}
	Point4d<T>&  operator = (const Point4d<T> &p){x=p.x;y=p.y;z=p.z;w=p.w;return *this;}

	Point4d<T> operator *(const Point4d<T> &p)const{return Point4d<T>(x*p.x,y*p.y,z*p.z,w*p.w);}
	Point4d<T>&operator *=(const Point4d<T> &p) {x*=p.x;y*=p.y;z*=p.z;w*=p.w;return *this;}
	Point4d<T>&operator *=( T v){x*=v;y*=v;z*=v;w*=v;return *this;}

	Point4d<T> operator /(const Point4d<T> &p)const{return Point4d<T>(x/p.x,y/p.y,z/p.z,w/p.w);}
	Point4d<T>&operator /=(const Point4d<T> &p) {x/=p.x;y/=p.y;z/=p.z;w/=p.w;return *this;}
	Point4d<T>&operator /=(T v){x/=v;y/=v;z/=v;w/=v;return *this;}

	bool operator <=(const Point4d<T>&p)const{return x<=p.x && y<=p.y && z<=p.z&& w<=p.w;}
	bool operator >=(const Point4d<T>&p)const{return x>=p.x && y>=p.y && z>=p.z && w>=p.w;}
	bool operator == (const Point4d<T> &p)const{return (x==p.x && y==p.y && z==p.z && w==p.w);}
	bool operator != (const Point4d<T> &p)const{return (x!=p.x || y!=p.y || z!=p.z || w!=p.w);}

	bool Equal(const Point4d<T> &p)const{
		return fEqual(x,p.x)&&fEqual(y,p.y)&&fEqual(z,p.z)&&fEqual(w,p.w);
	}

	T dotProduct(const Point4d<T>&p)const{
		return x*p.x+y*p.y+z*p.z+w*p.w;
	}


	double getDistSQ(const Point4d<T>&p)const{
		return Point4d<T>(x-p.x,y-p.y,z-p.z,w-p.w).LengthSQ();
	}
	double getDist(const Point4d<T>&p)const{
		return sqrt(getDistSQ(p));
	}

	bool isBetween(const Point4d<T> &p1,const Point4d<T> &p2)const
	{
		T f=p1.getDistSQ(p2);
		return (getDistSQ(p1)<f && getDistSQ(p2)<f);
	}

	Point4d<T> setLength(T len)
	{
		Normalize();
		*this *= len;
		return *this;
	}
	Point4d<T> invert()
	{
		x*=-1;
		y*=-1;
		z*=-1;
		w*=-1;
		return *this;
	}

	void getAsArray(T*arr)const
	{
		arr[0]=x;
		arr[1]=y;
		arr[2]=z;
		arr[3]=w;
	}

	math::Point3d<T> XYZ()const
	{
		return math::Point3d<T>(x,y,z);
	}


	T &operator [] (int i){
		assert(i<4);
		if(i==0)return x;
		if(i==1)return y;
		if(i==2)return z;
		return w;
	}
	T operator [] (int i)const{
		if(i==0)return x;
		if(i==1)return y;
		if(i==2)return z;
		if(i==3)return w;
		return 0;
	}
	Point4d<T>  operator + (const Point4d<T> &p)const
	{
		return Point4d<T>(x+p.x,y+p.y,z+p.z,w+p.w);
	}

	Point4d<T>  operator - (const Point4d<T> &p)const
	{
		return Point4d<T>(x-p.x,y-p.y,z-p.z,w-p.w);
	}
	Point4d<T>  operator - ()const{return Point4d<T>(-x,-y,-z,-w);}
	Point4d<T>  operator * (T v)const{return Point4d<T>(x*v,y*v,z*v,w*v);}
	Point4d<T>  &operator = (const T v){x=v;y=v;z=v;w=v;return *this;}


	void set(T _x,T _y,T _z,T _w)
	{
		x=_x;
		y=_y;
		z=_z;
		w=_w;
	}
	double LengthSQ()const{return x*x+y*y+z*z+w*w;}
	double Length()const{return sqrt(LengthSQ());}



	Point4d<T> Normalize()
	{
		T len=(T)Length();
		if(len){
			len=(T) 1.0/len;
			x*=len;
			y*=len;
			z*=len;
			w*=len;
		}
		return *this;
	}


	static const Point4d<T> Zero;

};
template <class T>
const Point4d<T> Point4d<T>::Zero=Point4d<T>(0,0,0,0);

typedef Point4d<int> Point4di;
typedef Point4d<float> Point4df;

typedef Point4d<int> vector4di;
typedef Point4d<float> vector4df;

typedef Point4d<float> vector4d;

/*
template <class S,class T>
Point4d<T> operator *(const S s,Point4d<T> &p){return p*s;}
template <class S,class T>
Point4d<T> operator +(const S s,Point4d<T> &p){return p+s;}
template <class S,class T>
Point4d<T> operator -(const S s,Point4d<T> &p){return (-p)-(-s);}
*/


};//math
};//mray

#endif