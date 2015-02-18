
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   0:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PhysicalCCDesc.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PhysicalCCDesc
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysicalCCDesc___
#define ___PhysicalCCDesc___

#include "PhysicsCCCallBack.h"

namespace mray{
namespace physics{

	enum ECCType{
		ECCT_Box,
		ECCT_Capsule
	};
	enum ECCInteractionFlag
	{
		ECCI_Include ,	// Always collide character controllers
		ECCI_Exclude	// Never collide character controllers
	};
	enum ECCCapsuleClimbingMode
	{
		ECCCM_Easy,			//!< Standard mode, let the capsule climb over surfaces according to impact normal
		ECCCM_Constrained	//!< Constrained mode, try to limit climbing according to the step offset
	};

	enum ECCUpDir
	{
		ECCUD_X,
		ECCUD_Y,
		ECCUD_Z
	};

	class IPhysicsCCDesc
	{
	public:

		IPhysicsCCDesc(){
			setDefault();
		}

		virtual ECCType getType()=0;
		virtual void setDefault(){
			upDir			= ECCUD_Y;
			slopeLimit		= 0.707f;
			skinWidth		= 0.1f;
			stepOffset		= 0.5f;
			callback		= 0;
			interactionFlag	= ECCI_Include;
			position=0;
		}

		virtual bool isValid(){
			if(slopeLimit<0)
				return false;
			if(skinWidth<0)
				return false;
			if(stepOffset<0)
				return false;
			return true;
		}

		math::vector3d position;
		ECCUpDir upDir;
		float slopeLimit;
		float skinWidth;
		float stepOffset;
		ECCInteractionFlag interactionFlag;
		PhysicsCCCallBack *callback;
	};


	class PhysicsCCBoxDesc:public IPhysicsCCDesc
	{
	public:

		PhysicsCCBoxDesc(){
			setDefault();
		}


		virtual void setDefault(){
			extent.set(0.5,1,0.5);
		}
		virtual bool isValid(){
			if(!IPhysicsCCDesc::isValid())
				return false;
			if(extent.x<=0)return false;
			if(extent.y<=0)return false;
			if(extent.z<=0)return false;
			return true;
		}
		virtual ECCType getType(){
			return ECCT_Box;
		}

		math::vector3d extent;
	};

	class PhysicsCCCapsuleDesc:public IPhysicsCCDesc
	{
	public:

		PhysicsCCCapsuleDesc(){
			setDefault();
		}


		virtual void setDefault(){
			height=1;
			radius=0.5;
			climbingMode=ECCCM_Constrained;
		}
		virtual bool isValid(){
			if(!IPhysicsCCDesc::isValid())
				return false;
			if(radius<=0)return false;
			if(height<=0)return false;
			return true;
		}
		virtual ECCType getType(){
			return ECCT_Capsule;
		}

		float radius;
		float height;
		ECCCapsuleClimbingMode climbingMode;
	};
}
}


#endif //___PhysicalCCDesc___
