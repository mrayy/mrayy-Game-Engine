
#include "stdafx.h"
#include "DirectedConnector.h"
#include "IConnector.h"


namespace mray
{
namespace canvas
{

DirectedConnector::DirectedConnector(CanvasManager*m):ConnectionCurve(m)
{
}
DirectedConnector::~DirectedConnector()
{
}

bool DirectedConnector::_updateCurve()
{
	if(!ConnectionCurve::_updateCurve())return false;


	math::vector2d dNorm=m_destination->GetConnectionNormal();
	math::vector2d dBin(-dNorm.y,dNorm.x);

	m_points.push_back(m_lastDestPos);
	m_points.push_back(m_lastDestPos+(dNorm+dBin*0.5)*10);
	m_points.push_back(m_lastDestPos);

	m_points.push_back(m_lastDestPos+(dNorm-dBin*0.5)*10);

	return true;
}

}
}


