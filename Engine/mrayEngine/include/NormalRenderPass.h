
/********************************************************************
	created:	2010/02/23
	created:	23:2:2010   14:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\NormalRenderPass.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	NormalRenderPass
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NormalRenderPass___
#define ___NormalRenderPass___

#include "IRenderPass.h"
#include "CompileConfig.h"

namespace mray{
namespace scene{
	class ViewPort;

class MRAY_DLL NormalRenderPass:public IRenderPass
{
private:
protected:
	CameraNode* m_camera;
	ViewPort*m_viewport;
public:
	NormalRenderPass();
	virtual~NormalRenderPass();

	virtual bool isPrepareationPass(){return false;}

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


#endif //___NormalRenderPass___
