

/********************************************************************
	created:	2009/07/23
	created:	23:7:2009   16:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\StaticGeometryBatch.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	StaticGeometryBatch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___StaticGeometryBatch___
#define ___StaticGeometryBatch___

#include "IGeometryBatch.h"
#include "videoCommon.h"



namespace mray{
	namespace video{
		class IHardwareStreamBuffer;
		class RenderMaterial;
	}
namespace scene{

	class IMeshBuffer;
	class MeshRenderableNode;

class MRAY_DLL StaticGeometryBatch:public IGeometryBatch
{
public:
	struct MeshStreamInfo
	{
		MeshStreamInfo(video::EMeshStreamType s):stream(s),index(0),__usedStream(0)
		{}
		MeshStreamInfo(video::EMeshStreamType s,int i):stream(s),index(i),__usedStream(0)
		{}
		video::EMeshStreamType stream;
		int index;

		//temporal use only
		video::IHardwareStreamBuffer* __srcStream;
		video::IHardwareStreamBuffer* __usedStream;
		void* __dataPtr;
		void* __srcPtr;
		bool __onlyCopy;
		int __dataSize;
	};
private:
protected:
	struct GeometryInstance
	{
		IMeshBuffer* buffer;
		math::matrix4x4 worldMat;
	};
	struct GeometryCollection
	{
		GeometryCollection()
		{
			m_vCnt=0;
			m_iCnt=0;
			mtrl=0;
		}
		uint m_vCnt;
		uint m_iCnt;

		video::RenderMaterial* mtrl;
		std::list<GeometryInstance> instances;
	};

	typedef std::map<video::RenderMaterial*,GeometryCollection> StaticInstanceList;
	StaticInstanceList m_internalInstances;

	uint g_max16bit;
	uint g_max32bit;

	bool m_limitTo16Bit;
	scene::SMesh* m_mesh;

	std::list<MeshStreamInfo> m_streamsToCopy;
public:
	StaticGeometryBatch(bool limitTo16Bit=false);
	StaticGeometryBatch(const std::list<MeshStreamInfo>&streamsToCopy,bool limitTo16Bit=false);
	virtual~StaticGeometryBatch();

	virtual EGeomBatchType getType(){return EGBT_Static;}

	scene::SMesh* getMesh();

	virtual bool addNode(MeshRenderableNode* buff);
	virtual bool addNode(SMesh* meshBuffer,IMovable* node);
	virtual bool addNode(SMesh* meshBuffer,const math::matrix4x4& transform);
	virtual bool addNode(IMeshBuffer*  buff ,video::RenderMaterial*mtrl,const math::matrix4x4& transform);
	virtual void clearAll();

	virtual uint build();

	virtual void update();
};

}
}


#endif //___StaticGeometryBatch___
