


/********************************************************************
	created:	2012/09/02
	created:	2:9:2012   23:23
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\CUBEMapRenderNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	CUBEMapRenderNode
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CUBEMapRenderNode___
#define ___CUBEMapRenderNode___

#include "ISceneNode.h"
#include "IRenderTarget.h"


namespace mray
{
namespace scene
{


class MRAY_DLL CUBEMapRenderNode:public ISceneNode
{
protected:
	
	video::IRenderTargetPtr m_faces[6];
public:
	CUBEMapRenderNode(const  core::string&name,int id,ISceneManager* sceneMngr);
	virtual~CUBEMapRenderNode();

	void CreateCubeMap(const math::vector2d& sz,video::EPixelFormat fmt);
	void SetCubeMap(const video::IRenderTargetPtr *faces);

	virtual void preRender();
	virtual void render(IRenderPass*pass);
	virtual void update(float dt);
	
};

}
}

#endif
