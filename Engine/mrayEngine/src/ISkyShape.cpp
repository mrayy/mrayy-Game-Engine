#include "stdafx.h"

#include "ISkyShape.h"
#include "RenderManager.h"

namespace mray{
namespace scene{

ISkyShape::ISkyShape():MeshRenderableNode(0)
{
	SetTargetRenderGroup(RGH_Skies);
	SetHasCustomRenderGroup(true);
}
ISkyShape::~ISkyShape(){

}

}
}
