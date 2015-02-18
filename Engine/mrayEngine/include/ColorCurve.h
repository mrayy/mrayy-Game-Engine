



/********************************************************************
	created:	2012/01/08
	created:	8:1:2012   11:55
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ColorCurve.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ColorCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ColorCurve__
#define __ColorCurve__

#include "CompileConfig.h"
#include "ICurve.h"

namespace mray
{
namespace video
{

class MRAY_DLL ColorCurve
{
private:
protected:
	std::vector<math::vector2d> m_cvs;
	math::ICurve<math::vector2d>* m_curve;
	void _RecalcCurve();
public:
	ColorCurve();
	virtual~ColorCurve();

	void AddPoint(float x,float y);

	math::vector2d GetValue(float t)const;

};

}
}

#endif
