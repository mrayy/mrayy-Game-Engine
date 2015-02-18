

/********************************************************************
	created:	2010/03/08
	created:	8:3:2010   13:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\INavWorld.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	INavWorld
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	base abstract class for world navigation
*********************************************************************/

#ifndef ___INavWorld___
#define ___INavWorld___

#include <GCPtr.h>
#include <box3d.h>

//#include "IWorldVisualizer.h"
//#include "IGraphSearch.h"
#include "PathEdge.h"
//#include "IGraphLink.h"
#include "IGraphNode.h"
#include "IVisualizable.h"
#include "INavWorldNode.h"
//#include "XMLElement.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace xml{
	class XMLElement;
}
namespace AI{

	class IRenderDevice;
	class IWorldVisualizer;
	class IGraphSearch;
	//class IGraphNode;
	class ICostFunction;

class WorldWall
{
public:
	WorldWall():owner(0)
	{
	}
	INavWorldNode* owner;
	math::line3d wallLine;
};
class INavWorld:public IVisualizable
{
private:
protected:
public:

	INavWorld(){}
	virtual~INavWorld(){}

	virtual void LoadAttributeDescFile(const core::string&name)=0;
	virtual core::string GetAttributeName(int id)=0;
	virtual int GetAttributeID(const core::string& attr)=0;
	virtual float GetAttributeDefaultWeight(int id)=0;


	//test if 2 points can see each other directly
	virtual bool lineOfSightTest(const math::vector3d&start,INavWorldNode*startPoly,const math::vector3d&target,
		INavWorldNode*targetPoly,math::vector3d*intersection)=0;

	// get polygons which contains p in their XZ plane,and return the nearest poly to 
	// that point from those polys
	virtual INavWorldNode* QueryPolys(const math::vector3d&p,NavWorldNodeListPtr&outPoly)=0;
	
	//bounding box of the mesh
	virtual const math::box3d& GetBoundingBox()=0;

	virtual const NavWorldNodeListPtr& GetWorldNodes()=0;

	virtual const GraphNodesList& GetGraphNodes()=0;
	virtual const GraphLinksList& GetGraphLinks()=0;
	virtual IGraphNode* GetGraphNode(int id)=0;

	virtual void OptimizePath(const math::vector3d&start,INavWorldNode*startPoly,
		const math::vector3d&end,INavWorldNode*endPoly,
		GraphLinksList&path,PathEdgeList&list)=0;

	virtual IWorldVisualizer* GetVisualizer()=0;

	virtual void Clear()=0;

	virtual void Deserialize(const core::string& file)=0;
	virtual void Serialize(OS::IStream* stream)=0;

	virtual IGraphSearch* CreatePathFinder()=0;

	virtual ICostFunction* CreateHeuristicFunction()=0;

	virtual const std::vector<WorldWall>& GetWorldWalls()=0;

	virtual void Visualize(IRenderDevice*dev)=0;

	virtual void loadXMLSettings(xml::XMLElement* e)=0;
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem)=0;
};

}
}


#endif //___INavWorld___
