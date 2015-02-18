
/********************************************************************
	created:	2008/12/23
	created:	23:12:2008   20:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RenderManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RenderManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RenderManager___
#define ___RenderManager___

#include "CompileConfig.h"
#include "RenderOperation.h"
#include "IJobPiece.h"


namespace mray{
	namespace OS
	{
		class ThreadBarrier;
	}
	class ThreadJobManager;

namespace scene{
	class ISceneManager;
	class IRenderGroup;
	class IRenderPass;


	enum ERenderGroupsHeaders{
		RGH_Skies=0,
		RGH_Solid=10,
		RGH_Transparent=20
	};

class MRAY_DLL RenderManager
{
protected:
	typedef std::map<int,IRenderGroup*> RenderGroupMap;
	RenderGroupMap m_renderGroups;

	ISceneManager* m_sceneManager;

	int m_currGroup;

	ThreadJobManager* m_threadManager;

	IJobPiece* m_sortNodesJob;
	OS::ThreadBarrier* m_sortNodesJobBarrier;
	//JobLoadCompleteDelegate m_sortNodesCompleted;

//	void OnSortNodesCompleted(JobOrder*const &o,const bool&s);
public:
	RenderManager(ISceneManager* sceneManager);
	virtual~RenderManager();

	ISceneManager* getSceneManager(){return m_sceneManager;}

	IRenderGroup *getRenderGroup(int idx);


	void addOperation(int group,RenderOperation &op);
	void startSorting();
	void endSorting();
	void renderAll(IRenderPass*pass);
	void clear();

	int getCurrGroup();

	//////////////////////////////////////////////////////////////////////////
	void _SortNodes();//called from a separate thread, not from the main thread!
};

}
}


#endif //___RenderManager___

