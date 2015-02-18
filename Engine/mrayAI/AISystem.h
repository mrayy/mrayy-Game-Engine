
/********************************************************************
	created:	2009/01/31
	created:	31:1:2009   20:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AISystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AISystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AISystem___
#define ___AISystem___

#include "compileConfig.h"
#include "AIActor.h"
#include "ISingleton.h"
#include "IDGenerator.h"
#include <GCPtr.h>

namespace mray{
namespace AI{

	struct AISystemDesc
	{
		AISystemDesc()
		{
			cellPartitionSize=20;
			worldBB.MinP=-100;
			worldBB.MaxP=+100;
		}
		math::box3d worldBB;

		int cellPartitionSize;
	};

	class PathFindingManager;
	class MessageDispatcher;
	class GridSpacePartition;
	class IRenderDevice;
	class INavWorld;

class MRAY_AI_DLL AISystem:public IVisualizable
{
protected:

	GridSpacePartition* m_spacePartition;

	IDGenerator<AIActor*> m_actorsID;

	typedef std::list<GCPtr<AIActor>> ActorsList;

	ActorsList m_actors;

	INavWorld* m_navWorld;

	bool m_enableVis;

	MessageDispatcher* m_messageDispatcher;
	PathFindingManager* m_pathFindingManager;
public:
	AISystem();
	virtual ~AISystem();

	void OnNavMeshLoaded();

	virtual void EnableVisualize(bool e);
	virtual bool IsEnableVisualize();

	void SetParameters(const AISystemDesc& desc);

	void AddActor(GCPtr<AIActor> actor);
	void RemoveActor(AIActor*actor);

	void RemoveAllActors();
	void Clear();

	void QueryActors(const math::box3d& box,AIActorList&actors);

	//void SetNavigationWorld(GCPtr<INavWorld> world);
	INavWorld* GetNavigationWorld();
	MessageDispatcher* GetMessageDispatcher();

	PathFindingManager* GetPathFindingManager();

	void Update(float dt);
	virtual void Visualize(IRenderDevice*dev);

	void loadXMLSettings(xml::XMLElement* e);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}

#endif //___AISystem___
