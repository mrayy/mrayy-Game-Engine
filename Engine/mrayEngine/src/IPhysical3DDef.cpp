


#include "stdafx.h"
#include "IPhysical3DDef.h"
#include "XMLElement.h"
#include "StringConverter.h"
#include "PhysicalShapes.h"
#include "PhysicXMLSerializer.h"


namespace mray
{
namespace physics
{
	const core::string s_jointTypes[]=
	{
		mT("RevoluteJoint"),	
		mT("PrismaticJoint"),	
		mT("DistanceJoint"),	
		mT("PulleyJoint"),		
		mT("FixedJoint"),		
		mT("GearJoint"),		
		mT("D6Joint"),			
		mT("Cylindrical"),		
		mT("Spherical"),		
		mT("PointOnLine"),		
		mT("PointOnPlane")
	};

#define PARSE_ATTRIBUTE(val,attrName) if((attr=e->getAttribute(mT(attrName)))!=0)\
	core::StringConverter::parse(attr->value,val);
#define WRITE_ATTRIBUTE(val,attrName,DefaultVal)\
	if(!(val==DefaultVal))e->addAttribute(mT(attrName),core::StringConverter::toString(val));


#define PARSE_ELEMENT(val,attrName)\
	{\
		xml::XMLElement* elem=e->getSubElement(mT(attrName));\
		if(elem)\
			val.LoadFromXML(elem);\
	}

#define WRITE_ELEMENT(val,attrName)\
	{\
		xml::XMLElement* elem=new xml::XMLElement(mT(attrName));\
		e->addSubElement(elem);\
		val.SaveToXML(elem);\
		}
	const math::vector3d PhysicsSystemDesc::DefaultGravity(0,-9.8,0);
	const float PhysicsSystemDesc::DefaultMaxStep=1.0f/60.0f;
	void PhysicsSystemDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(gravity,"Gravity");
		PARSE_ATTRIBUTE(maxTimestep,"MaxTimestep");
		PARSE_ATTRIBUTE(maxIter,"MaxIter");
		PARSE_ATTRIBUTE(useFixedTimeStep,"FixedTimeStep");
	}
	void PhysicsSystemDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(gravity,"Gravity",DefaultGravity);
		WRITE_ATTRIBUTE(maxTimestep,"MaxTimestep",DefaultMaxStep);
		WRITE_ATTRIBUTE(maxIter,"MaxIter",DefaultMaxIter);
		WRITE_ATTRIBUTE(useFixedTimeStep,"FixedTimeStep",DefaultFixedTimeStep);
	}


	const float PhysicalBodyDesc::DefaultMass=0;
	const math::vector3d PhysicalBodyDesc::DefaultLinearVelocity;
	const math::vector3d PhysicalBodyDesc::DefaultAnglarVelocity;
	const float PhysicalBodyDesc::DefaultWakeupCounter=20.0f*0.02f;
	const float PhysicalBodyDesc::DefaultLinearDamping=0;
	const float PhysicalBodyDesc::DefaultAngularDamping=0.05f;
	const float PhysicalBodyDesc::DefaultMaxAngularVelocity=-1.0f;
	const float PhysicalBodyDesc::DefaultCCDMotionThreshold=0;
	const float PhysicalBodyDesc::DefaultSleepLinearVelocity=-1.0f;
	const float PhysicalBodyDesc::DefaultSleepAngularVelocity=-1.0f;
	const int   PhysicalBodyDesc::DefaultSolverIterationCount=4;
	const float PhysicalBodyDesc::DefaultSleepEnergyThreshold=-1.0f;
	const float PhysicalBodyDesc::DefaultSleepDamping=0;
	const float PhysicalBodyDesc::DefaultContactReportThreshold=math::Infinity;

	void PhysicalBodyDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(massLocalPos,"MassLocalPos");
		PARSE_ATTRIBUTE(massSpaceInertia,"MassSpaceInertia");
		PARSE_ATTRIBUTE(mass,"Mass");
		PARSE_ATTRIBUTE(linearVelocity,"LinearVelocity");
		PARSE_ATTRIBUTE(angularVelocity,"AngularVelocity");
		PARSE_ATTRIBUTE(wakeupCounter,"WakeupCounter");
		PARSE_ATTRIBUTE(linearDamping,"LinearDamping");
		PARSE_ATTRIBUTE(angularDamping,"AngularDamping");
		PARSE_ATTRIBUTE(maxAngularVelocity,"MaxAngularVelocity");
		PARSE_ATTRIBUTE(CCDMotionThreshold,"CCDMotionThreshold");
		PARSE_ATTRIBUTE(sleepLinearVelocity,"SleepLinearVelocity");
		PARSE_ATTRIBUTE(sleepAngularVelocity,"SleepAngularVelocity");
		PARSE_ATTRIBUTE(solverIterationCount,"SolverIterationCount");
		PARSE_ATTRIBUTE(sleepEnergyThreshold,"SleepEnergyThreshold");
		PARSE_ATTRIBUTE(sleepDamping,"SleepDamping");
		PARSE_ATTRIBUTE(contactReportThreshold,"ContactReportThreshold");
		PARSE_ATTRIBUTE(kinematic,"Kinematic");
		PARSE_ATTRIBUTE(disableGravity,"DisableGravity");
	}
	void PhysicalBodyDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(massLocalPos,"MassLocalPos",math::matrix4x4::Identity);
		WRITE_ATTRIBUTE(massSpaceInertia,"MassSpaceInertia",math::vector3d());
		WRITE_ATTRIBUTE(mass,"Mass",DefaultMass);
		WRITE_ATTRIBUTE(linearVelocity,"LinearVelocity",DefaultLinearVelocity);
		WRITE_ATTRIBUTE(angularVelocity,"AngularVelocity",DefaultAnglarVelocity);
		WRITE_ATTRIBUTE(wakeupCounter,"WakeupCounter",DefaultWakeupCounter);
		WRITE_ATTRIBUTE(linearDamping,"LinearDamping",DefaultLinearDamping);
		WRITE_ATTRIBUTE(angularDamping,"AngularDamping",DefaultAngularDamping);
		WRITE_ATTRIBUTE(maxAngularVelocity,"MaxAngularVelocity",DefaultMaxAngularVelocity);
		WRITE_ATTRIBUTE(CCDMotionThreshold,"CCDMotionThreshold",DefaultCCDMotionThreshold);
		WRITE_ATTRIBUTE(sleepLinearVelocity,"SleepLinearVelocity",DefaultSleepLinearVelocity);
		WRITE_ATTRIBUTE(sleepAngularVelocity,"SleepAngularVelocity",DefaultSleepAngularVelocity);
		WRITE_ATTRIBUTE(solverIterationCount,"SolverIterationCount",DefaultSolverIterationCount);
		WRITE_ATTRIBUTE(sleepEnergyThreshold,"SleepEnergyThreshold",DefaultSleepEnergyThreshold);
		WRITE_ATTRIBUTE(sleepDamping,"SleepDamping",DefaultSleepDamping);
		WRITE_ATTRIBUTE(contactReportThreshold,"ContactReportThreshold",DefaultContactReportThreshold);
		WRITE_ATTRIBUTE(kinematic,"Kinematic",false);
		WRITE_ATTRIBUTE(disableGravity,"DisableGravity",false);
	}

	const float PhysicalSpringDesc::DefaultSpring=0;
	const float PhysicalSpringDesc::DefaultDamper=0;
	const float PhysicalSpringDesc::DefaultTargetValue=0;
	void PhysicalSpringDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(spring,"Spring");
		PARSE_ATTRIBUTE(damper,"Damper");
		PARSE_ATTRIBUTE(targetValue,"TargetValue");
	}
	void PhysicalSpringDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(spring,"Spring",DefaultSpring);
		WRITE_ATTRIBUTE(damper,"Damper",DefaultDamper);
		WRITE_ATTRIBUTE(targetValue,"TargetValue",DefaultTargetValue);
	}


	const float PhysicalJointLimitDesc::DefaultValue=0;
	const float PhysicalJointLimitDesc::DefaultRestitution=0;
	const float PhysicalJointLimitDesc::DefaultHardness=1;
	void PhysicalJointLimitDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(value,"Value");
		PARSE_ATTRIBUTE(restitution,"Restitution");
		PARSE_ATTRIBUTE(hardness,"Hardness");
	}
	void PhysicalJointLimitDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(value,"Value",DefaultValue);
		WRITE_ATTRIBUTE(restitution,"Restitution",DefaultRestitution);
		WRITE_ATTRIBUTE(hardness,"Hardness",DefaultHardness);
	}
	void PhysicalJointLimitPairDesc::LoadFromXML(xml::XMLElement *e)
	{
		PARSE_ELEMENT(low,"Low");
		PARSE_ELEMENT(high,"High");
// 		PhysicXMLSerializer::Parse(e,&low);
// 		PhysicXMLSerializer::Parse(e,&high);
	}
	void PhysicalJointLimitPairDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ELEMENT(low,"Low");
		WRITE_ELEMENT(high,"High");
