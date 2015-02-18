
/********************************************************************
created:	2009/02/05
created:	5:2:2009   19:27
filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IPhysical3DDef.h
file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
file base:	IPhysical3DDef
file ext:	h
author:		Mohamad Yamen Saraiji

purpose:	
*********************************************************************/

#ifndef ___IPhysical3DDef___
#define ___IPhysical3DDef___

#include <Point3d.h>
#include <quaternion.h>
#include "common.h"
#include "mTypes.h"

namespace mray{
	namespace xml{
		class XMLElement;
	}
	class IObject;
	namespace physics{

		class IPhysicalNode;
		class IPhysicMaterial;
		class IPhysicalShapeDesc;
		class IPhysicSceneQueryReport;

		typedef ushort GroupID;
		typedef ushort MaterialID;

		#define PHYICS_SLEEP_INTERVAL (20.0f*0.02f)		// Corresponds to 20 frames for the standard time step.

		enum EShapeType
		{
			EShape_Fixed=(1<<0),
			EShape_Dynamic=(1<<1),
			EShape_All=EShape_Fixed | EShape_Dynamic 
		};

		enum EForceMode
		{
			EFM_Force,                   //!< parameter has unit of mass * distance/ time^2, i.e. a force
			EFM_Impulse,                 //!< parameter has unit of mass * distance /time
			EFM_VelocityChange,			//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
			EFM_SmoothImpulse,          //!< same as EFM_Impulse but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
			EFM_SmoothVelocityChange,	//!< same as EFM_VelocityChange but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
			EFM_Acceleration				//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
		};
		enum ESupported3DJoints{
			ESJ3D_RevoluteJoint,	//Also known as a hinge joint, permits one rotational degree of freedom.
			ESJ3D_PrismaticJoint,	//Permits a single translational degree of freedom.
			ESJ3D_DistanceJoint,	//A point on one actor maintains a certain distance range to another point on another actor.
			ESJ3D_PulleyJoint,		//A pulley joint.
			ESJ3D_FixedJoint,		//A fixed connection.
			ESJ3D_GearJoint,		//Gear Joint
			ESJ3D_D6Joint,			//A D6 joint is a general constraint between two actors
			ESJ3D_Cylindrical,		// Formerly known as a sliding joint, permits one translational and one rotational degree of freedom.
			ESJ3D_Spherical,		// Also known as a ball or ball and socket joint.
			ESJ3D_PointOnLine,		//A point on one actor is constrained to stay on a line on another.
			ESJ3D_PointOnPlane,		//A point on one actor is constrained to stay on a plane on another.
		};
		enum ED6JointMotion{
			ED6Joint_Motion_Locked,
			ED6Joint_Motion_Limited,
			ED6Joint_Motion_Free
		};

		enum EJointFlags
		{
			EJFlag_CollisionEnable,
			EJFlag_Visualization,
			EJFlag_Flags_Count
		};

		enum EJointProjectionMode{
			EJPM_None,				// don't project this joint
			EJPM_PointMindist ,		// linear and angular minimum distance projection
			EJPM_LinearMindist 		// linear only minimum distance projection
		};

		enum EMaterialCombineMode
		{
			ECM_Average, //Average: (a + b)/2
			ECM_Min,	 //Minimum: min(a,b)
			ECM_Multiply,//Multiply: a*b
			ECM_Max,	 //Maximum: max(a,b)
			ECM_ModesCoun
		};
		enum EMaterialFlags
		{
			EMF_Anisotropic,
			EMF_DisableFriction,
			EMF_DisableStrongFriction,
			EMF_MaterialFlags_Count
		};

		enum ESimulationType
		{
			EST_Software,
			EST_Hardware
		};


		enum EBodyFlag
		{
			EBF_DisableGravity=BIT(0),

			EBF_Frozen_PosX=BIT(1),
			EBF_Frozen_PosY=BIT(2),
			EBF_Frozen_PosZ=BIT(3),

			EBF_Frozen_RotX=BIT(4),
			EBF_Frozen_RotY=BIT(5),
			EBF_Frozen_RotZ=BIT(6),

			EBF_Frozen_Pos=EBF_Frozen_PosX | EBF_Frozen_PosY | EBF_Frozen_PosZ,
			EBF_Frozen_Rot=EBF_Frozen_RotX | EBF_Frozen_RotY | EBF_Frozen_RotZ,
			EBF_Frozen = EBF_Frozen_Pos | EBF_Frozen_Rot,

