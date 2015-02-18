


/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   23:19
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\MotorEncoderComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	MotorEncoderComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___MotorEncoderComponent___
#define ___MotorEncoderComponent___

#include "IVTComponent.h"


namespace mray
{
	namespace game
	{
		class PhysicalJointDOF6Component;
	}
namespace VT
{

class MotorEncoderComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:
	core::string m_jointName;
	game::PhysicalJointDOF6Component* m_joint;
	float m_precision;
	float m_encoderVal;
public:
	DECLARE_PROPERTY_TYPE(JointName,core::string,);
	DECLARE_PROPERTY_TYPE(Precision,float,);
public:
	MotorEncoderComponent(game::GameEntityManager*mngr);
	virtual~MotorEncoderComponent();
	
	virtual bool InitComponent();

	DECLARE_SETGET_PROP(JointName,core::string);
	DECLARE_SETGET_PROP(Precision,float);

	float GetEncoderValue(){return m_encoderVal;}

	void Update(float dt);
	
};
DECLARE_GAMECOMPONENT_FACTORY(MotorEncoderComponent);


}
}

#endif

