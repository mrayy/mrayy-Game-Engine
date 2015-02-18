


/********************************************************************
	created:	2010/09/19
	created:	19:9:2010   21:59
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ReflectionRenderPass.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ReflectionRenderPass
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ReflectionRenderPass_h__
#define ReflectionRenderPass_h__

#include "IRenderPass.h"
#include "IRenderTarget.h"
#include "ISceneManager.h"

namespace mray
{
namespace scene
{

class MRAY_DLL ReflectionRenderPass:public IRenderPass
{
private:
protected:
	CameraNode* m_camera;

	GCPtr<video::IRenderTarget> m_reflectionRT;

	math::Plane m_reflectionPlane;
public:
	ReflectionRenderPass(CameraNode*camera);
	virtual ~ReflectionRenderPass();

	virtual bool isPrepareationPass(){return true;}

	void SetRenderTarget(GCPtr<video::IRenderTarget> rt);
	video::IRenderTarget* GetRenderTarget();

	void SetReflectionPlane(const math::Plane&p);
	const math::Plane& GetReflectionPlane();

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
#endif // ReflectionRenderPass_h__
