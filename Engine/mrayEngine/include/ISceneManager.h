
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   15:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISceneManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISceneManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISceneManager___
#define ___ISceneManager___


#include "IRenderable.h"
#include "IVideoDevice.h"
#include "ISceneNode.h"
#include "IRenderPass.h"

namespace mray{
namespace scene{
	class RenderManager;
	class CameraNode;
	class IDebugDrawManager;
	class IRenderPassSubscriber;

class MRAY_DLL ISceneManager
{
protected:
	video::IVideoDevice*m_device;
public:
	typedef std::map<core::string,ISceneNodePtr> SceneNodeMap;

	ISceneManager(video::IVideoDevice*dev);
	virtual~ISceneManager();

	video::IVideoDevice*getDevice();
	void setDevice(video::IVideoDevice*dev);

	virtual void setRootNode(scene::ISceneNode* root) = 0;
	virtual scene::ISceneNode* getRootNode() = 0;

	virtual void SetDebugDrawManager(IDebugDrawManager* mngr)=0;
	virtual IDebugDrawManager* GetDebugDrawManager()=0;

	virtual scene::RenderManager* getRenderManager()=0;

	virtual void registerBenchmark()=0;
	virtual void unregisterBenchmark()=0;

	virtual void addLightToRender(LightNode*l)=0;
	virtual void addNodeToRender(ISceneNode*node)=0;
	virtual void clearRenderList()=0;

	virtual void render(ViewPort*vp)=0;
	virtual void doRender()=0;

	virtual void update(float dt)=0;

	virtual void addPassSubscriber(IRenderPassSubscriber*rps)=0;
	virtual void removePassSubscriber(IRenderPassSubscriber*rps)=0;

	virtual LightNode* createLightNode(const core::string&name = mT(""), ISceneNode* parent = 0) = 0;
	virtual ISceneNode* createSceneNode(const core::string&name = mT(""), ISceneNode* parent = 0) = 0;
	virtual CameraNode* createCamera(const core::string&name = mT(""), ISceneNode* parent = 0) = 0;

	/*!
		Add a Node to scene manager
		if parent is null, the node would be a child of the scene manager root node
	*/
	virtual void addSceneNode(const ISceneNodePtr& node,ISceneNode* parent=0)=0;
	virtual void removeSceneNode(ISceneNode* node)=0;
	virtual ISceneNode* getNodeByID(uint id)=0;
	virtual void clearSceneNodes()=0;
	virtual const SceneNodeMap& GetSceneNodes()= 0;

	virtual CameraNode* getActiveCamera()=0;
	virtual void setActiveCamera(CameraNode* camera)=0;


	virtual bool isNodeCulled(ICullable*node)=0;

	virtual void setActiveViewnode(IViewNode*node)=0;
	virtual IViewNode*getActiveViewnode()=0;

	virtual void addRenderPass(const IRenderPassPtr& pass)=0;
	virtual void removeRenderPass(IRenderPass* pass)=0;
	virtual bool haveRenderPass(IRenderPass* pass)=0;
	virtual IRenderPass*getCurrnetRenderPass()=0;


	virtual void OnVisit(ISceneVisitor*visitor);
};

}
}

#endif //___ISceneManager___

