
/********************************************************************
	created:	2009/07/24
	created:	24:7:2009   8:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\DynamicGeometryBatch.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	DynamicGeometryBatch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DynamicGeometryBatch___
#define ___DynamicGeometryBatch___

#include "IGeometryBatch.h"

#include "SMesh.h"

namespace mray{
namespace scene{

	class MeshRenderableNode;

class MRAY_DLL DynamicGeometryBatch:public IGeometryBatch
{
private:
protected:
	uint g_max16bit;
	uint g_max32bit;

	std::list<math::matrix4x4> m_lastTrans;

	GCPtr<SMesh> m_mesh;
	uint m_vCnt;
	uint m_iCnt;


	void buildMesh(IMeshBuffer* b);
public:
	DynamicGeometryBatch();
	virtual~DynamicGeometryBatch();

	virtual EGeomBatchType getType(){return EGBT_Dynamic;}

	GCPtr<SMesh> getMesh(){
		return m_mesh;
	}

	virtual bool addNode(MeshRenderableNode* buff);
	virtual bool addNode(SMesh* meshBuffer,IMovable* node);

	virtual uint build();

	virtual void update();
};

}
}


#endif //___DynamicGeometryBatch___
