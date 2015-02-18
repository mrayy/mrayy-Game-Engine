 

#include "stdafx.h"
#include "ObjectRenderer.h"
#include "IVideoDevice.h"
#include "ISceneNode.h"
#include "MathUtil.h"


namespace mray
{

ObjectRenderer::ObjectRenderer(video::IVideoDevice* device)
{
	m_device=device;
}
ObjectRenderer::~ObjectRenderer()
{
}

GCPtr<video::IRenderTarget> ObjectRenderer::RenderObject(scene::IRenderable*node,int count,const math::vector2di& size)
{
	GCPtr<video::ITexture> resultTex=m_device->createTexture2D(size,video::EPixel_R8G8B8A8,true);
	GCPtr<video::IRenderTarget> resultRt=m_device->createRenderTarget(mT(""),resultTex,0,0,0);

	int texPerEdge=ceil(sqrt((float)count));
	math::vector2di subTexSize;
	subTexSize.x=size.x/texPerEdge;
	subTexSize.y=size.y/texPerEdge;

	math::vector3d eyePos;
	math::matrix4x4 projection;

	math::box3d bbox= node->getOwner()->getTransformedBoundingBox();
	float maxLength=math::Max(bbox.getWidth(),math::Max(bbox.getHeight(),bbox.getDepth()));

	projection=math::MathUtil::CreateProjectionMatrixPerspectiveFov(math::toRad(90),1,0.1,maxLength+10);

	return resultRt;
}


}
