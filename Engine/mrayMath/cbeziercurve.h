



#ifndef ___CBezierCurve___
#define ___CBezierCurve___

#include "Point3d.h"
#include "mArray.h"

#include "ICurve.h"
#include "CompileConfig.h"
#include "mCurves.h"

namespace mray{
namespace math{


template <class T>
class CBezierCurve:public ICurve<T>
{
	//int m_curveSubD;
	//std::vector<T> m_points;
public:

	CBezierCurve(const T *points,int count,int curveSubD){
		createCurve(points,count,curveSubD);
	}
	virtual~CBezierCurve(){
		m_points.clear();
	}

	virtual void createCurve(){

		int index=0;
		int count=m_ControlPoints.size();
		T*points=&m_ControlPoints[0];
		if(count<4)return;

		m_points.resize((count-3)*m_curveSubD);

		float csdinv=1.0f/(float)m_curveSubD;

		T v[4];
		for(int i=0;i<count-3;++i){
			v[0]=points[i];
			v[1]=points[i+1];
			v[2]=points[i+2];
			v[3]=points[i+3];

			for(int j=0;j<m_curveSubD;j++){
				m_points[index]=cubicBezier<T>(j*csdinv,v[0],v[1],v[2],v[3]);
				index++;
			}
		}

	}

	virtual void createCurve(const T *points,int count,int curveSubD){

		m_curveSubD=curveSubD;
		if(m_curveSubD<=0)m_curveSubD=1;

		if(count<=0)return;

		m_ControlPoints.resize(count);
		memcpy(&m_ControlPoints[0],points,sizeof(T)*count);

		_UpdatePoints();
		//createCurve();
	}

	
	virtual T getPoint(float t,int curveSubD){
		if(m_ControlPoints.size()<4)return 0;
		if(curveSubD<=0)curveSubD=m_curveSubD;

		//t=fmod(t,100.0f)*0.01f;
		t=math::clamp(t,0.f,1.f);

		float ptr=(float)(m_ControlPoints.size()-3)*t;
		int i=(int)ptr;
		ptr-=i;

		T v[4];
		v[0]=m_ControlPoints[i];
		v[1]=m_ControlPoints[i+1];
		v[2]=m_ControlPoints[i+2];
		v[3]=m_ControlPoints[i+3];

		return cubicBezier<T>(ptr,v[0],v[1],v[2],v[3]);

	}

	virtual T getTangent(float t,int curveSubD=-1){

		if(m_ControlPoints.size()<4)return 0;
		if(curveSubD<=0)curveSubD=m_curveSubD;

		t=fmod(t,100.0f)*0.01f;

		float ptr=(float)(m_ControlPoints.size()-3)*t;
		int i=(int)ptr;
		ptr-=i;

		T v[4];
		v[0]=m_ControlPoints[i];
		v[1]=m_ControlPoints[i+1];
		v[2]=m_ControlPoints[i+2];
		v[3]=m_ControlPoints[i+3];
		return cubicBezierD<T>(ptr,v[0],v[1],v[2],v[3]);

	}
};

}
}
#endif