// 		PhysicXMLSerializer::Save(e,&low);
// 		PhysicXMLSerializer::Save(e,&high);

	}

	const float PhysicalMotor3DDesc::DefaultVelTarget=math::Infinity;
	const float PhysicalMotor3DDesc::DefaultMaxForce=0;
	const float PhysicalMotor3DDesc::DefaultFreeSpin=0;
	void PhysicalMotor3DDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(velTarget,"VelTarget");
		PARSE_ATTRIBUTE(maxForce,"MaxForce");
		PARSE_ATTRIBUTE(freeSpin,"FreeSpin");
	}
	void PhysicalMotor3DDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(velTarget,"VelTarget",DefaultVelTarget);
		WRITE_ATTRIBUTE(maxForce,"MaxForce",DefaultMaxForce);
		WRITE_ATTRIBUTE(freeSpin,"FreeSpin",DefaultFreeSpin);
	}

	const float PhysicalJointLimitSoftDesc::DefaultValue=0;
	const float PhysicalJointLimitSoftDesc::DefaultRestitution=0;
	const float PhysicalJointLimitSoftDesc::DefaultSpring=0;
	const float PhysicalJointLimitSoftDesc::DefaultDamping=0;
	void PhysicalJointLimitSoftDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(value,"Value");
		PARSE_ATTRIBUTE(restitution,"Restitution");
		PARSE_ATTRIBUTE(spring,"Spring");
		PARSE_ATTRIBUTE(damping,"Damping");
	}
	void PhysicalJointLimitSoftDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(value,"Value",DefaultValue);
		WRITE_ATTRIBUTE(restitution,"Restitution",DefaultRestitution);
		WRITE_ATTRIBUTE(spring,"Spring",DefaultSpring);
		WRITE_ATTRIBUTE(damping,"Damping",DefaultDamping);
	}

	void PhysicalJointLimitSoftPairDesc::LoadFromXML(xml::XMLElement *e)
	{
		PARSE_ELEMENT(low,"Low");
		PARSE_ELEMENT(high,"High");
	/*
		xml::XMLElement* elem=new xml::XMLElement("Low");
		e->addSubElement(elem);
		low.SaveToXML(elem);
		elem=new xml::XMLElement("High");
		e->addSubElement(elem);
		high.SaveToXML(elem);*/
	
	}
	void PhysicalJointLimitSoftPairDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ELEMENT(low,"Low");
		WRITE_ELEMENT(high,"High");
