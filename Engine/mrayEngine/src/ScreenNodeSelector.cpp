#include "stdafx.h"

#include "ScreenNodeSelector.h"
#include "CameraNode.h"
#include "SceneHelper.h"


namespace mray{
namespace scene{

ScreenNodeSelector::ScreenNodeSelector()
{
}
ScreenNodeSelector::~ScreenNodeSelector()
{
}
void ScreenNodeSelector::setup(const math::vector2di&coord,CameraNode*camera,const MovableNodeList& nodes,uint idMask)
{
	math::line3d r= SceneHelper::getInstance().getRayFromScreenCoord(coord,camera);
	RayNodeSelector::setup(r,nodes,idMask);
}


}
}