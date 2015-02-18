
#include "stdafx.h"
#include "ColorCurve.h"
#include "NatCubicCurve.h"



namespace mray
{
namespace video
{

ColorCurve::ColorCurve()
{
	m_curve=new math::NatCubicCurve<math::vector2d>();
	AddPoint(0,0);
	AddPoint(0,0);
	AddPoint(1,1);
	AddPoint(1,1);
}
ColorCurve::~ColorCurve()
{
	delete m_curve;
}


void ColorCurve::_RecalcCurve()
{
	m_curve->createCurve(&m_cvs[0],m_cvs.size(),4);
}

void ColorCurve::AddPoint(float x,float y)
{
	x=math::clamp<float>(x,0,1);
	y=math::clamp<float>(y,0,1);
	std::vector<math::vector2d>::iterator it=m_cvs.begin();
	std::vector<math::vector2d>::iterator it2=it;
	for(;it!=m_cvs.end();++it)
	{
		if((*it).x>x)
		{
			m_cvs.insert(it,math::vector2d(x,y));
			_RecalcCurve();
			return;
		}
		it2=it;
	}
	m_cvs.push_back(math::vector2d(x,y));
	_RecalcCurve();
}

math::vector2d ColorCurve::GetValue(float t)const
{
	return m_curve->getPoint(t);
}

}
}
