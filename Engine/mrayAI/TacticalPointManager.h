


/********************************************************************
	created:	2010/05/23
	created:	23:5:2010   19:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\TacticalPointManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	TacticalPointManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	discover tactical points on the world,and allow to use queries to resolve
				queries can be : known what an agent can see,where are good hide positions,..,etc
*********************************************************************/

#ifndef TacticalPointManager_h__
#define TacticalPointManager_h__

#include "CompileConfig.h"
#include <BitVector.h>
#include <vector>
#include <map>
#include <marray.h>
#include <Point3d.h>
#include "IVisualizable.h"

namespace mray
{
namespace AI
{

	class INavWorld;
	class INavWorldNode;
	class WorldWall;
	class IRenderDevice;

	class TacticalPoint
	{
	public:
		TacticalPoint():id(0),owner(0),snipingCost(0),visiblePoints(0)
		{}

		int id;
		const WorldWall* owner;
		math::vector3d pos;

		int visiblePoints;
		float snipingCost;
	};
class MRAY_AI_DLL TacticalPointManager:public IVisualizable
{
private:
protected:
	typedef std::map<INavWorldNode*,int> NodeIDMap;
	typedef std::vector<std::vector<int>> NodeTacticalPointsIDMap;
	typedef std::vector<TacticalPoint> TacticalPointsList;
	NodeIDMap m_nodesID;
	TacticalPointsList m_tacticalPoints;
	NodeTacticalPointsIDMap m_nodePointsMap;
	INavWorld* m_world;
	std::vector<core::BitVector> m_visibility;	//matrix contains visibility information between nodes

	bool m_isVisible;

	void GenerateTacticalPoints();
	void GenerateVisibilityInfo();
public:
	TacticalPointManager(INavWorld*world);
	virtual ~TacticalPointManager();

	//get points which node can see
	void GetVisibleNodesVec(INavWorldNode*node,const math::vector3d&pos,core::BitVector&vec);

	//make the visibility vector wider with one level (query)
	core::BitVector ExtendVisibility(const core::BitVector&vec);

	//convert from bitvector into tactical points vector
	void GetNodesFromVector(const core::BitVector&vec,std::vector<TacticalPoint*>&points);

	virtual void EnableVisualize(bool e){m_isVisible=e;}
	virtual bool isEnableVisualize(){return m_isVisible;}
	virtual void Visualize(IRenderDevice* dev);
	
};

}
}

#endif // TacticalPointManager_h__
