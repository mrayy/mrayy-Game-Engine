


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   12:05
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\CanvasUtilities.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	CanvasUtilities
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CanvasUtilities__
#define __CanvasUtilities__

#include "CanvasCommon.h"

namespace mray
{
namespace canvas
{

class CanvasUtilities
{
private:
protected:
public:

	static math::rectf CalculateBoundingRect(const PointList& points);
	static void CalculatePointsRatio(const PointList& points,const math::rectf& rc,std::vector<math::vector2d>& ratio);
	static void CalculateTextureCoords(const PointList& points,const math::rectf& texCoords,std::vector<math::vector2d> &outCoords);
	
	//pass 4 colors which represents the 4 corners of the shape
	static void CalculatePointColor(const PointList& points,const video::SColor* clr,std::vector<video::SColor> &outClrs);
	static void BuildCurve(const PointList& inPoints,PointList& outPoints,int subD=4,bool bezier=false);

	//////////////////////////////////////////////////////////////////////////
	static void GenerateRectangle(const math::vector2d& size,PointList& outPoints);
	static void GenerateChamferedRectangle(const math::vector2d& size,float ratio,PointList& outPoints);
	static void GenerateEllipse(const math::vector2d& size,int segments,PointList& outPoints);
	static void GenerateCircle(float radius,int segments,PointList& outPoints);
};

}
}

#endif
