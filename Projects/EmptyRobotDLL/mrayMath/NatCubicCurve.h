


/********************************************************************
	created:	2012/01/08
	created:	8:1:2012   13:51
	filename: 	d:\Development\mrayEngine\Engine\mrayMath\NatCubicCurve.h
	file path:	d:\Development\mrayEngine\Engine\mrayMath
	file base:	NatCubicCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __NatCubicCurve__
#define __NatCubicCurve__

#include "ICurve.h"

namespace mray
{
namespace math
{

template <class T>
class NatCubicCurve:public ICurve<T>
{
	struct CubicSegment
	{
		T a,b,c,d;
		CubicSegment(){}
		CubicSegment(const T& v0,const T& v1,const T& v2,const T& v3)
			:a(v0),b(v1),c(v2),d(v3)
		{
		}

		T eval(float t)
		{
			return a + t*(b + t*(c + t*d));
		}
	};
private:
protected:
	int m_curveSubD;
	std::vector<CubicSegment> m_segments;
public:
	NatCubicCurve():m_curveSubD(0)
	{}
	virtual~NatCubicCurve(){}

	virtual void createCurve()
	{
		std::vector<float> gamma;
		std::vector<T> delta;
		std::vector<T> D;

		int n=m_ControlPoints.size()-1;
		if(n<2)
			return;
		T* x=&m_ControlPoints[0];

		gamma.resize(n+1);
		delta.resize(n+1);
		D.resize(n+1);

		gamma[0]=0.5f;
		for(int i=1;i<n;++i)
		{
			gamma[i]=1.0f/(4.0f-gamma[i-1]);
		}
		gamma[n]=1.0f/(2.0 - gamma[n-1]);

		delta[0]=3.0f*(x[1]-x[0])*gamma[0];
		for(int i=1;i<n;++i)
		{
			delta[i]=(3.0f*(x[i+1]-x[i-1])-delta[i-1])*gamma[i];
		}
		delta[n]=(3.0f*(x[n]-x[n-1])-delta[n-1])*gamma[n];

		D[n]=delta[n];
		for(int i=n-1;i>=0;--i)
		{
			D[i]=delta[i]-gamma[i]*D[i+1];
		}

		m_segments.resize(n);
		for(int i=0;i<n;++i)
		{
			m_segments[i]=CubicSegment(x[i],D[i],3.0f*(x[i+1]-x[i  ])-2.0f*D[i]-D[i+1],
												 2.0f*(x[i  ]-x[i+1])+     D[i]+D[i+1]);
		}
	}
	virtual void createCurve(const T *points,int count,int curveSubD)
	{
		m_curveSubD=curveSubD;
		if(m_curveSubD<=0)m_curveSubD=1;


		if(count<=0)return;
		m_ControlPoints.resize(count);
		memcpy(&m_ControlPoints[0],points,sizeof(T)*count);
		createCurve();
		_UpdatePoints();
	}
	virtual T getPoint(float t,int curveSubD=-1)
	{
		if(m_segments.size()==0)
			return T();
		if(curveSubD<=0)curveSubD=m_curveSubD;

		t=math::clamp(t,0.f,1.f);

		int seg=t*m_segments.size();
		if(seg==m_segments.size())
			--seg;
		float startT=(float)seg/(float)m_segments.size();
		t=(t-startT)*(float)m_segments.size();

		return m_segments[seg].eval(t);
	}
	virtual T getTangent(float t,int curveSubD=-1)
	{
		return T();
	}
};


}
}

#endif