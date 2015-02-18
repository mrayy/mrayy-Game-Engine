
/********************************************************************
	created:	2014/02/16
	created:	16:2:2014   2:01
	filename: 	C:\Development\mrayEngine\Projects\AugTel\ArmExtenderComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	ArmExtenderComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ArmExtenderComponent__
#define __ArmExtenderComponent__

#include "ArmComponent.h"


namespace mray
{
namespace AugTel
{

class ArmExtenderComponent :public ArmComponent
{
	DECLARE_RTTI;
protected:
	float m_threshold;

	math::vector3d m_lastPos;
	math::vector3d m_avgVelocity;

	float m_time;
	int m_extension;
	DECLARE_PROPERTY_TYPE(Threshold, float, );
	
	void _SetExtension(int e);
public:
	ArmExtenderComponent(game::GameEntityManager*m);
	virtual~ArmExtenderComponent();

	DECLARE_SETGET_PROP(Threshold, float);

	virtual void Update(float dt);
};

DECLARE_GAMECOMPONENT_FACTORY(ArmExtenderComponent);
}
}

#endif
