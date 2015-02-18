
#ifndef ___POINT3D___
#define ___POINT3D___

#include "mMath.h"
#include <assert.h>

namespace mray{
namespace math{

template <class T>
class MRAY_MATH_DLL Point3d
{
public:
	T x;
	T y;
	T z;


	Point3d():x(0),y(0),z(0){}

	Point3d(T _x,T _y,T _z):x(_x),y(_y),z(_z){}

	Point3d(T v):x(v),y(v),z(v){}

	template <class B>
	Point3d(const Point3d<B> &p):x((T)p.x),y((T)p.y),z((T)p.z){}

	Point3d<T>& operator += (const Point3d<T> &p){x+=p.x; y+=p.y;z+=p.z;return *this;}
	Point3d<T>& operator -= (const Point3d<T> &p){x-=p.x; y-=p.y;z-=p.z;return *this;}
	Point3d<T>  operator + (T v)const{return Point3d<T>(x+v,y+v,z+v);}
	Point3d<T>  operator - (T v)const{return Point3d<T>(x-v,y-v,z-v);}
	Point3d<T>  operator / (T v)const{return Point3d<T>(x/v,y/v,z/v);}
	Point3d<T>&  operator = (const Point3d<T> &p){x=p.x;y=p.y;z=p.z;return *this;}

	Point3d<T> operator *(const Point3d<T> &p)const{return Point3d<T>(x*p.x,y*p.y,z*p.z);}
	Point3d<T>&operator *=(const Point3d<T> &p) {x*=p.x;y*=p.y;z*=p.z;return *this;}
	Point3d<T>&operator *=( T v){x*=v;y*=v;z*=v;return *this;}

	Point3d<T> operator /(const Point3d<T> &p)const{return Point3d<T>(x/p.x,y/p.y,z/p.z);}
	Point3d<T>&operator /=(const Point3d<T> &p) {x/=p.x;y/=p.y;z/=p.z;return *this;}
	Point3d<T>&operator /=(T v){x/=v;y/=v;z/=v;return *this;}

	bool operator <=(const Point3d<T>&p)const{return x<=p.x && y<=p.y && z<=p.z;}
	bool operator >=(const Point3d<T>&p)const{return x>=p.x && y>=p.y && z>=p.z;}
	bool operator == (const Point3d<T> &p)const{return (x==p.x && y==p.y && z==p.z);}
	bool operator != (const Point3d<T> &p)const{return (x!=p.x || y!=p.y || z!=p.z);}

	bool Equal(const Point3d<T> &p)const{
		return fEqual(x,p.x)&&fEqual(y,p.y)&&fEqual(z,p.z);
	}

	inline T dotProduct(const Point3d<T>&p)const{
		return x*p.x+y*p.y+z*p.z;
	}


	double getDistSQ(const Point3d<T>&p)const{
		return Point3d<T>(x-p.x,y-p.y,z-p.z).LengthSQ();
	}
	double getDist(const Point3d<T>&p)const{
		return Point3d<T>(x-p.x,y-p.y,z-p.z).Length();
	}

	bool isBetween(const Point3d<T> &p1,const Point3d<T> &p2)const
	{
		double f=p1.getDistSQ(p2);
		return (getDistSQ(p1)<f && getDistSQ(p2)<f);
	}

	Point3d<T> setLength(T len)
	{
		Normalize();
		*this *= len;
		return *this;
	}
	inline Point3d<T> invert()
	{
		x=-x;
		y=-y;
		z=-z;
		return *this;
	}

	void getAsArray(T*arr)
	{
		arr[0]=x;
		arr[1]=y;
		arr[2]=z;
		arr[3]=0;
	}



	T &operator [] (int i){
		assert(i<3);
		if(i==0)return x;
		if(i==1)return y;
		return z;
	}
	T operator [] (int i)const{
		if(i==0)return x;
		if(i==1)return y;
		if(i==2)return z;
		return 0;
	}
	Point3d<T>  operator + (const Point3d<T> &p)const
	{
		return Point3d<T>(x+p.x,y+p.y,z+p.z);
	}
	
	Point3d<T>  operator - (const Point3d<T> &p)const
	{
		return Point3d<T>(x-p.x,y-p.y,z-p.z);
	}
	Point3d<T>  operator - ()const{return Point3d<T>(-x,-y,-z);}
	Point3d<T>  operator * (T v)const{return Point3d<T>(x*v,y*v,z*v);}
	Point3d<T>  &operator = (const T v){x=v;y=v;z=v;return *this;}


	inline void set(T _x,T _y,T _z)
	{
		x=_x;
		y=_y;
		z=_z;
	}
	inline double LengthSQ()const{return x*x+y*y+z*z;}
	inline double Length()const{return sqrt((double)(x*x+y*y+z*z));}


	inline Point3d<T> crossProduct(const Point3d<T> &p)const{
		return Point3d<T>(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);
	}

	inline double Normalize()
	{
		double len=sqrt((double)(x*x+y*y+z*z));
		if(len){
			double l=1.0/len;
			x*=l;
			y*=l;
			z*=l;
		}
		return len;
	}

