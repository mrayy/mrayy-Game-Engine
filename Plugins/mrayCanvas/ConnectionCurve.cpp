

#include "stdafx.h"
#include "ConnectionCurve.h"
#include "IConnector.h"
#include "CanvasUtilities.h"
#include "CanvasManager.h"
#include "DefaultColors.h"


namespace mray
{
namespace canvas
{

ConnectionCurve::ConnectionCurve(CanvasManager*m)
:IConnectionCurve(m),m_dirty(false),m_selected(0)
{
}
ConnectionCurve::~ConnectionCurve()
{
}

void ConnectionCurve::SetSource(IConnector* s)
{
	if(s && !m_source)
		m_dirty=true;
	IConnectionCurve::SetSource(s);
	_updateCurve();
}
void ConnectionCurve::SetDestination(IConnector* s)
{
	if(s && !m_destination)
		m_dirty=true;

	IConnectionCurve::SetDestination(s);
	_updateCurve();
}
void ConnectionCurve::Draw()
{
	if(!m_source || !m_destination)
		return;
	_updateCurve();
	m_canvasManager->DrawLines(m_points,video::DefaultColors::Black,m_selected?2:1);

}

EMouseStatus ConnectionCurve::ProcessMouse(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	if(evt!=MET_LEFTDOWN)
		return ENone;
	const float minDist=2;
	for(int i=0;i<m_points.size()-1;++i)
	{
		math::vector2d v=(m_points[i+1]-m_points[i]);
		float len=v.Length();
		v=v/len;
		math::vector2d v2=pt-m_points[i];
		float d=v.dotProduct(v2);
		if(d>-minDist && d<=len-minDist)
		{
			math::vector2d v3(-v.y,v.x);
			if(abs(v2.dotProduct(v3))<=minDist)
			{
				m_selected=true;
				return EClicked;
			}
		}
	}
	return ENone;
}
bool ConnectionCurve::_updateCurve()
{
	if(!m_source || !m_destination)
	{
		m_points.clear();
		return false;
	}
	math::vector2d spos=m_source->GetConnectionPoint();
	math::vector2d dpos=m_destination->GetConnectionPoint();
	if(spos==m_lastSourcePos && dpos==m_lastDestPos && !m_dirty)
	{
		return false;
	}
	m_lastSourcePos=spos;
	m_lastDestPos=dpos;
	m_dirty=true;

	math::vector2d sNorm=m_source->GetConnectionNormal();
	math::vector2d dNorm=m_destination->GetConnectionNormal();

	PointList CVs;
	CVs.push_back(spos);
	CVs.push_back(spos);
	CVs.push_back(spos+sNorm*30);
	CVs.push_back(dpos+dNorm*30);
	CVs.push_back(dpos);
	CVs.push_back(dpos);

	CanvasUtilities::BuildCurve(CVs,m_points,3,false);
	m_points.push_back(dpos);

	m_dirty=false;
	return true;
}


}
}

