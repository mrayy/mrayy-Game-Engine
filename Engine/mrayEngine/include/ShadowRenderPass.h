
/********************************************************************
	created:	2010/02/23
	created:	23:2:2010   11:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ShadowRenderPass.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ShadowRenderPass
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ShadowRenderPass___
#define ___ShadowRenderPass___

#include "IRenderPass.h"
#include "IRenderTarget.h"
#include "GPUShader.h"
#include "ISceneManager.h"

namespace mray{
namespace scene{

class LightNode;

class ShadowRenderPass:public IRenderPass
{
private:
protected:
	LightNode* m_light;
	CameraNode* m_camera;

	GCPtr<video::GPUShader> m_defaultDepthShader;

	GCPtr<video::RenderPass> m_material;
public:
	ShadowRenderPass(LightNode*light);
	virtual~ShadowRenderPass();

	virtual bool isPrepareationPass(){return true;}

	//virtual bool canRenderType(NODE_RENDER_TYPE t);

	virtual void beginPass(ISceneManager*smgr,ViewPort*vp);
	virtual void endPass();

	virtual void setActiveCamera(CameraNode* cam);
	virtual IViewNode* getViewNode();

	virtual bool useMaterial(video::RenderPass*m);
	virtual bool canRenderObject(IRenderable*r);
	virtual video::GPUShader* getMaterialShader(video::RenderPass*m);
};

}
}

#endif //___ShadowRenderPass___
