

#ifndef ___SCENEMANAGER___
#define ___SCENEMANAGER___

#include "ISceneManager.h"

#include "ISceneNode.h"
#include "Key_Map.h"
#include "RenderManager.h"

#include "IDGenerator.h"
#include "LightNode.h"
#include "IDebugDrawManager.h"



namespace mray{

	class ThreadJobManager;
	class BenchmarkItem;

namespace OS{

	class IMutex;
}
namespace scene{


class MRAY_DLL SceneManager:public ISceneManager
{

protected:

	uint m_nameIDGenerator;

	SceneNodeMap m_Children;
	IDGenerator<ISceneNode*> m_objectsID;

	CameraNode* m_activeCamera;
	IViewNode * m_activeViewnode;

	scene::RenderManager* m_renderManager;

	std::list<ISceneNode*> m_nodesToRender;
	std::list<LightNode*> m_lightNodesToRender;
	std::list<IRenderPassSubscriber*> m_passSubscribers;

	typedef std::list<IRenderPass*> RenderPassList;
	IRenderPass* m_currentPass;


	OS::IMutex* m_nodesMutex;

	ISceneNode* m_rootNode;

	ThreadJobManager* m_jobsThreadsManager;

	BenchmarkItem* m_benchmarkItem;
	BenchmarkItem* m_renderBI;
	BenchmarkItem* m_updateBI;

	IDebugDrawManager* m_debugDrawManager;

	void _renderInternal(IRenderPass*pass);

	void _setupLightsToRender(IRenderPass*pass);

	void _collectPasses(RenderPassList& plist);

public:
	SceneManager(video::IVideoDevice*dev);
	virtual ~SceneManager();

	ThreadJobManager* GetJobsManager(){return m_jobsThreadsManager;}

	virtual void setRootNode(scene::ISceneNode* root) { m_rootNode = root; }
	virtual scene::ISceneNode* getRootNode(){ return m_rootNode; }
	virtual void registerBenchmark();
	virtual void unregisterBenchmark();

	virtual void addLightToRender(LightNode*l);
	virtual void addNodeToRender(ISceneNode*node);
	virtual void clearRenderList();

	scene::RenderManager *getRenderManager();

	virtual void render(ViewPort*vp);
	virtual void doRender();

	virtual void update(float dt);


	virtual void SetDebugDrawManager(IDebugDrawManager* mngr);
	virtual IDebugDrawManager* GetDebugDrawManager();

	virtual LightNode* createLightNode(const core::string&name = mT(""), ISceneNode* parent = 0);
	virtual ISceneNode* createSceneNode(const core::string&name = mT(""), ISceneNode* parent = 0);
	virtual CameraNode* createCamera(const core::string&name = mT(""), ISceneNode* parent = 0);

	/*!
	  Add a custom scene Node to scene manager
	  the node would be a child of the scene manager
	*/
	void addSceneNode(const ISceneNodePtr& node, ISceneNode* parent = 0);
	void removeSceneNode(ISceneNode*node);
	ISceneNode* getNodeByID(uint id);

	void clearSceneNodes();

	virtual void addPassSubscriber(IRenderPassSubscriber*rps);
	virtual void removePassSubscriber(IRenderPassSubscriber*rps);

	CameraNode* getActiveCamera();
	void setActiveCamera(CameraNode* camera);

	bool isNodeCulled(ICullable*node);

	void setActiveViewnode(IViewNode*node);
	IViewNode*getActiveViewnode();

	virtual void addRenderPass(const IRenderPassPtr& pass);
	virtual void removeRenderPass(IRenderPass* pass);
	virtual bool haveRenderPass(IRenderPass* pass);
	virtual IRenderPass*getCurrnetRenderPass();

	virtual void OnVisit(ISceneVisitor*visitor);

	const SceneNodeMap& GetSceneNodes(){return m_Children;}
};

}
}

#endif