			EBF_Kinematic = BIT(7),

			EBF_FilterSleepVel= BIT(8),

			EBF_EnergySleepTest= BIT(9)
		};
		enum EContactReportFlag
		{
			ECRF_StartTouch,
			ECRF_Touch,
			ECRF_EndTouch,
			ECRF_Impact,
			ECRF_Roll,
			ECRF_Slide,

			ECRF_StartTouchForceThreshold,
			ECRF_TouchThreshold,
			ECRF_EndTouchThreshold,
			
			ECRF_NotifyForces,

			ECRF_IgnorePair,

			ECRF_FlagsCount
		};

		enum ESceneQueryExecMode
		{
			ESQExec_Syncronous,
			ESQExec_Asyncronous
		};


		enum EShapeFlag
		{
			ESF_TriggerOnEnter=BIT(0),
			ESF_TriggerOnLeave=BIT(1),
			ESF_TriggerOnStay=BIT(2),
			ESF_TriggerEnabled=ESF_TriggerOnEnter | ESF_TriggerOnLeave | ESF_TriggerOnStay,

			ESF_DisableCollision=BIT(3),
			ESF_DisableRaycasting=BIT(4),

			ESF_FluidDrain=BIT(5),
			ESF_FluidDisableCollision=BIT(6),
			ESF_FluidTwoWay=BIT(7),

			ESF_ClothDrain=BIT(8),
			ESF_ClothDisableCollision=BIT(9),
			ESF_ClothTwoWay=BIT(10),

			ESF_SoftbodyDrain=BIT(11),
			ESF_SoftbodyDisableCollision=BIT(12),
			ESF_SoftbodyTwoWay=BIT(13),

			ESF_DisableCollisionResponse=BIT(14),
			ESF_DynamicDynamicCCD=BIT(15),
			ESF_DisableSceneQueries=BIT(16),
			ESF_ContactForce=BIT(17)
		};

		typedef EShapeFlag ETriggerStatus;

		class CollisionMask
		{
		public:
			CollisionMask()
			{
				bits[0]=bits[1]=bits[2]=bits[3]=0;
			}
			uint bits[4];
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL PhysicsSystemDesc
		{
		public:
			PhysicsSystemDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				maxTimestep			= DefaultMaxStep;
				maxIter				= DefaultMaxIter;
				maxBounds			= 0;
				simType				= DefaultSimType;
				groundPlane			= DefaultGroundPlane;
				boundsPlanes		= DefaultBoundsPlanes;
				upAxis				= DefaultUpAxis;

				useFixedTimeStep	= DefaultFixedTimeStep;
			}

			math::vector3d gravity;

			float maxTimestep;
			bool useFixedTimeStep;

			uint maxIter;

			math::box3d* maxBounds;

			ESimulationType simType;

			bool groundPlane;

			bool boundsPlanes;

			//1 for Y,2 for Z,0 disable it
			uint upAxis;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const math::vector3d DefaultGravity;
			static const float DefaultMaxStep;
			static const uint DefaultMaxIter=8;
			static const bool DefaultFixedTimeStep=0;
			static const ESimulationType DefaultSimType=EST_Software;
			static const bool DefaultGroundPlane=0;
			static const bool DefaultBoundsPlanes=0;
			static const uint DefaultUpAxis=0;
		};

		//////////////////////////////////////////////////////////////////////////

		class MRAY_DLL PhysicalBodyDesc
		{
		public:
			PhysicalBodyDesc(){
				autoDelete=false;
				setToDefault();
			}
			virtual void setToDefault(){
				wakeupCounter			= DefaultWakeupCounter;
				mass					= DefaultMass;
				linearDamping			= DefaultLinearDamping;
				angularDamping			= DefaultAngularDamping;
				maxAngularVelocity		= DefaultMaxAngularVelocity;
				sleepLinearVelocity		= DefaultSleepLinearVelocity;
				sleepAngularVelocity	= DefaultSleepAngularVelocity;
				CCDMotionThreshold		= DefaultCCDMotionThreshold;
				solverIterationCount    = DefaultSolverIterationCount;
				sleepEnergyThreshold	= DefaultSleepEnergyThreshold;
				sleepDamping			= DefaultSleepDamping;
				contactReportThreshold  = DefaultContactReportThreshold;
				linearVelocity			= DefaultLinearVelocity;
				angularVelocity			= DefaultAnglarVelocity;

				kinematic				= false;
				disableGravity			= false;
			}
			bool autoDelete;

