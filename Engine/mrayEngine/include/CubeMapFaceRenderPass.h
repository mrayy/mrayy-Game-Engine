
/********************************************************************
	created:	2012/09/02
	created:	2:9:2012   23:22
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\CubeMapFaceRenderPass.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	CubeMapFaceRenderPass
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CubeMapFaceRenderPass___
#define ___CubeMapFaceRenderPass___



namespace mray
{
namespace scene
{

class CubeMapFaceRenderPass:public IRenderPass
{
protected:

	GCPtr<video::IRenderTarget> m_faceSurface;
public:
	CubeMapFaceRenderPass();
	virtual~CubeMapFaceRenderPass();

	virtual bool isPrepareationPass(){return true;}

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

#endif

