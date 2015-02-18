
#include "stdafx.h"
#include "CameraComponent.h"
#include "StereoRenderer.h"
#include "Viewport.h"


namespace mray
{
namespace VT
{


	ECameraType ToCameraType(const core::string& str)
	{
		if(str.equals_ignore_case(mT("color")))
			return ECamera_Color;
		if(str.equals_ignore_case(mT("grayscale")))
			return ECamera_Grayscale;
		if(str.equals_ignore_case(mT("depth")))
			return ECamera_Depth;
		return ECamera_Color;
	}
	core::string CameraTypeToString(ECameraType type)
	{
		if(type==ECamera_Color)
			return mT("Color");
		if(type==ECamera_Grayscale)
			return mT("Grayscale");
		if(type==ECamera_Depth)
			return mT("Depth");
		return mT("");
	}

	CameraComponent::CameraComponent(game::GameEntityManager*mngr)
		:MountableComponent(mngr)
	{

		m_fieldOfView=PropertyTypeFieldOfView::instance.GetDefaultValue();
		m_znear=PropertyTypeZNear::instance.GetDefaultValue();
		m_zfar=PropertyTypeZFar::instance.GetDefaultValue();
		m_width=PropertyTypeWidth::instance.GetDefaultValue();
		m_height=PropertyTypeHeight::instance.GetDefaultValue();
		m_fps=PropertyTypeFPS::instance.GetDefaultValue();
		m_camType=PropertyTypeCameraType::instance.GetDefaultValue();
		m_camOffset=PropertyTypeCameraOffset::instance.GetDefaultValue();
		CPropertieDictionary* dic;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeFieldOfView::instance);
			dic->addPropertie(&PropertyTypeZNear::instance);
			dic->addPropertie(&PropertyTypeZFar::instance);
			dic->addPropertie(&PropertyTypeWidth::instance);
			dic->addPropertie(&PropertyTypeHeight::instance);
			dic->addPropertie(&PropertyTypeFPS::instance);
			dic->addPropertie(&PropertyTypeCameraType::instance);
			dic->addPropertie(&PropertyTypeCameraOffset::instance);
		}
	}
	CameraComponent::~CameraComponent()
	{
	}

	bool CameraComponent::InitComponent()
	{
		if(!MountableComponent::InitComponent())
			return false;
		return true;
	}

	IMPLEMENT_SETGET_PROP(CameraComponent,FieldOfView,float,m_fieldOfView,45,core::StringConverter::toFloat,core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(CameraComponent,ZNear,float,m_znear,0.01,core::StringConverter::toFloat,core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(CameraComponent,ZFar,float,m_zfar,1000,core::StringConverter::toFloat,core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(CameraComponent,Width,int,m_width,128,core::StringConverter::toInt,core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(CameraComponent,Height,int,m_height,128,core::StringConverter::toInt,core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(CameraComponent,FPS,int,m_fps,30,core::StringConverter::toInt,core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(CameraComponent,CameraType,ECameraType,m_camType,ECamera_Color,ToCameraType,CameraTypeToString);
	IMPLEMENT_SETGET_PROP(CameraComponent,CameraOffset,float,m_camOffset,0.06,core::StringConverter::toFloat,core::StringConverter::toString);


	void CameraComponent::MountCamera(scene::IMovable* hm,scene::StereoRenderer* r)
	{
		if(m_node)
		{
			m_node->addChild(hm);
			hm->setPosition(m_offset);
			hm->setOrintation(m_angles);
		}
		if(r)
		{
			r->GetLeftCamera()->setFovY(m_fieldOfView);
			r->GetLeftCamera()->setZNear(m_znear);
			r->GetLeftCamera()->setZFar(m_zfar);

			r->GetRightCamera()->setFovY(m_fieldOfView);
			r->GetRightCamera()->setZNear(m_znear);
			r->GetRightCamera()->setZFar(m_zfar);

			math::vector2d size(m_width,m_height);

			math::rectf rc= r->GetLeftVP()->getAbsViewPort();
			rc.BRPoint=rc.ULPoint+size;
			r->GetLeftVP()->setAbsViewPort(rc);

			video::IRenderTarget* rt= r->GetLeftVP()->getRenderTarget();
			if(rt)
			{
				for(int i=0;i<16;++i)
				{
					video::ITexture* tex= rt->GetColorTexture(i);
					if(tex)
					{
						if(tex->getSize()!=math::vector3d(size.x,size.y,1))
						{
							tex->createTexture(math::vector3d(size.x,size.y,1),tex->getImageFormat());
						}
					}
				}
			}

			rc= r->GetRightVP()->getAbsViewPort();
			rc.BRPoint=rc.ULPoint+size;
			r->GetRightVP()->setAbsViewPort(rc);

			rt= r->GetRightVP()->getRenderTarget();
			if(rt)
			{
				for(int i=0;i<16;++i)
				{
					video::ITexture* tex= rt->GetColorTexture(i);
					if(tex)
					{
						if(tex->getSize()!=math::vector3d(size.x,size.y,1))
						{
							tex->createTexture(math::vector3d(size.x,size.y,1),tex->getImageFormat());
						}
					}
				}
			}

			r->SetOffset(m_camOffset);
		}
	}

}
}
