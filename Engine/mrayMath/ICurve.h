

#ifndef ___ICurve___
#define ___ICurve___

#include "Point3d.h"

#include <vector>

namespace mray{
namespace math{

template <class T>
class ICurve{
protected:
	
	std::vector<T> m_ControlPoints;
	std::vector<T> m_points;
	int m_curveSubD;

	void _UpdatePoints()
	{
		if(m_curveSubD<1)m_curveSubD=1;
		unsigned int n=m_ControlPoints.size()*m_curveSubD;
		m_points.resize(n+1);
		float invN=1.0f/(float)n;
		for(int i=0;i<n;++i)
		{
			m_points[i]=getPoint(i*invN,m_curveSubD);
		}
		m_points[n]=getPoint(1,m_curveSubD);
	}

public:

	void clearPoints(){
		m_ControlPoints.clear();
	}
	
	virtual void createCurve()=0;
	virtual void createCurve(const T *points,int count,int curveSubD)=0;

	virtual int getPointsCount(){return m_points.size();}
	virtual T* getCurvePoints(){return &m_points[0];}

	virtual int getControlPointsCount(){return m_ControlPoints.size();}
	virtual const T* getControlCurvePoints(){return &m_ControlPoints[0];}

	virtual T getPoint(float t,int curveSubD=-1)=0;
	virtual T getTangent(float t,int curveSubD=-1)=0;

	virtual void addPoint(T p,bool recalc=false){
		m_ControlPoints.push_back(p);
		if(recalc)
			createCurve();
	}
};


}
}


#endif



