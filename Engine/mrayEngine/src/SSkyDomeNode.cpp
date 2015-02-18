#include "stdafx.h"


#include "SSkyDomeNode.h"
#include "ISceneManager.h"

#include "CameraNode.h"
#include "IVideoDevice.h"
#include "MeshGenerator.h"

#include "RenderManager.h"


namespace mray{
namespace scene{

SSkyDomeNode::SSkyDomeNode(const video::ITexturePtr& texture,int uSegments,int vSegments,
						   double texPercentage,double spherePercentage,float radius)
{

	MeshGenerator::getInstance().generateSkyDome(texture,uSegments,vSegments,texPercentage,spherePercentage,radius,getMesh());
}


SSkyDomeNode::~SSkyDomeNode()
{
}















}
}