/*

		xml::XMLElement* elem=e->getSubElement("Low");
		if(elem)
			low.SaveToXML(elem);
		elem=e->getSubElement("High");
		if(elem)
			high.SaveToXML(elem);*/
			
	}


	const float PhysicalJointDriveDesc::DefaultSpring=0;
	const float PhysicalJointDriveDesc::DefaultDamping=0;
	const float PhysicalJointDriveDesc::DefaultForceLimit= math::Infinity;
	const bool PhysicalJointDriveDesc::DefaultDrivePosition=false;;
	const bool PhysicalJointDriveDesc::DefaultDriveVelocity=false;
	void PhysicalJointDriveDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(forceLimit,"ForceLimit");
		PARSE_ATTRIBUTE(drivePosition,"DrivePosition");
		PARSE_ATTRIBUTE(driveVelocity,"driveVelocity");
		PARSE_ATTRIBUTE(spring,"Spring");
		PARSE_ATTRIBUTE(damping,"Damping");
	}
	void PhysicalJointDriveDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(forceLimit,"ForceLimit",DefaultForceLimit);
		WRITE_ATTRIBUTE(drivePosition,"DrivePosition",DefaultDrivePosition);
		WRITE_ATTRIBUTE(driveVelocity,"driveVelocity",DefaultDriveVelocity);
		WRITE_ATTRIBUTE(spring,"Spring",DefaultSpring);
		WRITE_ATTRIBUTE(damping,"Damping",DefaultDamping);
	}

	const math::vector3d IPhysicalJoint3DDesc::DefaultLocalNormal=math::vector3d::XAxis;
	const math::vector3d IPhysicalJoint3DDesc::DefaultLocalAxis=math::vector3d::ZAxis;
	const math::vector3d IPhysicalJoint3DDesc::DefaultLocalAnchor;
	const float IPhysicalJoint3DDesc::DefaultMaxForce=math::Infinity;
	const float IPhysicalJoint3DDesc::DefaultMaxTorque=math::Infinity;
	const float IPhysicalJoint3DDesc::DefaultSolverExtrapolationFactor=1;
	const bool IPhysicalJoint3DDesc::DefaultUseAccelerationSpring=0;
	IPhysicalJoint3DDesc* IPhysicalJoint3DDesc::CreateJoint(const core::string& type)
	{
		if(type==s_jointTypes[ESJ3D_RevoluteJoint])
			return new IPhysicalJointRevoluteDesc();
		if(type==s_jointTypes[ESJ3D_PrismaticJoint])
			return new IPhysicalJointPrismaticDesc();
		if(type==s_jointTypes[ESJ3D_DistanceJoint])
			return new IPhysicalJointDistanceDesc();
		if(type==s_jointTypes[ESJ3D_PulleyJoint])
			return new IPhysicalJointPulleyDesc();
		if(type==s_jointTypes[ESJ3D_FixedJoint])
			return new IPhysicalJointFixedDesc();
		if(type==s_jointTypes[ESJ3D_D6Joint])
			return new IPhysicalJointD6Desc();
		if(type==s_jointTypes[ESJ3D_Cylindrical])
			return new IPhysicalJointCylinderDesc();
		if(type==s_jointTypes[ESJ3D_Spherical])
			return new IPhysicalJointSphericalDesc();
		if(type==s_jointTypes[ESJ3D_PointOnLine])
			return new IPhysicalJointPointOnLineDesc();
		if(type==s_jointTypes[ESJ3D_PointOnPlane])
			return new IPhysicalJointPointOnPlaneDesc();
		return 0;
	}
	IPhysicalJoint3DDesc* IPhysicalJoint3DDesc::CreateJoint(ESupported3DJoints type)
	{
		return CreateJoint(s_jointTypes[type]);
	}


	void IPhysicalJoint3DDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(localNormal[0],"LocalNormalA");
		PARSE_ATTRIBUTE(localNormal[1],"LocalNormalB");
		PARSE_ATTRIBUTE(localAxis[0],"LocalAxisA");
		PARSE_ATTRIBUTE(localAxis[1],"LocalAxisB");
		PARSE_ATTRIBUTE(localAnchor[0],"LocalAnchorA");
		PARSE_ATTRIBUTE(localAnchor[1],"LocalAnchorB");
		PARSE_ATTRIBUTE(maxForce,"MaxForce");
		PARSE_ATTRIBUTE(maxTorque,"MaxTorque");
		PARSE_ATTRIBUTE(solverExtrapolationFactor,"solverExtrapolationFactor");
		PARSE_ATTRIBUTE(useAccelerationSpring,"UseAccelerationSpring");
		PARSE_ATTRIBUTE(flags[EJFlag_CollisionEnable],"EnableCollision");
	}
	void IPhysicalJoint3DDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(s_jointTypes[getType()],"JointType",mT(""));
		WRITE_ATTRIBUTE(localNormal[0],"LocalNormalA",DefaultLocalNormal);
		WRITE_ATTRIBUTE(localNormal[1],"LocalNormalB",DefaultLocalNormal);
		WRITE_ATTRIBUTE(localAxis[0],"LocalAxisA",DefaultLocalAxis);
		WRITE_ATTRIBUTE(localAxis[1],"LocalAxisB",DefaultLocalAxis);
		WRITE_ATTRIBUTE(localAnchor[0],"LocalAnchorA",DefaultLocalAnchor);
		WRITE_ATTRIBUTE(localAnchor[1],"LocalAnchorB",DefaultLocalAnchor);
		WRITE_ATTRIBUTE(maxForce,"MaxForce",DefaultMaxForce);
		WRITE_ATTRIBUTE(maxTorque,"MaxTorque",DefaultMaxTorque);
		WRITE_ATTRIBUTE(solverExtrapolationFactor,"solverExtrapolationFactor",DefaultSolverExtrapolationFactor);
		WRITE_ATTRIBUTE(useAccelerationSpring,"UseAccelerationSpring",DefaultUseAccelerationSpring);
		WRITE_ATTRIBUTE(flags[EJFlag_CollisionEnable],"EnableCollision",false);
	}
	const float IPhysicalJointDistanceDesc::DefaultMaxDistance=0;
	const float IPhysicalJointDistanceDesc::DefaultMinDistance=0;
	const bool IPhysicalJointDistanceDesc::DefaultMaxDistanceEnable=0;
	const bool IPhysicalJointDistanceDesc::DefaultMinDistanceEnable=0;
	const bool IPhysicalJointDistanceDesc::DefaultSpringEnable=0;
	void IPhysicalJointDistanceDesc::LoadFromXML(xml::XMLElement *e)
	{
		IPhysicalJoint3DDesc::LoadFromXML(e);
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(maxDistance,"MaxDistance");
		PARSE_ATTRIBUTE(minDistance,"MinDistance");
		PARSE_ATTRIBUTE(maxDistanceEnable,"MaxDistanceEnable");
		PARSE_ATTRIBUTE(minDistanceEnable,"MinDistanceEnable");
		PARSE_ATTRIBUTE(springEnable,"SpringEnable");

		PARSE_ELEMENT(spring,"Spring");
		//PhysicXMLSerializer::Parse(e,&spring);
	}
	void IPhysicalJointDistanceDesc::SaveToXML(xml::XMLElement* e)
	{
		IPhysicalJoint3DDesc::SaveToXML(e);
		WRITE_ATTRIBUTE(maxDistance,"MaxDistance",DefaultMaxDistance);
		WRITE_ATTRIBUTE(minDistance,"MinDistance",DefaultMinDistance);
		WRITE_ATTRIBUTE(maxDistanceEnable,"MaxDistanceEnable",DefaultMaxDistanceEnable);
		WRITE_ATTRIBUTE(minDistanceEnable,"MinDistanceEnable",DefaultMinDistanceEnable);
		WRITE_ATTRIBUTE(springEnable,"SpringEnable",DefaultSpringEnable);

		WRITE_ELEMENT(spring,"Spring");
		//PhysicXMLSerializer::Save(e,&spring);
	}
	const math::vector3d IPhysicalJointSphericalDesc::DefaultSwingAxis=math::vector3d::ZAxis;
	const float IPhysicalJointSphericalDesc::DefaultProjectionDistance=1;
	const EJointProjectionMode IPhysicalJointSphericalDesc::DefaultProjectionMode=EJPM_None;
	void IPhysicalJointSphericalDesc::LoadFromXML(xml::XMLElement *e)
	{
		IPhysicalJoint3DDesc::LoadFromXML(e);
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(swingAxis,"SwingAxis");
		PARSE_ATTRIBUTE(projectionDistance,"ProjectionDistance");
		PARSE_ATTRIBUTE(projectionMode,"ProjectionMode");

		PARSE_ELEMENT(twistLimit,"TwistLimit");
		PARSE_ELEMENT(twistLimit,"SwingLimit");
		PARSE_ELEMENT(twistLimit,"TwistSpring");
		PARSE_ELEMENT(twistLimit,"SwingSpring");
		PARSE_ELEMENT(twistLimit,"JointSpring");
		
// 		PhysicXMLSerializer::Parse(e,&twistLimit);
// 		PhysicXMLSerializer::Parse(e,&swingLimit);
// 		PhysicXMLSerializer::Parse(e,&twistSpring);
// 		PhysicXMLSerializer::Parse(e,&swingSpring);
//		PhysicXMLSerializer::Parse(e,&jointSpring);
	}
	void IPhysicalJointSphericalDesc::SaveToXML(xml::XMLElement* e)
	{
		IPhysicalJoint3DDesc::SaveToXML(e);
		WRITE_ATTRIBUTE(swingAxis,"SwingAxis",DefaultSwingAxis);
		WRITE_ATTRIBUTE(projectionDistance,"ProjectionDistance",DefaultProjectionDistance);
		WRITE_ATTRIBUTE(projectionMode,"ProjectionMode",DefaultProjectionMode);

		WRITE_ELEMENT(twistLimit,"TwistLimit");
		WRITE_ELEMENT(twistLimit,"SwingLimit");
		WRITE_ELEMENT(twistLimit,"TwistSpring");
		WRITE_ELEMENT(twistLimit,"SwingSpring");
		WRITE_ELEMENT(twistLimit,"JointSpring");
// 		PhysicXMLSerializer::Save(e,&twistLimit);
// 		PhysicXMLSerializer::Save(e,&swingLimit);
// 		PhysicXMLSerializer::Save(e,&twistSpring);
// 		PhysicXMLSerializer::Save(e,&swingSpring);
// 		PhysicXMLSerializer::Save(e,&jointSpring);
	}


	const float IPhysicalJointRevoluteDesc::DefaultProjectionDistance=1.0f;
	const float IPhysicalJointRevoluteDesc::DefaultProjectionAngle=0.0872f;
	const bool IPhysicalJointRevoluteDesc::DefaultLimitEnable=false;
	const bool IPhysicalJointRevoluteDesc::DefaultMotorEnable=false;
	const bool IPhysicalJointRevoluteDesc::DefaultSpringEnable=false;
	void IPhysicalJointRevoluteDesc::LoadFromXML(xml::XMLElement *e)
	{
		IPhysicalJoint3DDesc::LoadFromXML(e);
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(limitEnable,"LimitEnable");
		PARSE_ATTRIBUTE(motorEnable,"MotorEnable");
		PARSE_ATTRIBUTE(springEnable,"SpringEnable");
		PARSE_ATTRIBUTE(projectionDistance,"ProjectionDistance");
		PARSE_ATTRIBUTE(projectionAngle,"ProjectionAngle");

		PARSE_ELEMENT(limit,"Limit");
		PARSE_ELEMENT(spring,"Spring");
// 		PhysicXMLSerializer::Parse(e,&limit);
// 		PhysicXMLSerializer::Parse(e,&spring);
	}
	void IPhysicalJointRevoluteDesc::SaveToXML(xml::XMLElement* e)
	{
		IPhysicalJoint3DDesc::SaveToXML(e);
		WRITE_ATTRIBUTE(limitEnable,"LimitEnable",DefaultLimitEnable);
		WRITE_ATTRIBUTE(motorEnable,"MotorEnable",DefaultMotorEnable);
		WRITE_ATTRIBUTE(springEnable,"SpringEnable",DefaultSpringEnable);
		WRITE_ATTRIBUTE(projectionDistance,"ProjectionDistance",DefaultProjectionDistance);
		WRITE_ATTRIBUTE(projectionAngle,"ProjectionAngle",DefaultProjectionAngle);

		WRITE_ELEMENT(limit,"Limit");
		WRITE_ELEMENT(spring,"Spring");
// 		PhysicXMLSerializer::Save(e,&limit);
// 		PhysicXMLSerializer::Save(e,&spring);

	}

	const float IPhysicalJointPulleyDesc::DefaultDistance=0;
	const float IPhysicalJointPulleyDesc::DefaultStiffness=1;
	const float IPhysicalJointPulleyDesc::DefaultRatio=1;
	const bool IPhysicalJointPulleyDesc::DefaultIsRigid=0;
	const bool IPhysicalJointPulleyDesc::DefaultMotorEnabled=0;
	void IPhysicalJointPulleyDesc::LoadFromXML(xml::XMLElement *e)
	{
		IPhysicalJoint3DDesc::LoadFromXML(e);
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(pulley[0],"PulleyA");
		PARSE_ATTRIBUTE(pulley[1],"PulleyB");
		PARSE_ATTRIBUTE(distance,"Distance");
		PARSE_ATTRIBUTE(stiffness,"Stiffness");
		PARSE_ATTRIBUTE(ratio,"Ratio");
		PARSE_ATTRIBUTE(isRigid,"IsRigid");
		PARSE_ATTRIBUTE(motorEnable,"MotorEnable");
	}
	void IPhysicalJointPulleyDesc::SaveToXML(xml::XMLElement* e)
	{
		IPhysicalJoint3DDesc::SaveToXML(e);
		WRITE_ATTRIBUTE(pulley[0],"PulleyA",0);
		WRITE_ATTRIBUTE(pulley[1],"PulleyB",0);
		WRITE_ATTRIBUTE(distance,"Distance",DefaultDistance);
		WRITE_ATTRIBUTE(stiffness,"Stiffness",DefaultStiffness);
		WRITE_ATTRIBUTE(ratio,"Ratio",DefaultRatio);
		WRITE_ATTRIBUTE(isRigid,"IsRigid",DefaultIsRigid);
		WRITE_ATTRIBUTE(motorEnable,"MotorEnable",DefaultMotorEnabled);

	}

	const float IPhysicalJointD6Desc::DefaultProjectionDistance= 0.01f;
	const float IPhysicalJointD6Desc::DefaultProjectionAngle= 0.0872f;
	const float IPhysicalJointD6Desc::DefaultGearRatio=1;
	const EJointProjectionMode IPhysicalJointD6Desc::DefaultProjectionMode= EJPM_None;
	void IPhysicalJointD6Desc::LoadFromXML(xml::XMLElement *e)
	{
		IPhysicalJoint3DDesc::LoadFromXML(e);
		xml::XMLAttribute* attr;

		attr=e->getAttribute(mT("XMotion"));
		if(attr)xMotion= (ED6JointMotion)core::StringConverter::toEnum(mT("ED6JointMotion"),attr->value);
		attr=e->getAttribute(mT("YMotion"));
		if(attr)yMotion= (ED6JointMotion)core::StringConverter::toEnum(mT("ED6JointMotion"),attr->value);
		attr=e->getAttribute(mT("ZMotion"));
		if(attr)zMotion= (ED6JointMotion)core::StringConverter::toEnum(mT("ED6JointMotion"),attr->value);
		attr=e->getAttribute(mT("Swing1Motion"));
		if(attr)swing1Motion= (ED6JointMotion)core::StringConverter::toEnum(mT("ED6JointMotion"),attr->value);
		attr=e->getAttribute(mT("Swing2Motion"));
		if(attr)swing2Motion= (ED6JointMotion)core::StringConverter::toEnum(mT("ED6JointMotion"),attr->value);
		attr=e->getAttribute(mT("TwistMotion"));
		if(attr)twistMotion= (ED6JointMotion)core::StringConverter::toEnum(mT("ED6JointMotion"),attr->value);

		PARSE_ATTRIBUTE(drivePosition,"drivePosition");
		PARSE_ATTRIBUTE(driveOrientation,"driveOrientation");
		PARSE_ATTRIBUTE(driveLinearVelocity,"driveLinearVelocity");
		PARSE_ATTRIBUTE(driveAngularVelocity,"driveAngularVelocity");
		PARSE_ATTRIBUTE(projectionMode,"projectionMode");
		PARSE_ATTRIBUTE(projectionDistance,"projectionDistance");
		PARSE_ATTRIBUTE(projectionAngle,"projectionAngle");
		PARSE_ATTRIBUTE(gearRatio,"gearRatio");
		PARSE_ATTRIBUTE(slerpDriveFlag,"slerpDriveFlag");
		PARSE_ATTRIBUTE(gearEnabledFlag,"gearEnabledFlag");


		PARSE_ELEMENT(linearLimit,"LinearLimit");
		PARSE_ELEMENT(swing1Limit,"Swing1Limit");
		PARSE_ELEMENT(swing2Limit,"Swing2Limit");
		PARSE_ELEMENT(twistLimit,"TwistLimit");
		PARSE_ELEMENT(xDrive,"xDrive");
		PARSE_ELEMENT(yDrive,"yDrive");
		PARSE_ELEMENT(zDrive,"zDrive");
		PARSE_ELEMENT(swingDrive,"SwingDrive");
		PARSE_ELEMENT(twistDrive,"TwistDrive");
		PARSE_ELEMENT(slerpDrive,"SlerpDrive");
		/*
		PhysicXMLSerializer::Parse(e,&linearLimit);
		PhysicXMLSerializer::Parse(e,&swing1Limit);
		PhysicXMLSerializer::Parse(e,&swing2Limit);
		PhysicXMLSerializer::Parse(e,&twistLimit);
		PhysicXMLSerializer::Parse(e,&xDrive);
		PhysicXMLSerializer::Parse(e,&yDrive);
		PhysicXMLSerializer::Parse(e,&zDrive);
		PhysicXMLSerializer::Parse(e,&swingDrive);
		PhysicXMLSerializer::Parse(e,&twistDrive);
		PhysicXMLSerializer::Parse(e,&slerpDrive);*/
	}
	void IPhysicalJointD6Desc::SaveToXML(xml::XMLElement* e)
	{
		IPhysicalJoint3DDesc::SaveToXML(e);
		e->addAttribute(mT("XMotion"),core::StringConverter::toString(xMotion,core::string(mT("ED6JointMotion"))));
		e->addAttribute(mT("YMotion"),core::StringConverter::toString(yMotion,core::string(mT("ED6JointMotion"))));
		e->addAttribute(mT("ZMotion"),core::StringConverter::toString(zMotion,core::string(mT("ED6JointMotion"))));
		e->addAttribute(mT("Swing1Motion"),core::StringConverter::toString(swing1Motion,core::string(mT("ED6JointMotion"))));
		e->addAttribute(mT("Swing2Motion"),core::StringConverter::toString(swing2Motion,core::string(mT("ED6JointMotion"))));
		e->addAttribute(mT("TwistMotion"),core::StringConverter::toString(twistMotion,core::string(mT("ED6JointMotion"))));


		WRITE_ATTRIBUTE(drivePosition,"drivePosition",0);
		WRITE_ATTRIBUTE(driveOrientation,"driveOrientation",math::quaternion::Identity);
		WRITE_ATTRIBUTE(driveLinearVelocity,"driveLinearVelocity",0);
		WRITE_ATTRIBUTE(driveAngularVelocity,"driveAngularVelocity",0);
		WRITE_ATTRIBUTE(projectionMode,"projectionMode",EJPM_None);
		WRITE_ATTRIBUTE(projectionDistance,"projectionDistance",DefaultProjectionDistance);
		WRITE_ATTRIBUTE(projectionAngle,"projectionAngle",DefaultProjectionAngle);
		WRITE_ATTRIBUTE(gearRatio,"gearRatio",DefaultGearRatio);
		WRITE_ATTRIBUTE(slerpDriveFlag,"slerpDriveFlag",0);
		WRITE_ATTRIBUTE(gearEnabledFlag,"gearEnabledFlag",0);

		WRITE_ELEMENT(linearLimit,"LinearLimit");
		WRITE_ELEMENT(swing1Limit,"Swing1Limit");
		WRITE_ELEMENT(swing2Limit,"Swing2Limit");
		WRITE_ELEMENT(twistLimit,"TwistLimit");
		WRITE_ELEMENT(xDrive,"xDrive");
		WRITE_ELEMENT(yDrive,"yDrive");
		WRITE_ELEMENT(zDrive,"zDrive");
		WRITE_ELEMENT(swingDrive,"SwingDrive");
		WRITE_ELEMENT(twistDrive,"TwistDrive");
		WRITE_ELEMENT(slerpDrive,"SlerpDrive");
		/*
		PhysicXMLSerializer::Save(e,&linearLimit);
		PhysicXMLSerializer::Save(e,&swing1Limit);
		PhysicXMLSerializer::Save(e,&swing2Limit);
		PhysicXMLSerializer::Save(e,&twistLimit);
		PhysicXMLSerializer::Save(e,&xDrive);
		PhysicXMLSerializer::Save(e,&yDrive);
		PhysicXMLSerializer::Save(e,&zDrive);
		PhysicXMLSerializer::Save(e,&swingDrive);
		PhysicXMLSerializer::Save(e,&twistDrive);
		PhysicXMLSerializer::Save(e,&slerpDrive);*/

	}
	const float PhysicMaterialDesc::DefaultDynamicFriction=0;
	const float PhysicMaterialDesc::DefaultStaticFriction=0;
	const float PhysicMaterialDesc::DefaultDynamicFrictionV=0;
	const float PhysicMaterialDesc::DefaultStaticFrictionV=0;
	const float PhysicMaterialDesc::DefaultRestitution=0;
	const math::vector3d PhysicMaterialDesc::DefaultDirOfAnisotropy;
	const EMaterialCombineMode PhysicMaterialDesc::DefaultFrictionCombineMode=ECM_Average;
	const EMaterialCombineMode PhysicMaterialDesc::DefaultRestitutionCombineMode=ECM_Average;
	void PhysicMaterialDesc::LoadFromXML(xml::XMLElement *e)
	{
		xml::XMLAttribute* attr;

		PARSE_ATTRIBUTE(name,"Name");
		PARSE_ATTRIBUTE(dynamicFriction,"DynamicFriction");
		PARSE_ATTRIBUTE(staticFriction,"StaticFriction");
		PARSE_ATTRIBUTE(dynamicFrictionV,"DynamicFrictionV");
		PARSE_ATTRIBUTE(staticFrictionV,"StaticFrictionV");
		PARSE_ATTRIBUTE(restitution,"Restitution");
		PARSE_ATTRIBUTE(dirOfAnisotropy,"AnisotropyDirection");

		PARSE_ATTRIBUTE(flags[EMF_Anisotropic],"AnisotropicFlag");
		PARSE_ATTRIBUTE(flags[EMF_DisableFriction],"DisableFrictionFlag");
		PARSE_ATTRIBUTE(flags[EMF_DisableStrongFriction],"DisableStrongFrictionFlag");
	}
	void PhysicMaterialDesc::SaveToXML(xml::XMLElement* e)
	{
		WRITE_ATTRIBUTE(name,"Name",mT(""));
		WRITE_ATTRIBUTE(dynamicFriction,"DynamicFriction",DefaultDynamicFriction);
		WRITE_ATTRIBUTE(staticFriction,"StaticFriction",DefaultStaticFriction);
		WRITE_ATTRIBUTE(dynamicFrictionV,"DynamicFrictionV",DefaultDynamicFrictionV);
		WRITE_ATTRIBUTE(staticFrictionV,"StaticFrictionV",DefaultStaticFrictionV);
		WRITE_ATTRIBUTE(restitution,"Restitution",DefaultRestitution);
		WRITE_ATTRIBUTE(dirOfAnisotropy,"AnisotropyDirection",DefaultDirOfAnisotropy);

		if(flags[EMF_Anisotropic])
			WRITE_ATTRIBUTE(core::string(mT("true")),"AnisotropicFlag",mT("false"));
		if(flags[EMF_DisableFriction])
			WRITE_ATTRIBUTE(core::string(mT("true")),"DisableFrictionFlag",mT("false"));
		if(flags[EMF_DisableStrongFriction])
			WRITE_ATTRIBUTE(core::string(mT("true")),"DisableStrongFrictionFlag",mT("false"));
	}
	//////////////////////////////////////////////////////////////////////////
	PhysicalNodeDesc::~PhysicalNodeDesc()
	{
		for(int i=0;i<shapes.size();++i)
		{
			if(shapes[i]->autoDelete)
			{
				delete shapes[i];
			}
		}
		if(bodyDesc!=0 && bodyDesc->autoDelete)
		{
			delete bodyDesc;
		}
	}

	const float PhysicalNodeDesc::DefaultDensity=0;
	void PhysicalNodeDesc::SaveToXML(xml::XMLElement *e)
	{

		WRITE_ATTRIBUTE(name,"Name",mT(""));
		WRITE_ATTRIBUTE(globalPos,"GlobalPos",math::matrix4x4::Identity);
		WRITE_ATTRIBUTE(density,"Density",DefaultDensity);
		WRITE_ATTRIBUTE(actorGroupID,"ActorGroupID",0);
		if(bodyDesc)
		{
			PhysicXMLSerializer::Save(e,bodyDesc);
		}
		if(shapes.size()>0)
		{
			xml::XMLElement* elem=new xml::XMLElement(mT("Shapes"));
			e->addSubElement(elem);
			for(int i=0;i<shapes.size();++i)
			{
				xml::XMLElement* ee=new xml::XMLElement(mT("Shape"));
				shapes[i]->SaveToXML(ee);
				elem->addSubElement(ee);
			}
		}
	}
	void PhysicalNodeDesc::LoadFromXML(xml::XMLElement* e)
	{
		xml::XMLAttribute* attr;
		PARSE_ATTRIBUTE(name,"Name");
		PARSE_ATTRIBUTE(globalPos,"GlobalPos");
		PARSE_ATTRIBUTE(density,"Density");
		PARSE_ATTRIBUTE(actorGroupID,"ActorGroupID");
		bodyDesc=PhysicXMLSerializer::PhysicalBody_Load(e);
		if(bodyDesc)
			bodyDesc->autoDelete=true;
		xml::XMLElement* elem= e->getSubElement(mT("Shapes"));
		if(elem)
		{
			elem=elem->getSubElement(mT("Shape"));
			while(elem)
			{
				xml::XMLAttribute*attr=elem->getAttribute(mT("Type"));
				if(attr)
				{
					IPhysicalShapeDesc* s=0;
					if(attr->value==mT("Sphere"))
					{
						s=new SphereShapeDesc();
					}else if(attr->value==mT("Box"))
					{
						s=new BoxShapeDesc();
					}else if(attr->value==mT("Plane"))
					{
						s=new PlaneShapeDesc();
					}else if(attr->value==mT("Terrain"))
					{
						s=new TerrainShapeDesc();
					}else if(attr->value==mT("Capsule"))
					{
						s=new CapsuleShapeDesc();
					}else if(attr->value==mT("Convex"))
					{
						s=new ConvexShapeDesc();
					}else if(attr->value==mT("Triangle"))
					{
						s=new TriangleShapeDesc();
					}
					if(s)
					{
						s->autoDelete=true;
						s->LoadFromXML(elem);
						shapes.push_back(s);
					}
				}
				elem=elem->nextSiblingElement(mT("Shape"));
			}
		}
	}
}
}



