
#include "stdafx.h"

#include <windows.h>
#include <gl\gl.h>
#include "CanvasManager.h"
#include "IVideoDevice.h"
#include "ICanvasObject.h"
#include "IConnectionCurve.h"
#include "FontResourceManager.h"
#include "GUIBatchRenderer.h"
#include "IConnector.h"
#include "DefaultColors.h"

#include "ConnectionCurve.h"
#include "DirectedConnector.h"



namespace mray
{
namespace canvas
{


CanvasManager::CanvasManager(video::IVideoDevice* dev):m_device(dev),m_focusObject(0),m_rotate(0),m_scale(1),
m_selectedConnector(0),m_selectedCurve(0),m_dragging(0),m_enableGrid(true),m_gridSpacing(20)
{
	m_renderer=new GUI::GUIBatchRenderer();
	((GUI::GUIBatchRenderer*)m_renderer)->SetDevice(dev);
	m_fontAttrs.fontSize=13;
	m_fontAttrs.hasShadow=false;
	m_fontAttrs.wrap=true;

	m_background=new video::TextureUnit();
}
CanvasManager::~CanvasManager()
{
	ClearObjects();
	if(m_renderer)
		delete m_renderer;
}


void CanvasManager::_updateTransformation()
{
	math::matrix4x4 transform;
	m_transformation.makeIdentity();
	m_transformation.setTranslation(math::vector3d(m_translate.x,m_translate.y,0));
	m_transformation.rotateZ(m_rotate);
	if(m_scale!=1)
	{
		math::matrix4x4 o;
		o.setScale(math::vector3d(m_scale,m_scale,1));
		m_transformation=m_transformation*o;
	}
	m_transformation.getInverse(m_transformationInv);
}

void CanvasManager::SetTranslation(const math::vector2d &pos)
{
	m_translate=pos;
	_updateTransformation();
}
void CanvasManager::SetBackgroundTexture(video::ITextureCRef tex)
{
	m_background->SetTexture(tex);
}
video::TextureUnitCRef CanvasManager::GetBackgroundTextureUnit()
{
	return m_background;
}

float CanvasManager::AlignToGrid(float v)
{
	if(!m_enableGrid)return v;
	v=v-fmod(v,m_gridSpacing);
	return v;
}

void CanvasManager::SetRotation(float angle)
{
	m_rotate=angle;
	_updateTransformation();
}

void CanvasManager::SetScale(float scale)
{
	m_scale=scale;
	_updateTransformation();
}

IConnectionCurve* CanvasManager::CreateConnection(IConnector* source,IConnector* destination)
{
	ConnectionCurve* curve=new ConnectionCurve(this);
	AddCurve(curve);
	curve->SetSource(source);
	curve->SetDestination(destination);

	return curve;
}

void CanvasManager::AddObject(ICanvasObject*o)
{
	o->SetCanvasManager(this);
	m_objects.push_back(o);
}
void CanvasManager::RemoveObject(ICanvasObject*o)
{
	if(m_focusObject==o)
		m_focusObject=0;
	if(m_selectedConnector!=0 && m_selectedConnector->GetOwner()==o)
		m_selectedConnector=0;
	std::list<ICanvasObject*>::iterator it=m_objects.begin();
	for(;it!=m_objects.end();++it)
	{
		if((*it)==o)
		{
			m_objects.erase(it);
			break;
		}
	}
}
void CanvasManager::ClearObjects()
{
	std::list<ICanvasObject*>::iterator it=m_objects.begin();
	for(;it!=m_objects.end();++it)
	{
		delete *it;
	}
	m_objects.clear();
	m_focusObject=0;
	ClearCurves();
}


void CanvasManager::AddCurve(IConnectionCurve*o)
{
	o->SetCanvasManager(this);
	m_curves.push_back(o);

}

void CanvasManager::RemoveCurve(IConnectionCurve*o)
{
	if(m_selectedCurve==o)
		m_selectedCurve=0;
	std::list<IConnectionCurve*>::iterator it=m_curves.begin();
	for(;it!=m_curves.end();++it)
	{
		if((*it)==o)
		{
			m_curves.erase(it);
			break;
		}
	}

}

void CanvasManager::ClearCurves()
{
	std::list<IConnectionCurve*>::iterator it=m_curves.begin();
	for(;it!=m_curves.end();++it)
	{
		delete *it;
	}
	m_curves.clear();
}

void CanvasManager::SendBack(ICanvasObject*o)
{
	std::list<ICanvasObject*>::iterator it=m_objects.begin();
	for(;it!=m_objects.end();++it)
	{
		if((*it)==o)
		{
			m_objects.erase(it);
			m_objects.push_front(o);
			break;
		}
	}
	
}

void CanvasManager::SendFront(ICanvasObject*o)
{
	std::list<ICanvasObject*>::iterator it=m_objects.begin();
	for(;it!=m_objects.end();++it)
	{
		if((*it)==o)
		{
			m_objects.erase(it);
			m_objects.push_back(o);
			break;
		}
	}


}


void CanvasManager::DrawObjects(const math::recti& rc)
{

	if(rc.getWidth()<=0 || rc.getHeight()<=0)
		return;
	math::recti vp;
	math::recti lastVP;
	//lastVP=m_device->getViewportRect();

	vp=m_vpRect=rc;
	vp.ULPoint.y=m_device->getRenderTargetSize().y-m_vpRect.ULPoint.y;
	vp.BRPoint.y=m_device->getRenderTargetSize().y-m_vpRect.BRPoint.y;

	math::Swap(vp.BRPoint.y,vp.ULPoint.y);

//	glEnable(0x809D);
	bool ms=m_device->getMultiSampling();
	m_device->setMultiSampling(true);
	//m_device->setViewportRect(vp);
	m_device->set2DMode();
	m_device->useTexture(0,m_background);
	m_device->draw2DImage(math::rectf(0,0,rc.getWidth(),rc.getHeight()),video::DefaultColors::White);
	m_device->setTransformationState(video::TS_WORLD,m_transformation);

	{
		std::list<ICanvasObject*>::iterator it=m_objects.begin();
		for(;it!=m_objects.end();++it)
		{
			(*it)->Draw();
		}
	}
	SetTransformation(0,0,1);
	{
		std::list<IConnectionCurve*>::iterator it=m_curves.begin();
		for(;it!=m_curves.end();++it)
		{
			(*it)->Draw();
		}
	}
	SetTransformation(0,0,1);
	if(m_selectedConnector)
	{
		m_device->setLineWidth(2);
		math::vector2d pos=m_selectedConnector->GetLastRect().getCenter()+m_selectedConnector->GetOwner()->GetPos();
		m_device->draw2DLine(pos,m_mousePos,video::DefaultColors::Black);
	}

	//m_device->setViewportRect(lastVP);
	m_device->set2DMode();
	m_device->setMultiSampling(false);
//	glDisable(0x809D);
}

void CanvasManager::SetTransformation(const math::vector2d& pos,float angle,const math::vector2d& scale)
{

	math::matrix4x4 transform;
	transform.setTranslation(math::vector3d(pos.x,pos.y,0));
	transform.rotateZ(angle);
	if(scale!=1)
	{
		math::matrix4x4 o;
		o.setScale(math::vector3d(scale.x,scale.y,1));
		transform=transform*o;
	}
	m_device->setTransformationState(video::TS_WORLD,m_transformation*transform);
}

void CanvasManager::DrawLines(const PointList& points,const video::SColor& clr,float thickness)
{
	m_device->setLineWidth(thickness);
	m_device->draw2DLine(&points[0],points.size(),clr);
}

void CanvasManager::DrawString(const core::UTFString& str,const math::rectf& rc,const video::SColor& clr)
{
	m_renderer->Prepare();
	m_fontAttrs.fontColor=clr;
	GUI::IFont* font=gFontResourceManager.getFontByName(m_fontAttrs.fontName);
	if(!font)return;
	font->print(rc,&m_fontAttrs,&rc,str,m_renderer);
	m_renderer->Flush();
}

void CanvasManager::DrawShapeWired(const PointList& points,const video::SColor& clr,float thickness)
{
	m_device->setLineWidth(thickness);
	m_device->draw2DShape(&points[0],points.size(),clr,false);
}
void CanvasManager::DrawShapeFilled(const PointList& points,const video::SColor& clr)
{
	m_device->draw2DShape(&points[0],points.size(),clr,true);
}

void CanvasManager::DrawShapeFilledGrad(const PointList& points,const video::SColor* clr)
{
	m_device->draw2DShape(&points[0],clr,points.size(),true);
}

void CanvasManager::DrawShapeTextured(const PointList& points,const video::SColor& clr,
									  const math::vector2d* coords,video::TextureUnitCRef tex)
{
	m_device->useTexture(0,tex);
	m_device->draw2DShapeTextured(&points[0],coords,points.size(),clr,true);
}
void CanvasManager::SetActiveCurve(IConnectionCurve*c)
{
	if(c)
	{
		SetFocus(0);
	}
	if(m_selectedCurve==c)return;
	if(m_selectedCurve)
		m_selectedCurve->LostFocus();
	m_selectedCurve=c;
}
void CanvasManager::SetFocus(ICanvasObject*o)
{
	if(o)
		SetActiveCurve(0);
	if(m_focusObject==o)return;

	if(m_focusObject)
		m_focusObject->LostFocus();
	m_focusObject=o;
}
bool CanvasManager::_ProcessObjectsMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	if(m_objects.empty())
		return false;

