


/********************************************************************
	created:	2010/07/27
	created:	27:7:2010   20:51
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\VegetationTest\ObjectRenderer.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\VegetationTest
	file base:	ObjectRenderer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ObjectRenderer_h__
#define ObjectRenderer_h__

#include "IRenderTarget.h"
#include "IRenderable.h"

namespace mray
{

class ObjectRenderer
{
private:
protected:
	video::IVideoDevice*m_device;
public:
	ObjectRenderer(video::IVideoDevice* device);
	virtual ~ObjectRenderer();
	
	GCPtr<video::IRenderTarget> RenderObject(scene::IRenderable*node,int count,const math::vector2di& size);
};

}
#endif // ObjectRenderer_h__
