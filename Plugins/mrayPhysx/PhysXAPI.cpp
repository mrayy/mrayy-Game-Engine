#include "stdafx.h"

#include "PhysXAPI.h"
#include "PhysXManager.h"

namespace mray{
namespace physics{

IPhysicManager* createPhysXManager(PhysicsSystemDesc* desc){
	return new PhysXManager(desc);
}

}
}
