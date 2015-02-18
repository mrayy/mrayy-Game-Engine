

/********************************************************************
	created:	2009/06/13
	created:	13:6:2009   12:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGeometryBatch.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGeometryBatch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGeometryBatch___
#define ___IGeometryBatch___

#include "compileConfig.h"
#include "mtypes.h"


namespace mray{
namespace scene{


	enum EGeomBatchType
	{
		// the fastest way to instance geometry,but it's no a flexible way.
		// it's good for the static geometry like :buildings
		EGBT_Static,
		// the slowest way to instance geometry,but it has the most flexibility
		// good for the animated mesh
		EGBT_Dynamic,
		// a hybrid way , where the geometry is in the GPU , and the attributes are streamed every frame
		EGBT_VertexConstants,

		EGBT_HardwareInstancing
	};

	class IMovable;
	class SMesh;

class MRAY_DLL IGeometryBatch
{
public:
	struct InstanceData
	{
		SMesh* meshBuffer;
		IMovable* node;
	};
	typedef std::list<InstanceData> InstanceList;
private:
protected:
	/*typedef std::list<GCPtr<MeshRenderableNode>> InstanceList;
	InstanceList m_instances;
	GCPtr<video::RenderMaterial> m_material;*/


	InstanceList m_instances;

public:
	IGeometryBatch();
	virtual~IGeometryBatch();

	int getInstancesCount(){return m_instances.size();}
	const InstanceList& getInstances()const{return m_instances;}

	virtual EGeomBatchType getType()=0;

	virtual bool addNode(SMesh* meshBuffer,IMovable* node);
	virtual void clearAll();

	virtual uint build()=0;

	virtual void update()=0;
};

}
}


#endif //___IGeometryBatch___
