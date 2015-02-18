

#ifndef __PhysXAPI__
#define __PhysXAPI__
/********************************************************************
	created:	2009/07/15
	created:	15:7:2009   12:42
	filename: 	c:\MyFiles\Programming\mrayEngine_Prj\mrayPhysx\mrayPhysx\PhysXAPI.h
	file path:	c:\MyFiles\Programming\mrayEngine_Prj\mrayPhysx\mrayPhysx
	file base:	PhysXAPI
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#include <IPhysicManager.h>
#include "CompileConfig.h"

namespace mray{
namespace physics{

MRAY_PHYSX_DLL IPhysicManager* createPhysXManager(PhysicsSystemDesc* desc);

}
}

#endif