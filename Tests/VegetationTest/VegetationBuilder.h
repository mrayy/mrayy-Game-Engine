


/********************************************************************
	created:	2010/07/27
	created:	27:7:2010   23:51
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\VegetationTest\VegetationBuilder.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\VegetationTest
	file base:	VegetationBuilder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef VegetationBuilder_h__
#define VegetationBuilder_h__

#include "DynamicGeometryBatch.h"
#include "VertConstGeoInstancing.h"
#include "StaticGeometryBatch.h"
#include "IMovable.h"

#include <vector>

namespace mray
{
namespace scene
{

	class IViewNode;

class VegetationBuilder
{
private:
protected:

	struct InstanceData
	{
		InstanceData()
		{
			isBillboard=true;
		}
		GCPtr<scene::IMovable> node;
		bool isBillboard;
	};

	GCPtr<StaticGeometryBatch> m_billboardsObjects;
	GCPtr<VertConstGeoInstancing> m_dynamicObjects;

	GCPtr<scene::SMesh> m_billboardMesh;
	GCPtr<scene::SMesh> m_instanceMesh;
	std::list<InstanceData> m_instancesObjects;

	float m_LODdistance;

	bool m_isDirty;

public:
	VegetationBuilder();
	virtual ~VegetationBuilder();

	GCPtr<scene::SMesh> GetBillboardObjects(){return  m_billboardsObjects->getMesh();}
	GCPtr<scene::SMesh> GetDynamicObjects(){return  m_dynamicObjects->getMesh();}

	void SetLODDistance(float d){m_LODdistance=d;}

	void SetBillboard(GCPtr<scene::SMesh> mesh);
	void SetBrush(GCPtr<scene::SMesh> mesh);
	void AddObject(GCPtr<scene::IMovable> node);

	void Update(IViewNode*viewNode);
};

}
}


#endif // VegetationBuilder_h__


