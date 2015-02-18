
/********************************************************************
	created:	2010/02/23
	created:	23:2:2010   11:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IRenderPass.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IRenderPass
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRenderPass___
#define ___IRenderPass___

#include "GCPtr.h"

namespace mray{
namespace video{
	class RenderPass;
	class GPUShader;
}

namespace scene{
	class IRenderable;
	class CameraNode;
	class IViewNode;
	class ISceneManager;
	class ViewPort;


	MakeSharedPtrType(IRenderPass);
class IRenderPass
{
private:
protected:
	bool m_enabled;
public:
	IRenderPass():m_enabled(true){}
	virtual~IRenderPass(){}

	virtual void beginPass(ISceneManager*smgr,ViewPort*vp)=0;
	virtual void endPass()=0;

	bool IsEnabled()const{return m_enabled;}
	virtual void SetEnabled(bool e){m_enabled=e;}

	//virtual bool canRenderType(NODE_RENDER_TYPE t)=0;

	//if it's a prepass on the scene,then this should be true,
	//else (Normal pass) this should be false
	virtual bool isPrepareationPass()=0;

	virtual void setActiveCamera(CameraNode* cam)=0;
	virtual IViewNode* getViewNode()=0;

	virtual bool useMaterial(video::RenderPass*m)=0;
	virtual bool canRenderObject(IRenderable*r)=0;
	virtual video::GPUShader* getMaterialShader(video::RenderPass*m)=0;

};


}
}


#endif //___IRenderPass___
