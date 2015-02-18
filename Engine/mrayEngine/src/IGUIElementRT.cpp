

#include "stdafx.h"
#include "IGUIElementRT.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "Viewport.h"
#include "CameraNode.h"
#include "Engine.h"


namespace mray
{
namespace scene
{

	IMPLEMENT_RTTI(IGUIElementRT,IObject)

	class CustomRenderArea:public video::IRenderArea
	{
	protected:
		math::vector2di m_size;
	public:
		CustomRenderArea(const math::vector2di& sz):m_size(sz){}
		virtual~CustomRenderArea()
		{
		}

		virtual math::vector2di getSize(){return m_size;}

		virtual const video::ITexturePtr& getColorTexture(int i=0){return video::ITexturePtr::Null;}

		virtual void GetParameter(const core::string& name,void* param){}

	};
IGUIElementRT::IGUIElementRT()
{
	m_enableBackClick=false;
	m_owner=0;
	m_creator=0;
	m_resolution=1;
	m_manualRender=false;

	video::IVideoDevice* device= Engine::getInstance().getDevice();
	m_renderTexture=device->createEmptyTexture2D(true);
	m_renderTexture->setMipmapsFilter(false);
	m_renderTexture->createTexture(1,video::EPixel_R8G8B8A8);
	m_renderTarget=device->createRenderTarget("",m_renderTexture,0,0,0);
}
IGUIElementRT::~IGUIElementRT()
{
	m_renderTarget=0;
	m_renderTexture=0;
}


void IGUIElementRT::_CheckRT()
{
	math::vector3d sz(1);
	sz.set(_GetElementSize().x,_GetElementSize().y,1);
	sz*=m_resolution;
	sz.x=math::Max<float>(sz.x,1);
	sz.y=math::Max<float>(sz.y,1);
	sz.z=1;

	if(m_renderTexture->getSize()!=sz)
	{
		m_renderTexture->createTexture(sz,video::EPixel_R8G8B8A8);
	}
}

bool IGUIElementRT::OnEvent(Event* evt)
{
	if(!m_owner || !m_owner->isVisible())
		return false;
	if(evt->getType()==ET_Mouse)
	{
		MouseEvent* e=(MouseEvent*)evt;
		scene::ViewPort* vp= e->GetViewPort();
		if(!vp)
			return false;
		scene::CameraNode* cam= vp->getCamera();
		if(!cam)
			return false;

		math::Plane p;
		p.set(math::vector3d::YAxis,0);
		if(!m_enableBackClick && p.getVertLocation(cam->getAbsolutePosition())<0)
			return false;

		//transform mouse coords from screen space to world space
		math::line3d ray;
		math::matrix4x4 mInv;
		ray.pStart= cam->getAbsolutePosition();
		ray.pEnd= cam->ScreenToWorld(e->pos,1000);
		m_owner->getAbsoluteTransformation().getInverse(mInv);
		ray.pStart=mInv* ray.pStart;
		ray.pEnd=mInv* ray.pEnd;

		math::vector3d pos;
		p.getIntersectionWithLine(ray.pStart,ray.getVector(),pos);

		if(pos.x<-0.5 || pos.x>0.5 || 
			pos.z<-0.5 || pos.z>0.5)
			return false;


		MouseEvent et;
		et.event=e->event;
		et.MouseWheel=e->MouseWheel;
		et.MouseWheelRel=e->MouseWheelRel;
		et.rel=e->rel;
		et.vpSize=e->vpSize;
		pos=-pos+0.5;
		et.pos.x=pos.x;
		et.pos.y=pos.z;

		bool ret=_OnMouseEvent(&et);
		if(et.isRecived())
			e->setRecived();

		return ret;
	}else if(evt->getType()==ET_Keyboard)
	{
		return _OnKeyboardEvent((KeyboardEvent*)evt);
	}
	return false;
}
void IGUIElementRT::DrawElement()
{
// 	if(!m_owner->IsDrown())
// 		return;
	_CheckRT();
	video::IVideoDevice* device= Engine::getInstance().getDevice();
	video::IRenderTargetPtr rt= device->getRenderTarget();
	math::recti vpRc(0,_GetElementSize());
	device->setRenderTarget(m_renderTarget,1,1,0,&vpRc);
	math::rectf vp(0, m_renderTarget->GetSize());
	_DrawElement(&vp);//&CustomRenderArea(_GetElementSize()));

	if(rt.isNull())
		device->setRenderTarget(0,0,0,0);
	else
		device->setRenderTarget(rt,0,0,0);




}
	
}
}

