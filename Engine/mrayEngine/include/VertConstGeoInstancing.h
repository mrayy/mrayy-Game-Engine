
/********************************************************************
	created:	2009/07/25
	created:	25:7:2009   19:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\VertConstGeoInstancing.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	VertConstGeoInstancing
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Vertex Constance Geometry Instancing,
				here we send an extra stream to vertex shader that contains
				instance id for each vertex , and use a global matrix array 
				contains world matrix for each instance
*********************************************************************/

#ifndef ___VertConstGeoInstancing___
#define ___VertConstGeoInstancing___

#include "IGeometryBatch.h"




namespace mray{
	namespace video{
		class RenderMaterial;
	}
namespace scene{
	class MeshRenderableNode;
	class IMeshBuffer;

class MRAY_DLL VertConstGeoInstancing:public IGeometryBatch
{
private:
protected:

	
	struct GeometryInstance
	{
		IMeshBuffer* buffer;
		math::matrix4x4 worldMat;
		int id;
	};
	struct GeometryCollection
	{
		GeometryCollection()
		{
			m_vCnt=0;
			m_iCnt=0;
			mtrl=0;
			buff=0;
		}
		uint m_vCnt;
		uint m_iCnt;

		IMeshBuffer*buff;
		video::RenderMaterial* mtrl;
		std::list<GeometryInstance> instances;
	};

	typedef std::map<video::RenderMaterial*,GeometryCollection> DynamicInstanceList;
	DynamicInstanceList m_internalInstances;

	uint g_max16bit;
	uint g_max32bit;

	int m_lastIDAssigned;

	scene::SMesh* m_mesh;

	int m_streamIndex;
	int m_objects;

	bool m_limitTo16Bit;

	//std::vector<math::matrix4x4> m_matricies;

public:
	VertConstGeoInstancing(int streamIndex=2,bool limitTo16Bit=false);
	virtual~VertConstGeoInstancing();

	virtual EGeomBatchType getType(){return EGBT_VertexConstants;}

	scene::SMesh* getMesh(){
		return m_mesh;
	}

	std::vector<math::matrix4x4> &getMatricies();

	virtual bool addNode(MeshRenderableNode* buff);
	virtual bool addNode(SMesh* meshBuffer,IMovable* node);

	virtual uint build();

	virtual void update();
};

}
}

#endif //___VertConstGeoInstancing___
