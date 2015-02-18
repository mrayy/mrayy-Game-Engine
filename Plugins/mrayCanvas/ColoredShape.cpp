
#include "stdafx.h"
#include "ColoredShape.h"
#include "ICanvasObject.h"
#include "CanvasManager.h"
#include "CanvasUtilities.h"
#include "DefaultColors.h"


namespace mray
{
namespace canvas
{

ColoredShape::ColoredShape()
{
	m_cornersColor[0]=video::DefaultColors::White;
	m_cornersColor[1]=video::DefaultColors::White;
	m_cornersColor[2]=video::DefaultColors::Gray;
	m_cornersColor[3]=video::DefaultColors::Gray;
}
ColoredShape::~ColoredShape()
{
}

void ColoredShape::SetColors(const video::SColor* c)
{
	m_cornersColor[0]=c[0];
	m_cornersColor[1]=c[1];
	m_cornersColor[2]=c[2];
	m_cornersColor[3]=c[3];
}


void ColoredShape::Draw(EMouseStatus status)
{
	if(!m_owner || !m_points.size())
		return;

	m_owner->GetCanvasManager()->DrawShapeFilledGrad(m_points,&m_pointColors[0]);
	ICShape::Draw(status);
}

void ColoredShape::SetShapePoints(const PointList &points)
{
	ICShape::SetShapePoints(points);
	CanvasUtilities::CalculatePointColor(points,m_cornersColor,m_pointColors);
}

const std::vector<video::SColor>& ColoredShape::GetPointColors()
{
	return m_pointColors;
}

}
}