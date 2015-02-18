

#ifndef ___MATHFUNCTIONS___
#define ___MATHFUNCTIONS___

#include <math.h>
#include <float.h>
//#include "mTypes.h"

namespace mray{
namespace math{


const float Epsilon =1e-05f;
const double Zero = 1e-08f;
const float Infinity = FLT_MAX;

const float RoundingValue	= 0.00005f;
const double PI64			= 3.1415926535897932384626433832795028841971693993751;
const double TwoPI64			=	PI64*2.0;
const double HalfPI64		=	PI64*0.5;
const double OneFourthPI64	=	PI64*0.25;

const float PI32			=	3.141592f;
const float TwoPI32			=	PI32*2;
const float HalfPI32		=	PI32*0.5f;
const float OneFourthPI32	=	PI32*0.25f;

#define RANDOM_FLOAT	((float)rand()/RAND_MAX)

const double GRAD_PI64 = 180.0 / PI64;
const float GRAD_PI32 = 180.0f / PI32;

const float i255 = 1.0f/255.0f;

inline double toRad(double a){return a*PI64/180;}
inline double toDeg(double a){return a*180/PI64;}

 
class __declspec(dllimport) MTrigTable
{
	static bool m_initlized;
	static unsigned short m_trigTableSize;
	static double m_trigTableFactor;

	static double *m_sinTable;
	static double *m_tanTable;

	static double sinTable(double angle);
	static double tanTable(double angle);

	static bool m_useTable;
	void buildTables();
public:

	MTrigTable(unsigned short tableSize=4096);
	~MTrigTable();

	static void useTable(bool use);
	static bool isUseTable();

	static inline double sin(double angle){	return sinTable(angle); }
	static inline double cos(double angle){	return sinTable(angle+0.5f*PI32);}
	static inline double tan(double angle){	return tanTable(angle);}

};

inline double cosd(double a){
	return MTrigTable::cos(toRad(a));
}
inline double sind(double a){
	return MTrigTable::sin(toRad(a));
}
inline double tand(double a){
	return MTrigTable::tan(toRad(a));
}

inline double cosr(double a){
	return MTrigTable::cos(a);
}
inline double sinr(double a){
	return MTrigTable::sin(a);
}
inline double tanr(double a){
	return MTrigTable::tan(a);
}


template <class T>
inline T Max(const T a,const T b)
{
	return  a<b ? b:a;
}
template <class T>
inline T Min(const T a,const T b)
{
	return a<b ? a:b;
}

template <class T>
inline T sqr(const T a)
{
	return  a * a;
}



inline bool isZero(float val){
	return (val<math::Zero && val>-math::Zero);
}

template <class T>
inline int sign(T x){
	return x<0 ? -1 : 1;
}
/*
float roundNumber(float x,float a,float b)
{
	bool flag=0;
	if(b==0)
	{
		flag=1;
		b+=10;
		a+=10;
	}
	if(x<a)
	{
		float tmp=a-x;
		tmp=tmp-b*(int)(tmp/b);
		x=b-tmp;
	}
	if(x>=b)
		x=x-b*(int)(x/b);
	if(flag)
		x-=10;
	return x;
}
*/
template <class T>
void Swap(T &a,T &b)
{
	T tmp;
	tmp=a;
	a=b;
	b=tmp;
}

inline bool fEqual(float v1,float v2,float epsilon=math::RoundingValue)
{
	return (v1+epsilon>v2 && v1-epsilon<v2);
}


template <class T>
inline T lerp(T a,T b,float t)
{
	return (T)(a*-(t-1)) + b*(t);
}
template <class T>
inline T clamp(T x,T low,T high)
{
	return Min(Max(x,low),high);
}
template <class T>
inline T cycle(T x,T a,T b)
{
	return (x-a)<0 ? (b+x-a) : (x>=b) ? (x-b+a) : x;
}
template <class T>
inline T ang(T a)
{
	a=fmod((double)a,(double)360);
	int s=a>=0 ? +1 : -1;
	if(abs((double)a)>180)
		return a-s*360;
	return a;
}

template <class T>
bool getValFromFrame(T*arr,int arrSize,float frame,int sframe,int eframe,T&out)
{
	if(!arr || frame<0  || sframe<0 || eframe<0||
		sframe>=arrSize || eframe>=arrSize)
		return false;
	float Alpha=frame/100.0f;
	float AlphaInv=1-Alpha;
	
	out=arr[eframe]*Alpha+arr[sframe]*AlphaInv;
	return 1;
}

template <class T>
bool getBlendValFromFrame(T*arr,int arrSize,float frame,int sframe,int eframe,
						  float blendAmount,int blendFrame,T&out)
{
	if(!arr || frame<0  || sframe<0 || eframe<0|| blendFrame<0 ||
		sframe>=arrSize || eframe>=arrSize|| blendFrame>=arrSize||
		blendAmount<0  
		)
		return false;
	float BAlpha=blendAmount/100.0f;
	float BAlphaInv=1-BAlpha;

	float FAlpha=frame/100.0f;
	float FAlphaInv=1-FAlpha;
	
	out=(arr[eframe] *FAlpha+arr[sframe]*FAlphaInv)*BAlphaInv+
		arr[blendFrame]*BAlpha;
	return 1;
}



};//math
};//mray

#endif