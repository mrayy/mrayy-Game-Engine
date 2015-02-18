


/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   12:46
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\SphereModelComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	SphereModelComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___SphereModelComponent___
#define ___SphereModelComponent___

#include "IPrimitiveModelComponent.h"


namespace mray
{
namespace game
{
	/*
#define DECLARE_SETGET_PROP(dt,Name,prop)\
	const dt& Get##Name()const{return prop;}\
	bool Set##Name(const dt&v){prop=v;return true;}
	*/
class MRAY_GAME_DLL SphereModelComponent:public IPrimitiveModelComponent
{
	DECLARE_RTTI;
protected:

	float m_radius;
	int m_uSegments;
	int m_vSegments;

public:
	DECLARE_PROPERTY_TYPE(Radius,float,);
	DECLARE_PROPERTY_TYPE(USegments,int,);
	DECLARE_PROPERTY_TYPE(VSegments,int,);
public:
	SphereModelComponent(GameEntityManager*mngr);
	virtual~SphereModelComponent();

	DECLARE_SETGET_PROP(Radius,float);
	DECLARE_SETGET_PROP(USegments,int);
	DECLARE_SETGET_PROP(VSegments,int);

	virtual bool InitComponent();
};
DECLARE_GAMECOMPONENT_FACTORY(SphereModelComponent);

}
}

#endif