	std::list<ICanvasObject*>::iterator it=m_objects.end();
	--it;
	bool done=false;
	bool found=false;
	while(!done && !found)
	{
		if((*it)->ProcessMouseEvent(pt,evt))
		{
			SetFocus(*it);
			if(evt==MET_LEFTDOWN)
			{
				SendFront(*it);
			}
			found=true;
			break;
		}
		if(it==m_objects.begin())
			done=true;
		else
			--it;
	}
	if(!found && m_focusObject)
		SetFocus(0);
	return m_focusObject!=0;
}
bool CanvasManager::_ProcessConnectorsMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	if(m_objects.empty())
		return false;

	if(evt==MET_RIGHTDOWN)
	{
		m_selectedConnector=0;
	}
	if(evt==MET_LEFTDOWN || evt==MET_LEFTUP)
	{
		std::list<ICanvasObject*>::iterator it=m_objects.end();
		--it;
		bool done=false;
		bool found=false;

		while(!done && !found)
		{
			IConnector*conn=(*it)->GetConnectorFromPoint(pt);
			if(conn)
			{
				bool selected=false;	// flag it to avoid entering to the next if
				if(evt==MET_LEFTDOWN)
				{
					if(!m_selectedConnector)
					{
						m_selectedConnector=conn;
						selected=true;
						SendFront(*it);
						return true;
					}else if(conn==m_selectedConnector)
						m_selectedConnector=0;
				}

				IConnector::EAcceptanceResult r=conn->Accepts(m_selectedConnector);
				if((evt==MET_LEFTDOWN || evt==MET_LEFTUP) && m_selectedConnector 
					&& m_selectedConnector!=conn && !selected	//basic checks
					&& r!=IConnector::EAR_No
					&& !m_selectedConnector->HasConnection(conn)	//make sure it is not connected to it
					)
				{
					SendFront(*it);
					if(r==IConnector::EAR_Destination)
					{
						math::Swap(m_selectedConnector,conn);
					}
					IConnectionCurve* curve=CreateConnection(m_selectedConnector,conn);
					conn->OnConnected(m_selectedConnector);
					m_selectedConnector->OnConnected(conn);
					m_selectedConnector->AddConnection(curve);
					conn->AddConnection(curve);
					m_selectedConnector=0;
				}
				return true;
			}
			if(it==m_objects.begin())
				done=true;
			else
				--it;
		}
	}
	return false;
}
bool CanvasManager::_ProcessCurvesMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)
{
	std::list<IConnectionCurve*>::iterator it=m_curves.begin();
	for(;it!=m_curves.end();++it)
	{
		IConnectionCurve*c=(*it);
		if(c->ProcessMouse(pt,evt)!=ENone)
		{
			SetActiveCurve(c);
			return true;
		}
	}
	return false;

}

bool CanvasManager::ProcessMouseEvent(MouseEvent* evt)
{
	math::vector2d pt=evt->pos-m_vpRect.ULPoint;

	math::vector3d transformed=m_transformationInv*math::vector3d(pt.x,pt.y,0);
	pt.set(transformed.x,transformed.y);

	if(evt->event==MET_MIDDLEDOWN)
	{
		m_dragging=true;
		m_startPos=evt->pos;
	}else if(evt->event==MET_MOVED)
	{
		float s=m_scale+evt->MouseWheelRel*0.001f;
		if(s<0.5)s=0.5;
		if(s>4)s=4;
		SetScale(s);
		if(m_dragging)
		{
			math::vector2d dp=(evt->pos-m_startPos);
			SetTranslation(dp+m_translate);
			m_startPos=evt->pos;
		}
	}else
		m_dragging=false;
	m_mousePos=pt;
	if(m_dragging)
		return true;

	if(_ProcessConnectorsMouseEvent(pt,evt->event))
		return true;
	if(_ProcessObjectsMouseEvent(pt,evt->event))
		return true;
	if(_ProcessCurvesMouseEvent(pt,evt->event))
		return true;
	return false;
}


}
}