			math::matrix4x4 massLocalPos;
			math::vector3d massSpaceInertia;
			float mass;
			math::vector3d linearVelocity;
			math::vector3d angularVelocity;

			float wakeupCounter;
			float linearDamping;
			float angularDamping;
			float maxAngularVelocity;
			float CCDMotionThreshold;
			float sleepLinearVelocity;
			float sleepAngularVelocity;

			int solverIterationCount;
			float sleepEnergyThreshold;
			float sleepDamping;
			float contactReportThreshold;

			bool kinematic;
			bool disableGravity;


			static const float DefaultMass;
			static const math::vector3d DefaultLinearVelocity;
			static const math::vector3d DefaultAnglarVelocity;
			static const float DefaultWakeupCounter;
			static const float DefaultLinearDamping;
			static const float DefaultAngularDamping;
			static const float DefaultMaxAngularVelocity;
			static const float DefaultCCDMotionThreshold;
			static const float DefaultSleepLinearVelocity;
			static const float DefaultSleepAngularVelocity;
			static const int   DefaultSolverIterationCount;
			static const float DefaultSleepEnergyThreshold;
			static const float DefaultSleepDamping;
			static const float DefaultContactReportThreshold;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};


		class MRAY_DLL PhysicalNodeDesc
		{
		public:
			PhysicalNodeDesc(){
				setToDefault();
			}
			virtual~PhysicalNodeDesc();
			virtual void setToDefault(){
				bodyDesc=0;
				density=DefaultDensity;
				userData=0;
				actorGroupID=0;
				for(int i=0;i<ECRF_FlagsCount;++i)
					contactFlags[i]=false;
			}

			core::string name;

			math::matrix4x4 globalPos;

			//null for static objects
			PhysicalBodyDesc* bodyDesc;

			float density;

			GroupID actorGroupID;

			std::vector<IPhysicalShapeDesc*> shapes;
			IObject*userData;

			bool contactFlags[ECRF_FlagsCount];

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const float DefaultDensity;

		};

		//////////////////////////////////////////////////////////////////////////

		class MRAY_DLL PhysicalSpringDesc
		{
		public:
			PhysicalSpringDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				spring=DefaultSpring;
				damper=DefaultDamper;
				targetValue=DefaultTargetValue;
			}

			float spring;
			float damper;
			float targetValue;

			static const float DefaultSpring;
			static const float DefaultDamper;
			static const float DefaultTargetValue;


			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};

