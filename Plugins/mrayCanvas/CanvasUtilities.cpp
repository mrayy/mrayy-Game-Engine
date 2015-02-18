
#include "stdafx.h"
#include "CanvasUtilities.h"
#include "CSpline.h"
#include "CBezierCurve.h"

namespace mray
{
namespace canvas
{

math::rectf CanvasUtilities::CalculateBoundingRect(const PointList& points)
{
	math::rectf rc;
	if(points.size()==0)return rc;
	rc.reset(points[0]);
	for(int i=1;i<points.size();++i)
	{
		rc.addPoint(points[i]);
	}
	return rc;
}
void CanvasUtilities::CalculatePointsRatio(const PointList& points,const math::rectf& rc,std::vector<math::vector2d>& ratio)
{
	math::vector2d inv(1.0f/rc.getWidth(),1.0f/rc.getHeight());


	for(int i=0;i<points.size();++i)
	{
		ratio.push_back((points[i]-rc.ULPoint)*inv);
	}
}

void CanvasUtilities::CalculateTextureCoords(const PointList& points,const math::rectf& texCoords,std::vector<math::vector2d> &outCoords)
{
	math::rectf BB=CalculateBoundingRect(points);
	std::vector<math::vector2d> r;
	CalculatePointsRatio(points,BB,r);
	outCoords.resize(points.size());
	for(int i=0;i<points.size();++i)
	{
		math::vector2d inv(1-r[i].x,1-r[i].y);
		outCoords[i]=inv*texCoords.ULPoint+r[i]*texCoords.BRPoint;
	}
}
void CanvasUtilities::CalculatePointColor(const PointList& points,const video::SColor* clr,std::vector<video::SColor> &outClrs)
{
	math::rectf BB=CalculateBoundingRect(points);
	std::vector<math::vector2d> r;
	CalculatePointsRatio(points,BB,r);
	outClrs.resize(points.size());
	for(int i=0;i<points.size();++i)
	{
		video::SColor l1=math::lerp(clr[0],clr[1],r[i].x);
		video::SColor l2=math::lerp(clr[2],clr[3],r[i].x);
		outClrs[i]=math::lerp(l1,l2,r[i].y);
	}
}
void CanvasUtilities::BuildCurve(const PointList& inPoints,PointList& outPoints,int subD,bool bezier)
{
	if(!bezier)
	{
		math::CSpline<math::vector2d> curve(&inPoints[0],inPoints.size(),subD);
		curve.createCurve();
		int cnt=curve.getPointsCount();
		math::vector2d*points=curve.getCurvePoints();
		outPoints.resize(cnt);
		for(int i=0;i<cnt;++i)
		{
			outPoints[i]=points[i];
		}
	}else
	{
		math::CBezierCurve<math::vector2d> curve(&inPoints[0],inPoints.size(),subD);
		curve.createCurve();
		int cnt=curve.getPointsCount();
		math::vector2d*points=curve.getCurvePoints();
		outPoints.resize(cnt);
		for(int i=0;i<cnt;++i)
		{
			outPoints[i]=points[i];
		}
	}
}

void CanvasUtilities::GenerateRectangle(const math::vector2d& size,PointList& outPoints)
{
	outPoints.resize(4);
	outPoints[0].set(-size.x*0.5f,-size.y*0.5f);
	outPoints[1].set(size.x*0.5f,-size.y*0.5f);
	outPoints[2].set(size.x*0.5f,size.y*0.5f);
	outPoints[3].set(-size.x*0.5f,size.y*0.5f);
}
void CanvasUtilities::GenerateChamferedRectangle(const math::vector2d& size,float ratio,PointList& outPoints)
{
	float minSz=math::Min(size.x,size.y)*0.5;
	ratio=math::Min(minSz,math::Max(0.0f,ratio));
	PointList points;
	PointList points1;

	math::vector2d v1=ratio;//*rc.getSize();
	math::vector2d v2=(size-ratio);//*rc.getSize();

	math::vector2d halfSize=size*0.5;
	math::vector2d negHalfSize(-halfSize.x,-halfSize.y);
	points1.push_back(negHalfSize);
	points1.push_back(negHalfSize+math::vector2d(v1.x,0));
	points1.push_back(negHalfSize+math::vector2d(v2.x,0));
	points1.push_back(negHalfSize+math::vector2d(size.x,0));
	points1.push_back(negHalfSize+math::vector2d(size.x,v1.y));
	points1.push_back(negHalfSize+math::vector2d(size.x,v2.y));
	points1.push_back(halfSize);
	points1.push_back(math::vector2d(negHalfSize.x+v2.x,halfSize.y));
	points1.push_back(math::vector2d(negHalfSize.x+v1.x,halfSize.y));
	points1.push_back(math::vector2d(negHalfSize.x,halfSize.y));
	points1.push_back(negHalfSize+math::vector2d(0,v2.y));
	points1.push_back(negHalfSize+math::vector2d(0,v1.y));
	

	points.push_back(points1[11]);
	points.push_back(points1[11]);
	points.push_back(points1[0]);
	points.push_back(points1[1]);
	points.push_back(points1[1]);
	points.push_back(points1[2]);
	points.push_back(points1[2]);
	points.push_back(points1[3]);
	points.push_back(points1[4]);
	points.push_back(points1[4]);
	points.push_back(points1[5]);
	points.push_back(points1[5]);
	points.push_back(points1[6]);
	points.push_back(points1[7]);
	points.push_back(points1[7]);
	points.push_back(points1[8]);
	points.push_back(points1[8]);
	points.push_back(points1[9]);
	points.push_back(points1[10]);
	points.push_back(points1[10]);
	points.push_back(points1[11]);
	points.push_back(points1[11]);
	points.push_back(points1[0]);


	BuildCurve(points,outPoints,4);

}

void CanvasUtilities::GenerateEllipse(const math::vector2d& size,int segments,PointList& outPoints)
{
	int steps=segments;//4*(1+4);
	float angleStep=math::toRad(360.0f/(float)steps);

	math::vector2d hSize=size/2.0f;
	outPoints.resize(steps);
	float angle=0;
	for(int i=0;i<steps;++i)
	{
		float cs=hSize.x*math::cosr(angle);
		float sn=hSize.y*math::sinr(angle);
		outPoints[i].set(cs,sn);
		angle+=angleStep;
	}
}
void CanvasUtilities::GenerateCircle(float radius,int segments,PointList& outPoints)
{
	GenerateEllipse(radius,segments,outPoints);
}


}
}