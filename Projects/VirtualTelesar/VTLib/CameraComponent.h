

/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   23:19
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\CameraComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	CameraComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CameraComponent___
#define ___CameraComponent___

#include "MountableComponent.h"


namespace mray
{
namespace scene
{
	class StereoRenderer;
}
namespace VT
{

	enum ECameraType
	{
		ECamera_Color,
		ECamera_Grayscale,
		ECamera_Depth
	};
class CameraComponent:public MountableComponent
{
	DECLARE_RTTI;
protected:

	float m_fieldOfView;
	float m_znear;
	float m_zfar;
	float m_camOffset;

	int m_width;
	int m_height;
	int m_fps;
	ECameraType m_camType;
public:

	DECLARE_PROPERTY_TYPE(FieldOfView,float,);
	DECLARE_PROPERTY_TYPE(ZNear,float,);
	DECLARE_PROPERTY_TYPE(ZFar,float,);
	DECLARE_PROPERTY_TYPE(Width,int,);
	DECLARE_PROPERTY_TYPE(Height,int,);
	DECLARE_PROPERTY_TYPE(FPS,int,);
	DECLARE_PROPERTY_TYPE(CameraOffset,float,);
	DECLARE_PROPERTY_TYPE(CameraType,ECameraType,);
public:
	CameraComponent(game::GameEntityManager*mngr);
	virtual~CameraComponent();
	
	virtual bool InitComponent();

	DECLARE_SETGET_PROP(FieldOfView,float);
	DECLARE_SETGET_PROP(ZNear,float);
	DECLARE_SETGET_PROP(ZFar,float);
	DECLARE_SETGET_PROP(Width,int);
	DECLARE_SETGET_PROP(Height,int);
	DECLARE_SETGET_PROP(FPS,int);
	DECLARE_SETGET_PROP(CameraType,ECameraType);
	DECLARE_SETGET_PROP(CameraOffset,float);

	void MountCamera(scene::IMovable* hm,scene::StereoRenderer* r);
	
};
DECLARE_GAMECOMPONENT_FACTORY(CameraComponent);


}
}

#endif

