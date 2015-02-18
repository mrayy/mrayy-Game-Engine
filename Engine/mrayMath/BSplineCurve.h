

/********************************************************************
	created:	2012/01/08
	created:	8:1:2012   15:05
	filename: 	d:\Development\mrayEngine\Engine\mrayMath\BSplineCurve.h
	file path:	d:\Development\mrayEngine\Engine\mrayMath
	file base:	BSplineCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BSplineCurve__
#define __BSplineCurve__

#include "ICurve.h"

namespace mray
{
namespace math
{

	template <class T>
class BSplineCurve:public ICurve<T>
{
private:
protected:
	int m_basis;
//	std::vector<T> m_points;
public:
	BSplineCurve(int basis):m_basis(basis)
	{}
	virtual~BSplineCurve(){}

	float getB(float t)
	{
		switch(m_basis)
		{
		case -2: return (((-t+3)*t-3)*t+1)/6.0f;
		case -1: return (((3*t-6)*t)*t+4)/6.0f;;
		case  0: return (((-3*t+3)*t+3)*t+1)/6.0f;
		case  1: 
		default:
			return (t*t*t)/6.0f;
		}
	}

	virtual void createCurve()
	{/*
		
		float invD=1.0f/(float)m_subD;
		for(int i=0;i<m_ControlPoints.size();++i)
		{
			for(int j=0;j<m_subD;++j)
			{
			}
		}*/
		
	}
	virtual void createCurve(const T *points,int count,int curveSubD)
	{
		if(count<=0)return;
		m_subD=curveSubD;
		if(m_subD<=0)m_subD=1;

		m_ControlPoints.resize(count);
		memcpy(&m_ControlPoints[0],points,sizeof(T)*count);
		//createCurve();
		_UpdatePoints();
	}
	virtual T getPoint(float t,int curveSubD=-1)
	{

		t=math::clamp(t,0.f,1.f);

		int seg=t*m_ControlPoints.size();
		if(seg==m_ControlPoints.size())
			--seg;
		float startT=(float)seg/(float)m_ControlPoints.size();
		t=(t-startT)*(float)m_ControlPoints.size();

		return m_ControlPoints[seg]*getB(t);
	}
	virtual T getTangent(float t,int curveSubD=-1)
	{
		return T();
	}
};


}
}

#endif

