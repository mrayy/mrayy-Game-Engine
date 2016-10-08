

/********************************************************************
	created:	2013/03/08
	created:	8:3:2013   9:23
	filename: 	D:\Development\mrayEngine\Engine\mrayMath\LinearCurve.h
	file path:	D:\Development\mrayEngine\Engine\mrayMath
	file base:	LinearCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _LinearCurve_
#define _LinearCurve_

#include "ICurve.h"


namespace mray
{
namespace math
{
	template<class T>
class LinearCurve:public ICurve<T>
{
protected:
public:
	LinearCurve(){}
	virtual~LinearCurve(){}
	virtual void createCurve()
	{
		int count=m_ControlPoints.size();
		T*points=&m_ControlPoints[0];

		m_points.resize(count);

		for(int i=0;i<count;++i){
			m_points[i]=points[i];
		}
	}
	virtual void createCurve(const T *points,int count,int curveSubD)
	{
		if(count<=0)return;

		m_ControlPoints.resize(count);
		memcpy(&m_ControlPoints[0],points,sizeof(T)*count);

		//createCurve();
		_UpdatePoints();
	}
	virtual T getPoint(float t,int curveSubD=-1)
	{
#define BOUNDS(pp) {if(pp<0)pp=0; else if(pp>=m_ControlPoints.size()) pp=m_ControlPoints.size()-1;}

		t=math::clamp(t,0.f,1.f);
		int p=t*(m_ControlPoints.size()-1);
		float lt=(t-p/(float)(m_ControlPoints.size()-1))*(m_ControlPoints.size()-1); //compute time fraction

		int p0=p   ;BOUNDS(p0);
		int p1=p+1;BOUNDS(p1);
		return m_ControlPoints[p0]*(1-lt)+m_ControlPoints[p1]*lt;
	}
	virtual T getTangent(float t,int curveSubD=-1)
	{
		return 0;
	}
};


}
}

#endif
