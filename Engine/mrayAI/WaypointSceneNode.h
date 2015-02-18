

/********************************************************************
	created:	2010/08/18
	created:	18:8:2010   22:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\WaypointSceneNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	WaypointSceneNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef WaypointSceneNode_h__
#define WaypointSceneNode_h__

#include "compileConfig.h"
#include "Waypoint.h"
#include "EditorSceneNode.h"

namespace mray
{
namespace scene
{

class MRAY_AI_DLL WaypointSceneNode:public EditorSceneNode
{
private:
protected:
	AI::Waypoint *m_point;
public:
	WaypointSceneNode(AI::Waypoint* point);
	virtual ~WaypointSceneNode();

	AI::Waypoint* GetWaypoint();
	void SetWaypoint(AI::Waypoint*wp);


	virtual math::vector3d getPosition()const;
	virtual math::quaternion getOrintation()const;

	virtual void setPosition(const math::vector3d& v);
	virtual void setOrintation(const math::quaternion& q);
	
};

}
}
#endif // WaypointSceneNode_h__
