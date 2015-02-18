
#include "QuaternionSpline.h"

namespace mray{
namespace math{

QuaternionSpline::QuaternionSpline(){
	m_isCalculated=true;
}
QuaternionSpline::~QuaternionSpline(){
}

void QuaternionSpline::createCurve(){
	recalcTangents();
}
void QuaternionSpline::createCurve(quaternion *points,int count,int curveSubD){
	if(count<=0)return;
	m_ControlPoints.resize(count);
	for(int i=0;i<count;++i){
		m_ControlPoints[i]=points[i];
	}

	recalcTangents();
	_UpdatePoints();
}
void QuaternionSpline::addPoint(quaternion p,bool recalc){
	m_isCalculated=false;
	ICurve::addPoint(p,recalc);
}

quaternion QuaternionSpline::getPoint(float t,int curveSubD){
	if(m_ControlPoints.size()<0)return quaternion::Identity;

	float pT=t*(m_ControlPoints.size());

	int index=(int)pT;
	pT=pT-index;
	return interpolate(index,pT,true);
}
quaternion QuaternionSpline::getTangent(float t,int curveSubD){
	if(!m_isCalculated)recalcTangents();
	if(m_tangents.size()==0)return quaternion::Identity;

	float pT=t*(m_tangents.size()-1);

	unsigned int index=(unsigned int)pT;
	pT=pT-index;
	if(index+1>=m_tangents.size())return m_tangents[index];
	if(pT==0)
		return m_tangents[index];
	else if(pT==1)
		return m_tangents[index+1];
	quaternion res;
	res.Slerp(m_tangents[index],m_tangents[index+1],pT);
	return res;
}

quaternion QuaternionSpline::interpolate(int index,float t,bool useShortIndex){
	if(!m_isCalculated)recalcTangents();
	if(!m_ControlPoints.size())return quaternion::Identity;
	if(index>=m_ControlPoints.size())
		return m_ControlPoints[m_ControlPoints.size()-1];
	if(index<0)
		return m_ControlPoints[0];
	if(m_tangents.size()==0)
		return m_ControlPoints[index];

	if(index+1==m_ControlPoints.size())
		return m_ControlPoints[index];
	if(t==0)
		return m_ControlPoints[index];
	else if(t==1)
		return m_ControlPoints[index+1];

	quaternion &q1=m_ControlPoints[index];
	quaternion &q2=m_ControlPoints[index+1];

//	quaternion &t1=m_tangents[index];
//	quaternion &t2=m_tangents[index+1];

	quaternion res;
	//res.Squad(t,q1,t1,q2,t2,useShortIndex);
	res.Slerp(q1,q2,t);
	return res;
}

void QuaternionSpline::recalcTangents(){
	// ShoeMake (1987) approach
	// Just like Catmull-Rom really, just more gnarly
	// And no, I don't understand how to derive this!
	//
	// let p = point[i], pInv = p.Inverse
	// tangent[i] = p * exp( -0.25 * ( log(pInv * point[i+1]) + log(pInv * point[i-1]) ) )
	//
	// Assume endpoint tangents are parallel with line with neighbour

	unsigned int i, numPoints;
	bool isClosed;

	numPoints = (unsigned int)m_ControlPoints.size();

	m_isCalculated=true;

	if (numPoints < 2)
	{
		// Can't do anything yet
		return;
	}

	m_tangents.resize(numPoints);

	if (m_ControlPoints[0] == m_ControlPoints[numPoints-1])
	{
		isClosed = true;
	}
	else
	{
		isClosed = false;
	}

	quaternion invp, part1, part2, preExp;
	for(i = 0; i < numPoints; ++i)
	{
		quaternion &p = m_ControlPoints[i];
		invp = p.inverse();

		if (i ==0)
		{
			// special case start
			part1 = (invp * m_ControlPoints[i+1]).log();
			if (isClosed)
			{
				// Use numPoints-2 since numPoints-1 == end == start == this one
				part2 = (invp * m_ControlPoints[numPoints-2]).log();
			}
			else
			{
				part2 = (invp * p).log();
			}
		}
		else if (i == numPoints-1)
		{
			// special case end
			if (isClosed)
			{
				// Wrap to [1] (not [0], this is the same as end == this one)
				part1 = (invp * m_ControlPoints[1]).log();
			}
			else
			{
				part1 = (invp * p).log();
			}
			part2 = (invp * m_ControlPoints[i-1]).log();
		}
		else
		{
			part1 = (invp * m_ControlPoints[i+1]).log();
			part2 = (invp * m_ControlPoints[i-1]).log();
		}

		preExp = (part1 + part2)*(-0.25);
		m_tangents[i] = p * preExp.exp();

	}


}

}
}
