#include "stdafx.h"

#include "IAIPhysics.h"
#include "IRenderDevice.h"
#include "DefaultColors.h"
#include "AIComponentTypes.h"

namespace mray{
namespace AI{

	IAIPhysics::IAIPhysics()
	{
	}
	IAIPhysics::~IAIPhysics()
	{
	}
void IAIPhysics::Visualize(IRenderDevice*dev)
{
	dev->drawLine(GetGlobalPosition(),GetGlobalPosition()+GetLinearVelocity(),video::DefaultColors::Blue);
}


}
}
