

#include "stdafx.h"
#include "ICanvasObject.h"
#include "CanvasManager.h"
#include "MouseEvent.h"
#include "matrix3x3.h"
#include "IConnector.h"

namespace mray
{
namespace canvas
{

	IMPLEMENT_RTTI(ICanvasObject,IObject);


ICanvasObject::ICanvasObject():m_canvasManager(0),m_angle(0),m_status(ENone),m_isDraging(false)
{
}
ICanvasObject::~ICanvasObject()
{
	ClearShapes();
}

void ICanvasObject::AddShape(IShape* s)
{
	s->SetOwner(this);
	m_shapes.push_back(s);

	if(m_shapes.size()==1)
		m_boundingRect=s->GetBoundingRect();
	else
	{
		m_boundingRect.addPoint(s->GetBoundingRect().ULPoint);
		m_boundingRect.addPoint(s->GetBoundingRect().BRPoint);
	}
}


void ICanvasObject::ClearShapes()
{
	for(int i=0;i<m_shapes.size();++i)
	{
		delete m_shapes[i];
	}
	m_shapes.clear();
	m_boundingRect.ULPoint=m_boundingRect.BRPoint=0;
}

void ICanvasObject::AddConnector(IConnector* s)
{
	s->SetOwner(this);
	m_connectors.push_back(s);
}

void ICanvasObject::ClearConnectors()
{
	for(int i=0;i<m_connectors.size();++i)
	{
		delete m_connectors[i];
	}
	m_connectors.clear();
}

void ICanvasObject::Draw()
{
	m_canvasManager->SetTransformation(m_pos,m_angle,1);
	for(int i=0;i<m_shapes.size();++i)
	{
		m_shapes[i]->Draw(m_status);
	}
	for(int i=0;i<m_connectors.size();++i)
	{
		m_connectors[i]->Draw();
	}
}

IShape* ICanvasObject::GetShapeFromPoint(const math::vector2d& pt)
{
	//transform point
	math::matrix3x3 m;
	m.setAngle(-m_angle);
	m.translate(math::vector2d(-m_pos.x,-m_pos.y));
	math::vector2d p=m*pt;

	for(int i=m_shapes.size()-1;i>=0;--i)
	{
		if(m_shapes[i]->IsPointIn(p))
			return m_shapes[i];
	}
	return 0;
}
IConnector* ICanvasObject::GetConnectorFromPoint(const math::vector2d& pt)
{
	//transform point
	math::matrix3x3 m;
	m.setAngle(-m_angle);
	m.translate(math::vector2d(-m_pos.x,-m_pos.y));
	math::vector2d p=m*pt;

	for(int i=0;i<m_connectors.size();++i)
	{
		if(m_connectors[i]->IsPointIn(p))
			return m_connectors[i];
	}
	return 0;
}

void ICanvasObject::_ProcessDragging(const math::vector2d& pt,MOUSE_EVENT_TYPE evt)
{
	if(evt==MET_RIGHTDOWN)
	{
		m_isDraging=false;
		m_pos=m_originalPos;
	}
	else if(evt==MET_MOVED)
	{
		m_pos=m_originalPos+(pt-m_originalMousePos);
		m_pos.x=m_canvasManager->AlignToGrid(m_pos.x);
		m_pos.y=m_canvasManager->AlignToGrid(m_pos.y);
		/*
		m_pos+=pt-m_originalMousePos;
		m_originalMousePos=pt;*/
	}
}

bool ICanvasObject::ProcessMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	IShape* shape=GetShapeFromPoint(pt);
	if(!shape)
	{
		if(m_isDraging)
		{
			_ProcessDragging(pt,evt);
			return true;
		}
		return false;
	}

	bool dragged=false;

	if(evt==MET_LEFTDOWN)
		m_status=EClicked;
	else if(evt==MET_LEFTUP)
		m_status=EHovered;
	else if(evt==MET_RIGHTDOWN)
	{
		m_status=EHovered;
		if(m_isDraging)
			m_pos=m_originalPos;
	}
	else if(evt==MET_MOVED)
	{
		if(m_status==EClicked || m_isDraging)
		{
			_ProcessDragging(pt,evt);
			dragged=true;
		}
		else
			m_status=EHovered;
		/*
		if(m_status==EClicked || m_isDraging)
		{
			if(!m_isDraging)
			{
				m_originalPos=m_pos;
				m_originalMousePos=pt;
			}
			dragged=true;
		}else
			m_status=EHovered;
			*/
	}else
		m_status=EHovered;
	m_isDraging=dragged;
	return true;
}
void ICanvasObject::LostFocus()
{
	m_status=ENone;
}

math::rectf ICanvasObject::GetTransformedRect()
{
	const math::vector2d edges[]=
	{
		m_boundingRect.ULPoint,
		math::vector2d(m_boundingRect.BRPoint.x,m_boundingRect.ULPoint.y),
		m_boundingRect.BRPoint,
		math::vector2d(m_boundingRect.ULPoint.x,m_boundingRect.BRPoint.y),
	};
	math::matrix3x3 m;
	m.setAngle(m_angle);
	m.translate(m_pos);
	math::rectf r;
	r.ULPoint=r.BRPoint=m_pos;
	for(int i=0;i<4;++i)
	{
		r.addPoint(m*edges[i]);
	}
	return r;
	
}

void ICanvasObject::ConnectorConnected(IConnector* conn,IConnector* other)
{
}

}
}