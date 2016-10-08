
#ifndef ___POINT2D___
#define ___POINT2D___

#include "compileConfig.h"

namespace mray{
namespace math{

template <class T>
class Point2d
{
public:
	T x,y;

	Point2d():x(0),y(0){};
	Point2d(T _x,T _y):x(_x),y(_y){};
	Point2d(T v):x(v),y(v){};
	Point2d(const Point2d<T> &p):x(p.x),y(p.y){};
	template <class B>
	Point2d(const Point2d<B>& v):x((T)v.x),y((T)v.y){};

	Point2d<T>& operator += (const Point2d<T> &p){x+=p.x; y+=p.y;return *this;}
	Point2d<T>& operator -= (const Point2d<T> &p){x-=p.x; y-=p.y;return *this;}
	Point2d<T>& operator += (T v){x+=v; y+=v;return *this;}
	Point2d<T>& operator -= (T v){x-=v; y-=v;return *this;}
	Point2d<T>& operator -- (){x--; y--;return *this;}
	Point2d<T>& operator ++ (){x++; y++;return *this;}
	Point2d<T>  operator + (const Point2d<T> &p)const{return Point2d<T>(x+p.x,y+p.y);}
	Point2d<T>  operator - (const Point2d<T> &p)const{return Point2d<T>(x-p.x,y-p.y);}
	Point2d<T>  operator + (T p)const{return Point2d<T>(x+p,y+p);}
	Point2d<T>  operator - (T p)const{return Point2d<T>(x-p,y-p);}
	Point2d<T>  operator = (const Point2d<T> &p){x=p.x;y=p.y;return *this;}
	Point2d<T>  operator = (T v){x=v;y=v;return *this;}

	Point2d<T> operator *(const Point2d<T> &p)const{return Point2d<T>(x*p.x,y*p.y);};
	Point2d<T>&operator *=(const Point2d<T> &p){x*=p.x;y*=p.y;return *this;};
	Point2d<T> operator *(const T v)const{return Point2d<T>(x*v,y*v);};
	Point2d<T>&operator *=(const T v){x*=v;y*=v;return *this;};

	Point2d<T> operator /(const Point2d<T> &p)const{return Point2d<T>(x/p.x,y/p.y);};
	Point2d<T>&operator /=(const Point2d<T> &p){x/=p.x;y/=p.y;return *this;};
	Point2d<T> operator /(const T v)const{return Point2d<T>(x/v,y/v);};
	Point2d<T>&operator /=(const T v){x/=v;y/=v;return *this;};

	bool operator <=(const Point2d<T>&p)const{return x<=p.x && y<=p.y ;};
	bool operator >=(const Point2d<T>&p)const{return x>=p.x && y>=p.y ;};
	bool operator == (const Point2d<T> &p)const{return (x==p.x && y==p.y );}
	bool operator != (const Point2d<T> &p)const{return (x!=p.x || y!=p.y );}

	bool Equal(const Point2d<T> &p){
		return fEqual(x,p.x)&&fEqual(y,p.y);
	}

	double LengthSQ(){return x*x+y*y;};
	double Length(){return sqrt(LengthSQ());};

	T dotProduct(const Point2d<T>&p){
		return x*p.x+y*p.y;
	}


	double getDistSQ(const Point2d<T>&p){
		return Point2d<T>(x-p.x,y-p.y).LengthSQ();
	}
	double getDist(const Point2d<T>&p){
		return sqrt(getDistSQ(p));
	}


	Point2d<T> Normalize()
	{
		T len=(T)Length();
		if(len){
			len=(T) 1.0f/len;
			x*=len;
			y*=len;
		}
		return *this;
	}

	Point2d<T> setLength(T len)
	{
		Normalize();
		*this *= len;
		return *this;
	}

	Point2d<T> invert()
	{
		x*=-1;
		y*=-1;
		return *this;
	}


	void getAsArray(T*arr)
	{
		arr[0]=x;
		arr[1]=y;
		arr[2]=0;
	}

	/**
	* Calculates which side of a line (v1,v2) this point is on.
	* \return -1 if it's is left of line segment 'v1->v2',
	*          1 if it's is right of segment 'v1->v2'
	*       or 0 if it's lies on segment 'v1->v2'.
	*/
	int getSide(const Point2d<T> &v1,const Point2d<T> &v2)const
	{
		T k=(v1.y-y)*(v2.x-v1.x);
		T k1=(v1.x-x)*(v2.y-v1.y);
		if(k<k1)return -1;
		if(k>k1)return +1;
		return 0;
	}

	void set(const T &x,const T &y){
		this->x=x;
		this->y=y;
	}

	static const Point2d<T> Zero;
	static const Point2d<T> XAxis;
	static const Point2d<T> YAxis;

};

template <class T>
const Point2d<T> Point2d<T>::Zero=Point2d<T>(0,0);
template <class T>
const Point2d<T> Point2d<T>::XAxis=Point2d<T>(1,0);
template <class T>
const Point2d<T> Point2d<T>::YAxis=Point2d<T>(0,1);

typedef Point2d<int> Point2di;
typedef Point2d<float> Point2df;

typedef Point2d<int> vector2di;
typedef Point2d<float> vector2df;

typedef Point2d<float> vector2d;
///////////////////////////////////////////////////////////////////////////

template <class S,class T>
Point2d<T> operator *(const S s,Point2d<T> &p){return p*s;}


};//math
};//mray

#endif