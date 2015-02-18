
/********************************************************************
	created:	2012/07/13
	created:	13:7:2012   11:50
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicXMLSerializer.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicXMLSerializer
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PhysicXMLSerializer___
#define ___PhysicXMLSerializer___

#include "CompileConfig.h"


namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace physics
{
	class PhysicsSystemDesc;
	class PhysicalBodyDesc;
	class PhysicalNodeDesc;
	class PhysicalSpringDesc;
	class PhysicalJointLimitDesc;
	class PhysicalJointLimitPairDesc;
	class PhysicalMotor3DDesc;
	class PhysicalJointLimitSoftDesc;
	class PhysicalJointLimitSoftPairDesc;
	class PhysicalJointDriveDesc;/*
	
		class IPhysicalJointDistanceDesc;
		class IPhysicalJointSphericalDesc;
		class IPhysicalJointRevoluteDesc;
		class IPhysicalJointPulleyDesc;
		class IPhysicalJointPrismaticDesc;
		class IPhysicalJointPointOnLineDesc;
		class IPhysicalJointCylinderDesc;
		class IPhysicalJointFixedDesc;
		class IPhysicalJointD6Desc;*/
	
	class PhysicMaterialDesc;
	class IPhysicalJoint3DDesc;

#define DECLARE_LOAD_SAVE_XML(type,node)\
	static type* ##node ##_Load(xml::XMLElement*e);\
	static xml::XMLElement* Parse(xml::XMLElement*e,type*d);\
	static xml::XMLElement* Save(xml::XMLElement*e,type*d); 

class MRAY_DLL PhysicXMLSerializer
{
public:

	DECLARE_LOAD_SAVE_XML(PhysicsSystemDesc,PhysicsSystem);
	DECLARE_LOAD_SAVE_XML(PhysicalBodyDesc,PhysicalBody);
	DECLARE_LOAD_SAVE_XML(PhysicalNodeDesc,PhysicalNode);
	DECLARE_LOAD_SAVE_XML(PhysicalSpringDesc,PhysicalSpring);
	DECLARE_LOAD_SAVE_XML(PhysicalJointLimitDesc,PhysicalJointLimit);
	DECLARE_LOAD_SAVE_XML(PhysicalJointLimitPairDesc,PhysicalJointLimitPair);
	DECLARE_LOAD_SAVE_XML(PhysicalMotor3DDesc,PhysicalMotor);
	DECLARE_LOAD_SAVE_XML(PhysicalJointLimitSoftDesc,PhysicalJointLimitSoft);
	DECLARE_LOAD_SAVE_XML(PhysicalJointLimitSoftPairDesc,PhysicalJointLimitSoftPair);
	DECLARE_LOAD_SAVE_XML(PhysicalJointDriveDesc,PhysicalJointDrive);
	/*
	DECLARE_LOAD_SAVE_XML(IPhysicalJointDistanceDesc,PhysicalJointDistance);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointSphericalDesc,PhysicalJointSpherical);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointRevoluteDesc,PhysicalJointRevolute);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointPulleyDesc,PhysicalJointPulley);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointPrismaticDesc,PhysicalJointPrismatic);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointPointOnLineDesc,IPhysicalJointPointOnLine);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointCylinderDesc,PhysicalJointCylinder);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointFixedDesc,PhysicalJointFixed);
	DECLARE_LOAD_SAVE_XML(IPhysicalJointD6Desc,PhysicalJointD6);*/

	DECLARE_LOAD_SAVE_XML(PhysicMaterialDesc,PhysicMaterial);
	DECLARE_LOAD_SAVE_XML(IPhysicalJoint3DDesc,PhysicalJoint);
};

#undef DECLARE_LOAD_SAVE_XML

}
}

#endif
