
/********************************************************************
	created:	2009/03/12
	created:	12:3:2009   23:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\QuaternionSpline.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	QuaternionSpline
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___QuaternionSpline___
#define ___QuaternionSpline___

#include "ICurve.h"
#include "quaternion.h"

namespace mray{
namespace math{

class QuaternionSpline:public ICurve<quaternion>
{
protected:
	std::vector<quaternion> m_tangents;
	std::vector<quaternion> m_points;
	bool m_isCalculated;
public:
	QuaternionSpline();
	virtual~QuaternionSpline();

	virtual void addPoint(quaternion p,bool recalc=false);
	virtual void createCurve();
	virtual void createCurve(quaternion *points,int count,int curveSubD);

	virtual quaternion getPoint(float t,int curveSubD=-1);
	virtual quaternion getTangent(float t,int curveSubD=-1);

	quaternion interpolate(int index,float t,bool useShortIndex);

	virtual void recalcTangents();
	
};

}
}


#endif //___QuaternionSpline___
