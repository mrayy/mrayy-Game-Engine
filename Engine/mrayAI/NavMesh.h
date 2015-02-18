

/********************************************************************
	created:	2010/02/17
	created:	17:2:2010   8:38
	file base:	NavMesh
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	generate Navigation Mesh and optimise it,
				used for operations like path finding
*********************************************************************/

#ifndef ___NavMesh___
#define ___NavMesh___

#include "compileConfig.h"
#include "GCPtr.h"
#include "NavMeshPolygon.h"

#include "NavMeshGrid.h"
#include "IWorldVisualizer.h"
#include "INavWorld.h"
#include "matrix4x4.h"

#include <map>

namespace mray{
	namespace OS{
		class IStream;
	}
namespace AI{

	class AISystem;

class MRAY_AI_DLL NavMesh:public INavWorld
{
private:
protected:

	struct NavMeshHeader
	{
		uchar magic;//magic id
		math::box3d bbox;
		float slope;
		float maxStepHeight;
		int numberOfVertices;
		int numberOfPolygons;
		int gridSize;
		int cellsCount;
	};
	struct PolyDesc
	{
		int id;
		math::box3d bbox;
		math::vector3d center;
		math::Plane plane;
		int indexCount;
		int adjCount;
		int attribute;
	};
	struct AdjDesc
	{
		int edgeId;
		int pId;
		math::vector3d edgeMidpoint;
	};
	struct GridCellDesc
	{
		int polyCount;
	};

	struct AttributeDesc
	{
		AttributeDesc()
		{
			defaultWeight=1;
		}
		core::string name;
		float defaultWeight;
	};

	AISystem* m_system;

	std::vector<WorldWall> m_walls;

	std::map<int,AttributeDesc> m_attributeTranslateMap;
	std::map<core::string,int> m_attributeNameIDMap;
	core::string m_AttrDescFileName;

	std::vector<math::vector3d> m_verticies;
	NavWorldNodeListPtr m_polygons;
	std::vector<NavWorldNodeListPtr> m_vertPoly;

	GraphNodesList m_nodes;
	GraphLinksList m_links;
	std::vector<IGraphNode*> m_nodesArray;

	IWorldVisualizer* m_visualizer;

	math::box3d m_boundingBox;
	float m_slope;
	float m_maxStepHeight;
	//2D Grid,used for fast queries
	NavMeshGrid m_grid;

	core::string m_navMeshFile;
	bool m_isVisible;

	void _onAddPolygon(NavMeshPolygon*p);
	void _onRemovePolygon(NavMeshPolygon*p);

	NavWorldNodeListPtr::iterator _findPoly(NavMeshPolygon*p);

	void _copyData(const math::vector3d*vert,int vCount,const void*ind,int indCount,int *attrs,bool is16bitIdx,const math::matrix4x4*transMat);
	void _optimize();
	bool isConvex(const NavMeshPolygon*p);
	bool getAdjEdge(NavMeshPolygon*p1,NavMeshPolygon*p2,uint &oi1,uint &oi2);

	bool getAdjEdge(NavMeshPolygon*p1,NavMeshPolygon*p2,
		int s,int cnt,uint &oi1,uint &oi2);//look up for adj poly between s and e from p1

	//e is the shared edges for p1
	bool _MergePolygons(NavMeshPolygon*p1,NavMeshPolygon*p2,int e,NavMeshPolygon*&outPoly);

	//generate the connection graph
	//creates the nodes connection and the edges
	void GenerateGraph();

	//Store walls in a list for further use
	void GenerateWalls();
public:
	NavMesh(AISystem* system);
	virtual~NavMesh();

	void SetGridCellSize(float sz);

	void SetMaxSlope(float slope);
	float GetMaxSlope();

	void SetMaxStepSize(float step);
	float GetMaxStepSize();

	//attrs: polys attributes
	void Generate(const math::vector3d*vert,int vCount,const void*ind,int indCount,int *attrs,bool is16bitIdx,const math::matrix4x4*transMat=0);
	virtual void Clear();

	const math::vector3d* GetVerticies();

	void LoadAttributeDescFile(const core::string&name);
	core::string GetAttributeName(int id);
	int GetAttributeID(const core::string& attr);
	float GetAttributeDefaultWeight(int id);


	const NavWorldNodeListPtr& GetWorldNodes();
	const GraphNodesList& GetGraphNodes();
	const GraphLinksList& GetGraphLinks();

	IGraphNode* GetGraphNode(int id);

	void OptimizePath(const math::vector3d&start,INavWorldNode*startPoly,
		const math::vector3d&end,INavWorldNode*endPoly,
		GraphLinksList&path,PathEdgeList&list);

	//test if 2 points can see each other directly
	bool lineOfSightTest(const math::vector3d&start,INavWorldNode*startPoly,const math::vector3d&target,
		INavWorldNode*targetPoly,math::vector3d*intersection);

	// get polygons which contains p in their XZ plane,and return the nearest poly to 
	// that point from those polys
	INavWorldNode* QueryPolys(const math::vector3d&p,NavWorldNodeListPtr&outPoly);

	//bounding box of the mesh
	const math::box3d& GetBoundingBox();

	virtual const std::vector<WorldWall>& GetWorldWalls();

	void Deserialize(const core::string& file);
	void Serialize(OS::IStream* stream);

	virtual IWorldVisualizer* GetVisualizer();
	virtual IGraphSearch* CreatePathFinder();
	virtual ICostFunction* CreateHeuristicFunction();

	virtual void EnableVisualize(bool e){m_isVisible=e;}
	virtual bool IsEnableVisualize(){return m_isVisible;}
	virtual void Visualize(IRenderDevice*dev);

	virtual void loadXMLSettings(xml::XMLElement* e);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

};

}
}


#endif //___NavMesh___
