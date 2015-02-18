

#include "stdafx.h"
#include "IConnector.h"
#include "ICanvasObject.h"
#include "CanvasUtilities.h"
#include "CanvasManager.h"
#include "IConnectionCurve.h"
#include "DefaultColors.h"


namespace mray
{
namespace canvas
{

IConnector::IConnector():m_id(0),m_owner(0),m_relativePos(0.5),m_placement(ECP_Left),
m_bgColor(video::DefaultColors::LightBlue)
{
	math::vector2d size(10,10);
	CanvasUtilities::GenerateRectangle(size,m_points);
}
IConnector::~IConnector()
{
}

const math::rectf& IConnector::GetRect()
{
	math::vector2d size(10,10);
	math::rectf& rc=m_lastRect;
	if(!m_owner)
		return rc;
	const math::rectf& orc= m_owner->GetBoundingRect();
	switch(m_placement)
	{
	case ECP_Top:
		{
			float x=orc.ULPoint.x;
			x+=m_relativePos*orc.getWidth();
			rc.ULPoint.set(x-size.x*0.5,orc.ULPoint.y-size.y*0.5);
			rc.BRPoint.set(x+size.x*0.5,orc.ULPoint.y+size.y*0.5);
		}
		break;
	case ECP_Bot:
		{
			float x=orc.ULPoint.x;
			x+=m_relativePos*orc.getWidth();
			rc.ULPoint.set(x-size.x*0.5,orc.BRPoint.y-size.y*0.5);
			rc.BRPoint.set(x+size.x*0.5,orc.BRPoint.y+size.y*0.5);
		}
		break;
	case ECP_Left:
		{
			float y=orc.ULPoint.y;
			y+=m_relativePos*orc.getHeight();
			rc.ULPoint.set(orc.ULPoint.x-size.x*0.5,y-size.y*0.5);
			rc.BRPoint.set(orc.ULPoint.x+size.x*0.5,y+size.y*0.5);
		}
		break;
	case ECP_Right:
		{
			float y=orc.ULPoint.y;
			y+=m_relativePos*orc.getHeight();
			rc.ULPoint.set(orc.BRPoint.x-size.x*0.5,y-size.y*0.5);
			rc.BRPoint.set(orc.BRPoint.x+size.x*0.5,y+size.y*0.5);
		}
		break;
	}
	return m_lastRect;
}
void IConnector::_updateRect()
{
}
void IConnector::Draw()
{
	const math::rectf& rc=GetRect();
	math::vector2d pos=m_owner->GetPos()+rc.getCenter();
	m_owner->GetCanvasManager()->SetTransformation(pos,m_owner->GetAngle(),1);

	video::SColor clr=m_bgColor;

	IConnector* c=m_owner->GetCanvasManager()->GetSelectedConnector();
	if(c && c!=this)
	{
		if(c->Accepts(this)!=EAR_No)
			clr=video::DefaultColors::Green;
		else
			clr=video::DefaultColors::Red;
	}
	
	m_owner->GetCanvasManager()->DrawShapeFilled(m_points,clr);
	m_owner->GetCanvasManager()->DrawShapeWired(m_points,video::DefaultColors::Black,1);

	m_owner->GetCanvasManager()->SetTransformation(pos,m_owner->GetAngle(),1);
}
bool IConnector::ProcessMouse(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	return false;
}

void IConnector::OnConnected(IConnector* other)
{
	m_owner->ConnectorConnected(this,other);
}
void IConnector::AddConnection(IConnectionCurve* other)
{
	m_connections.push_back(other);
}
bool IConnector::HasConnection(IConnector* other)
{
	std::list<IConnectionCurve*>::iterator it=m_connections.begin();
	for(;it!=m_connections.end();++it)
	{
		IConnectionCurve* curve=*it;
		if(curve->GetSource()==other || curve->GetDestination()==other)
		{
			return true;
		}
	}
	return false;
}
void IConnector::RemoveConnection(IConnector* other)
{
	std::list<IConnectionCurve*>::iterator it=m_connections.begin();
	for(;it!=m_connections.end();++it)
	{
		IConnectionCurve* curve=*it;
		if(curve->GetSource()==other || curve->GetDestination()==other)
		{
			other->OnConnectionRemoved(curve);
			m_owner->GetCanvasManager()->RemoveCurve(curve);
			delete curve;
			m_connections.erase(it);
			break;
		}
	}
}
void IConnector::ClearConnections()
{
	std::list<IConnectionCurve*>::iterator it=m_connections.begin();
	for(;it!=m_connections.end();++it)
	{
		IConnectionCurve* curve=*it;
		m_owner->GetCanvasManager()->RemoveCurve(curve);
		if(curve->GetSource()!=this)
		{
			curve->GetSource()->OnConnectionRemoved(curve);
		}else
			curve->GetDestination()->OnConnectionRemoved(curve);
		delete curve;
	}
	m_connections.clear();
}
void IConnector::OnConnectionRemoved(IConnectionCurve* other)
{
	std::list<IConnectionCurve*>::iterator it=m_connections.begin();
	for(;it!=m_connections.end();++it)
	{
		IConnectionCurve* curve=*it;
		if(curve==other)
		{
			m_connections.erase(it);
			break;
		}
	}
}

bool IConnector::IsPointIn(const math::vector2d& pt)
{
	return m_lastRect.IsPointInside(pt);
}

math::vector2d IConnector::GetConnectionPoint()
{
	math::vector2d size(10,10);
	math::vector2d res;
	if(!m_owner)
		return 0;
	const math::rectf& orc= m_owner->GetBoundingRect();
	switch(m_placement)
	{
	case ECP_Top:
		{
			float x=orc.ULPoint.x;
			x+=m_relativePos*orc.getWidth();
			res.set(x,orc.ULPoint.y);
		}
		break;
	case ECP_Bot:
		{
			float x=orc.ULPoint.x;
			x+=m_relativePos*orc.getWidth();
			res.set(x,orc.BRPoint.y);
		}
		break;
	case ECP_Left:
		{
			float y=orc.ULPoint.y;
			y+=m_relativePos*orc.getHeight();
			res.set(orc.ULPoint.x,y);
		}
		break;
	case ECP_Right:
		{
			float y=orc.ULPoint.y;
			y+=m_relativePos*orc.getHeight();
			res.set(orc.BRPoint.x,y);
		}
		break;
	}
	res+=m_owner->GetPos();
	return res;
}
math::vector2d IConnector::GetConnectionNormal()
{
	switch(m_placement)
	{
	case ECP_Top:
		return math::vector2d(0,-1);
	case ECP_Bot:
		return math::vector2d::YAxis;
	case ECP_Left:
		return math::vector2d(-1,0);
	case ECP_Right:
		return math::vector2d::XAxis;
	}
	return 0;
}

}
}