		class MRAY_DLL PhysicalJointLimitDesc
		{
		public:
			PhysicalJointLimitDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				value = DefaultValue;
				restitution = DefaultRestitution;
				hardness = DefaultHardness;
			}
			float value;
			float restitution;
			float hardness;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const float DefaultValue;
			static const float DefaultRestitution;
			static const float DefaultHardness;
		};
		class MRAY_DLL PhysicalJointLimitPairDesc
		{
		public:
			PhysicalJointLimitPairDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
			}

			PhysicalJointLimitDesc low;
			PhysicalJointLimitDesc high;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};

		class MRAY_DLL PhysicalMotor3DDesc
		{
		public:
			PhysicalMotor3DDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				velTarget=DefaultVelTarget;
				maxForce=DefaultMaxForce;
				freeSpin=DefaultFreeSpin;
			}

			float velTarget;
			float maxForce;
			float freeSpin;

			static const float DefaultVelTarget;
			static const float DefaultMaxForce;
			static const float DefaultFreeSpin;
			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};


		class MRAY_DLL PhysicalJointLimitSoftDesc
		{
		public:
			PhysicalJointLimitSoftDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				value = DefaultValue;
				restitution = DefaultRestitution;
				spring = DefaultSpring;
				damping = DefaultDamping;
			}

			float value;
			float restitution;
			float spring;
			float damping;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);


			static const float DefaultValue;
			static const float DefaultRestitution;
			static const float DefaultSpring;
			static const float DefaultDamping;
		};


		class MRAY_DLL PhysicalJointLimitSoftPairDesc
		{
		public:
			PhysicalJointLimitSoftPairDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				low.setToDefault();
				high.setToDefault();
			}

			PhysicalJointLimitSoftDesc low,high;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};

		class MRAY_DLL PhysicalJointDriveDesc
		{
		public:
			PhysicalJointDriveDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				spring = DefaultSpring;
				damping = DefaultDamping;
				forceLimit = DefaultForceLimit;
				drivePosition = DefaultDrivePosition;
				driveVelocity = DefaultDriveVelocity;
			}
			float		spring;
			float		damping;
			float		forceLimit;
			bool		drivePosition;
			bool		driveVelocity;


			static const float DefaultSpring;
			static const float DefaultDamping;
			static const float DefaultForceLimit;
			static const bool DefaultDrivePosition;;
			static const bool DefaultDriveVelocity;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL IPhysicalJoint3DDesc
		{
		private:
		public:
			IPhysicalJoint3DDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				for (int i=0; i<2; i++)
				{
					localAxis[i]=DefaultLocalAxis;
					localNormal[i]=DefaultLocalNormal;
					localAnchor[i]=DefaultLocalAnchor;
				}
				m_fromGlobalAxis=0;
				m_fromGlobalAnchor=0;

				body1=body2=0;
				maxForce	= DefaultMaxForce;	// very big val
				maxTorque	= DefaultMaxTorque;	// very big val
				solverExtrapolationFactor = DefaultSolverExtrapolationFactor;
				useAccelerationSpring = DefaultUseAccelerationSpring;
				userData	= 0;

				flags[EJFlag_CollisionEnable]=false;
				flags[EJFlag_Visualization]=false;
			}

			void setFromGlobalAxis(const math::vector3d&a){
				m_fromGlobalAxis=true;
				m_globalAxis=a;
			}
			void setFromGlobalAnchor(const math::vector3d&a){
				m_fromGlobalAnchor=true;
				m_globalAnchor=a;
			}

			virtual ESupported3DJoints getType()=0;

			/// Use this to attach application specific data to your joints.
			IObject* userData;

			/// The first attached body.
			IPhysicalNode* body1;

			/// The second attached body.
			IPhysicalNode* body2;

			math::vector3d localNormal[2];
			math::vector3d localAxis[2];
			math::vector3d localAnchor[2];
			float maxForce;
			float maxTorque;
			float solverExtrapolationFactor;
			int useAccelerationSpring;

			bool flags[EJFlag_Flags_Count];

			bool m_fromGlobalAxis;
			bool m_fromGlobalAnchor;
			math::vector3d m_globalAxis;
			math::vector3d m_globalAnchor;

			virtual void LoadFromXML(xml::XMLElement*e);
			virtual void SaveToXML(xml::XMLElement*e);

			static IPhysicalJoint3DDesc* CreateJoint(const core::string& type);
			static IPhysicalJoint3DDesc* CreateJoint(ESupported3DJoints type);

			static const math::vector3d DefaultLocalNormal;
			static const math::vector3d DefaultLocalAxis;
			static const math::vector3d DefaultLocalAnchor;
			static const float DefaultMaxForce;
			static const float DefaultMaxTorque;
			static const float DefaultSolverExtrapolationFactor;
			static const bool DefaultUseAccelerationSpring;
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL IPhysicalJointDistanceDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointDistanceDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
				maxDistance=DefaultMaxDistanceEnable;
				minDistance=DefaultMinDistance;
				spring.setToDefault();
				maxDistanceEnable=DefaultMaxDistanceEnable;
				minDistanceEnable=DefaultMinDistanceEnable;
				springEnable=DefaultSpringEnable;
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_DistanceJoint;
			}

			float maxDistance;
			float minDistance;
			PhysicalSpringDesc spring;

			bool maxDistanceEnable;
			bool minDistanceEnable;
			bool springEnable;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const float DefaultMaxDistance;
			static const float DefaultMinDistance;
			static const bool DefaultMaxDistanceEnable;
			static const bool DefaultMinDistanceEnable;
			static const bool DefaultSpringEnable;
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL IPhysicalJointSphericalDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointSphericalDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();

				swingAxis=DefaultSwingAxis;

				twistLimit.setToDefault();
				swingLimit.setToDefault();
				twistSpring.setToDefault();
				swingSpring.setToDefault();
				jointSpring.setToDefault();

				projectionDistance = DefaultProjectionDistance;
				projectionMode = DefaultProjectionMode;
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_Spherical;
			}

			math::vector3d swingAxis;
			float projectionDistance;
			PhysicalJointLimitPairDesc twistLimit;
			PhysicalJointLimitDesc swingLimit;
			PhysicalSpringDesc	 twistSpring;
			PhysicalSpringDesc	 swingSpring;
			PhysicalSpringDesc	 jointSpring;
			EJointProjectionMode projectionMode;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const math::vector3d DefaultSwingAxis;
			static const float DefaultProjectionDistance;
			static const EJointProjectionMode DefaultProjectionMode;
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL IPhysicalJointRevoluteDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointRevoluteDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
				limitEnable=DefaultLimitEnable;
				motorEnable=DefaultMotorEnable;
				springEnable=DefaultSpringEnable;
				projectionDistance = DefaultProjectionDistance;
				projectionAngle = DefaultProjectionAngle;	//about 5 degrees in radians.
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_RevoluteJoint;
			}
			PhysicalJointLimitPairDesc limit;
			PhysicalSpringDesc spring;
			float projectionDistance;
			float projectionAngle;

			bool limitEnable;
			bool motorEnable;
			bool springEnable;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const float DefaultProjectionDistance;
			static const float DefaultProjectionAngle;
			static const bool DefaultLimitEnable;
			static const bool DefaultMotorEnable;
			static const bool DefaultSpringEnable;
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL IPhysicalJointPulleyDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointPulleyDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
				pulley[0]=0;
				pulley[1]=0;
				distance=DefaultDistance;
				stiffness=DefaultStiffness;
				ratio=DefaultRatio;
				isRigid=DefaultIsRigid;
				motorEnable=DefaultMotorEnabled;
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_PulleyJoint;
			}
			math::vector3d pulley[2];
			float distance;
			float stiffness;
			float ratio;

			bool isRigid;
			bool motorEnable;

			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);

			static const float DefaultDistance;
			static const float DefaultStiffness;
			static const float DefaultRatio;
			static const bool DefaultIsRigid;
			static const bool DefaultMotorEnabled;

		};

		//////////////////////////////////////////////////////////////////////////
		class IPhysicalJointPrismaticDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointPrismaticDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_PrismaticJoint;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		class IPhysicalJointPointOnLineDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointPointOnLineDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_PointOnLine;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		class IPhysicalJointCylinderDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointCylinderDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_Cylindrical;
			}
		};
		//////////////////////////////////////////////////////////////////////////
		class IPhysicalJointPointOnPlaneDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointPointOnPlaneDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_PointOnPlane;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		class IPhysicalJointFixedDesc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointFixedDesc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_FixedJoint;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		class MRAY_DLL IPhysicalJointD6Desc:public IPhysicalJoint3DDesc
		{
		public:
			IPhysicalJointD6Desc(){
				setToDefault();
			}
			virtual void setToDefault()
			{
				IPhysicalJoint3DDesc::setToDefault();
				xMotion = ED6Joint_Motion_Free;
				yMotion = ED6Joint_Motion_Free;
				zMotion = ED6Joint_Motion_Free;
				twistMotion = ED6Joint_Motion_Free;
				swing1Motion = ED6Joint_Motion_Free;
				swing2Motion = ED6Joint_Motion_Free;

				drivePosition=0;
				driveOrientation=math::quaternion::Identity;

				driveLinearVelocity.set(0,0,0);
				driveAngularVelocity.set(0,0,0);

				projectionMode = DefaultProjectionMode;	// choose EJPM_PointMindist to turn projection on
				projectionDistance = DefaultProjectionDistance;
				projectionAngle = DefaultProjectionAngle;	//about 5 degrees in radians.

				gearRatio = DefaultGearRatio;

				slerpDriveFlag=0;
				gearEnabledFlag=0;

			}

			virtual ESupported3DJoints getType(){
				return ESJ3D_D6Joint;
			}

			//Define the linear degrees of freedom
			ED6JointMotion xMotion,yMotion,zMotion;

			//Define the angular degrees of freedom
			ED6JointMotion swing1Motion, swing2Motion, twistMotion;

			//If some linear DOF are limited, linearLimit defines the characteristics of these limits
			PhysicalJointLimitSoftDesc		linearLimit;

			//If swing1Motion is ED6Joint_Motion_Limited, swing1Limit defines the characteristics of the limit
			PhysicalJointLimitSoftDesc		swing1Limit; 

			//If swing2Motion is ED6Joint_Motion_Limited, swing2Limit defines the characteristics of the limit
			PhysicalJointLimitSoftDesc		swing2Limit; 

			//If twistMotion is ED6Joint_Motion_Limited, twistLimit defines the characteristics of the limit
			PhysicalJointLimitSoftPairDesc		twistLimit; 


			// Drive the three linear DOF
			PhysicalJointDriveDesc		xDrive, yDrive, zDrive;

			// These drives are used if the flag NX_D6JOINT_SLERP_DRIVE is not set
			PhysicalJointDriveDesc		swingDrive, twistDrive;

			// This drive is used if the flag NX_D6JOINT_SLERP_DRIVE is set
			PhysicalJointDriveDesc		slerpDrive;


			// If the type of xDrive (yDrive,zDrive) is NX_D6JOINT_DRIVE_POSITION, drivePosition defines the goal position
			math::vector3d			drivePosition;


			// If the type of swingDrive or twistDrive is NX_D6JOINT_DRIVE_POSITION, driveOrientation defines the goal orientation
			math::quaternion		driveOrientation;

			// If the type of xDrive (yDrive,zDrive) is NX_D6JOINT_DRIVE_VELOCITY, driveLinearVelocity defines the goal linear velocity
			math::vector3d					driveLinearVelocity;

			// If the type of swingDrive or twistDrive is NX_D6JOINT_DRIVE_VELOCITY, driveAngularVelocity defines the goal angular velocity
			math::vector3d					driveAngularVelocity;


			// If projectionMode is NX_JPM_NONE, projection is disabled. If NX_JPM_POINT_MINDIST, bodies are projected to limits leaving an linear error of projectionDistance and an angular error of projectionAngle
			EJointProjectionMode	projectionMode;

			//The distance above which to project the joint.
			float					projectionDistance;	

			// The angle above which to project the joint.
			float					projectionAngle;	

			// when the flag NX_D6JOINT_GEAR_ENABLED is set, the angular velocity of the second actor is driven towards the angular velocity of the first actor times gearRatio (both w.r.t. their primary axis)
			float					gearRatio;

			bool slerpDriveFlag;
			bool gearEnabledFlag;

			static const EJointProjectionMode DefaultProjectionMode;
			static const float DefaultProjectionDistance;
			static const float DefaultProjectionAngle;
			static const float DefaultGearRatio;


			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};


		//////////////////////////////////////////////////////////////////////////

		class MRAY_DLL PhysicMaterialDesc
		{
		public:
			PhysicMaterialDesc(){
				setToDefault();
				autoDel=false;
			}
			virtual void setToDefault(){
				dynamicFriction=DefaultDynamicFriction;
				staticFriction=DefaultStaticFriction;
				dynamicFrictionV=DefaultDynamicFrictionV;
				staticFrictionV=DefaultStaticFrictionV;
				restitution=DefaultRestitution;
				dirOfAnisotropy=DefaultDirOfAnisotropy;
				for(int i=0;i<EMF_MaterialFlags_Count;++i)
					flags[i]=0;

				friction_combineMode=DefaultFrictionCombineMode;
				restitution_combineMode=DefaultRestitutionCombineMode;

			}

			bool autoDel;

			core::string name;

			float dynamicFriction;
			float staticFriction;
			float restitution;

			float dynamicFrictionV;
			float staticFrictionV;

			math::vector3d dirOfAnisotropy;

			bool flags[EMF_MaterialFlags_Count];

			EMaterialCombineMode friction_combineMode;
			EMaterialCombineMode restitution_combineMode;

			static const float DefaultDynamicFriction;
			static const float DefaultStaticFriction;
			static const float DefaultRestitution;
			static const float DefaultDynamicFrictionV;
			static const float DefaultStaticFrictionV;
			static const math::vector3d DefaultDirOfAnisotropy;
			static const EMaterialCombineMode DefaultFrictionCombineMode;
			static const EMaterialCombineMode DefaultRestitutionCombineMode;


			void LoadFromXML(xml::XMLElement*e);
			void SaveToXML(xml::XMLElement*e);
		};


		//////////////////////////////////////////////////////////////////////////
		class PhysicSceneQueryDesc
		{
		public:
			PhysicSceneQueryDesc():report(0),execMode(ESQExec_Syncronous)
			{
			}
			IPhysicSceneQueryReport* report;
			ESceneQueryExecMode execMode;
		};
	}
}


#endif //___IPhysical3DDef___
