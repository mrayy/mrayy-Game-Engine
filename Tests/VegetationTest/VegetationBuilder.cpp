 
#include "stdafx.h"

#include "VegetationBuilder.h"
#include "IViewNode.h"


namespace mray
{
namespace scene
{

VegetationBuilder::VegetationBuilder()
{
	m_LODdistance=10;
	m_isDirty=false;
	m_billboardsObjects=new StaticGeometryBatch();
	m_dynamicObjects=new VertConstGeoInstancing();
}
VegetationBuilder::~VegetationBuilder()
{
}

void VegetationBuilder::SetBillboard(GCPtr<scene::SMesh> mesh)
{
	m_billboardMesh=mesh;
}
void VegetationBuilder::SetBrush(GCPtr<scene::SMesh> mesh)
{
	m_instanceMesh=mesh;
}
void VegetationBuilder::AddObject(GCPtr<scene::IMovable> node)
{
	InstanceData data;
	data.node=node;
	m_instancesObjects.push_back(data);
	m_isDirty=true;
}

void VegetationBuilder::Update(IViewNode*viewNode)
{
	bool needUpdataInstances=m_isDirty;
	float distSqrt=m_LODdistance*m_LODdistance;
	std::list<InstanceData>::iterator it=m_instancesObjects.begin();
	for(;it!=m_instancesObjects.end();++it)
	{
		IMovable*node = it->node;
		if(viewNode->getViewPos().getDistSQ(node->getAbsolutePosition())>distSqrt)
		{
			if(!it->isBillboard)
			{
			//	needUpdataInstances=true;
				it->isBillboard=true;
			}
		}else
		{
			if(it->isBillboard)
			{
			//	needUpdataInstances=true;
				it->isBillboard=false;
			}
		}
	}
	if(needUpdataInstances)
	{
		m_isDirty=false;
		m_billboardsObjects->clearAll();
		m_dynamicObjects->clearAll();
		it=m_instancesObjects.begin();
		for(;it!=m_instancesObjects.end();++it)
		{
			IMovable*node = it->node;
			if(it->isBillboard)
			{
				m_billboardsObjects->addNode(m_billboardMesh,node);
			}else
				m_dynamicObjects->addNode(m_instanceMesh,node);
		}
		m_billboardsObjects->build();
		m_dynamicObjects->build();
	}
	m_billboardsObjects->update();
	m_dynamicObjects->update();
}

}
}