	Point3d<T> TranslateByAngles(const Point3d<T>& abs,const Point3d<T>& angles)
	{
		Point3d<T> out=*this;

		Point3d<bool>cb(0);
		Point3d<bool>sb(0);
		Point3d<double> cs;
		Point3d<double> sn;
		//moving on x axis
		if(abs.x){
			cs.y=cosd(angles.y);
			sn.y=sind(angles.y);
			if(angles.z){
				cs.z=cosd(angles.z);
				sn.z=sind(angles.z);
			}
			else{
				cs.z=1.0;
				sn.z=0.0;
			}
			cb.y=cb.z=true;
			sb.y=sb.z=true;

			out.x+=abs.x*cs.y*cs.z;
			out.z-=abs.x*sn.y*cs.z;
			out.y+=abs.x*sn.z;
		}
		//moving on y axis
		if(abs.y){
			if(!cb.x){
				cs.x=cosd(angles.x);
				cb.x=true;
			}
			if(!cb.y){
				cs.y=cosd(angles.y);
				cb.y=true;
			}
			if(!sb.x){
				sn.x=sind(angles.x);
				sb.x=true;
			}
			if(!sb.y){
				sn.y=sind(angles.y);
				sb.y=true;
			}
			out.x-=abs.y*sn.y*sn.x;
			out.z-=abs.y*cs.y*sn.x;
			out.y+=abs.y*cs.x;
			/*
			out.x-=abs.y*sind(angles.x)*sind(angles.y);
			out.z-=abs.y*sind(angles.x)*cosd(angles.y);
			out.y+=abs.y*cosd(angles.x);
			*/
		}
		//moving on z axis
		if(abs.z){
			if(!cb.x){
				cs.x=cosd(angles.x);
			}
			if(!cb.y){
				cs.y=cosd(angles.y);
			}
			if(!sb.x){
				sn.x=sind(angles.x);
			}
			if(!sb.y){
				sn.y=sind(angles.y);
			}
			out.x+=abs.z*sn.y*cs.x;
			out.z+=abs.z*cs.y*cs.x;
			out.y+=abs.z*sn.x;
		}
		return out;
	}



	Point3d<T> rotateX(double Deg,const Point3d<T> &center=Point3d<T>(0,0,0))
	{
		Deg=toRad(Deg);
		float cs=cos(Deg);
		float sn=sin(Deg);
		y-=center.y;
		z-=center.z;
		*this=Point3d<T>(x,y*cs-z*sn,y*sn+z*cs);
		y+=center.y;
		z+=center.z;
		return *this;
	}
	Point3d<T> rotateY(double Deg,const Point3d<T> &center=Point3d<T>(0,0,0)){
		Deg=toRad(Deg);
		float cs=cos(Deg);
		float sn=sin(Deg);
		x-=center.x;
		z-=center.z;
		*this=Point3d<T>(x*cs-z*sn,y,x*sn+z*cs);
		x+=center.x;
		z+=center.z;
		return *this;
	}
	Point3d<T> rotateZ(double Deg,const Point3d<T> &center=Point3d<T>(0,0,0))
	{
		Deg=toRad(Deg);
		float cs=cos(Deg);
		float sn=sin(Deg);
		x-=center.x;
		y-=center.y;
		*this=Point3d<T>(x*cs-y*sn,y*cs+x*sn,z);
		x+=center.x;
		y+=center.y;
		return *this;
	}

	double getAngleBetween(const Point3d<T>&o)const
	{
		double lengthQ=Length()*o.Length();
		if(lengthQ<0.001)lengthQ=0.001;
		float f=this->dotProduct(o)/lengthQ;
		if(f>1)f=1;
		else if(f<-1)f=-1;
		return acos(f);
		

	}

	Point3d<T> getHorizontalAngle() const
	{
		Point3d<T> angle;

		angle.y=(T)atan2(x,z);
		angle.y*=(float)GRAD_PI64;
		
		if(angle.y<0)angle.y+=360;
		if(angle.y>=360)angle.y-=360;

		float z1=(float)sqrt(x*x + z*z);

		angle.x=(T)atan2(z1,y);
		angle.x*=(float)GRAD_PI64;
		//angle.x-=90;


		if(angle.x<0)angle.x+=360;
		if(angle.x>=360)angle.x-=360;

		return angle;
	}

	
	Point3d<T> getVecAngle(const Point3d<T>& vecTo)const
	{
		Point3d<T> vec=*this - vecTo;
		vec.Normalize();
		Point3d<T> angles;

		angles.x=-asin(vec.y);
		float cs=cos(angles.x);

		if(cs!=0){
			float ang=vec.x/cs;
			if(abs(ang)>1)
				ang=ang>0 ? 1 : -1;
			angles.y=-asin(ang);
			if(vec.z>0)
				angles.y=PI64-angles.y;
		}
		angles.x=toDeg(angles.x);
		angles.y=toDeg(angles.y);
		angles.z=0;
		return angles;
	}

	static const Point3d<T> Zero;
	static const Point3d<T> XAxis;
	static const Point3d<T> YAxis;
	static const Point3d<T> ZAxis;

};
template <class T>
const Point3d<T> Point3d<T>::Zero=Point3d<T>(0,0,0);
template <class T>
const Point3d<T> Point3d<T>::XAxis=Point3d<T>(1,0,0);
template <class T>
const Point3d<T> Point3d<T>::YAxis=Point3d<T>(0,1,0);
template <class T>
const Point3d<T> Point3d<T>::ZAxis=Point3d<T>(0,0,1);

typedef Point3d<int> Point3di;
typedef Point3d<float> Point3df;

typedef Point3d<int> vector3di;
typedef Point3d<float> vector3df;

typedef Point3d<float> vector3d;

/*
template <class S,class T>
Point3d<T> operator *(const S s,Point3d<T> &p){return p*s;}
template <class S,class T>
Point3d<T> operator +(const S s,Point3d<T> &p){return p+s;}
template <class S,class T>
Point3d<T> operator -(const S s,Point3d<T> &p){return (-p)-(-s);}
*/


};//math
};//mray

#endif