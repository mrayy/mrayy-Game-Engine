

/********************************************************************
	created:	2010/08/18
	created:	18:8:2010   21:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\WaypointPath.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	WaypointPath
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef WaypointPath_h__
#define WaypointPath_h__

#include "Waypoint.h"
#include "IVisualizable.h"
#include <vector>
#include <map>

namespace mray
{/*
	namespace scene
	{
		class WaypointSceneNode;
	}*/
	namespace xml
	{
		class XMLElement;
	}
namespace AI
{

class MRAY_AI_DLL WaypointPath:public IVisualizable
{
private:
protected:
	typedef std::vector<Waypoint> WaypointList;
	WaypointList m_waypoints;
/*
	typedef std::map<int,scene::WaypointSceneNode*> WaypointSceneNodes;
	WaypointSceneNodes m_wpSceneNodes;
*/
	core::string m_name;
	int m_lastID;

	bool m_visualizable;
	bool m_createWPSceneNodes;
public:
	WaypointPath(bool createWPSceneNodes);
	virtual ~WaypointPath();

	const core::string& GetName()const;
	void SetName(const core::string&name);


	void AddWaypoint(const Waypoint&p);
	void RemoveWaypoint(int pos);
	void RemoveWaypointById(int id);

	void Clear();

	int GetWaypointsCount();

	Waypoint* GetWaypoint(int i);
	Waypoint* GetWaypointById(int id);



	void loadXMLSettings(xml::XMLElement* e);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

	virtual void EnableVisualize(bool e);
	virtual bool IsEnableVisualize();
	virtual void Visualize(IRenderDevice*dev);
};

}
}

#endif // WaypointPath_h__
