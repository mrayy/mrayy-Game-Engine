

/********************************************************************
	created:	2011/02/03
	created:	3:2:2011   14:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\DebugDrawManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	DebugDrawManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef DebugDrawManager_h__
#define DebugDrawManager_h__

#include "IDebugDrawManager.h"
#include "JobOrder.h"
#include "RenderPass.h"


namespace mray
{
	class ThreadJobManager;
	class IJobPiece;

namespace video
{
	class IVideoDevice;
	class IHardwareStreamBuffer;
	class IHardwareStreamBuffer;
	class IndexData;
}
namespace OS
{
	class ThreadBarrier;
	class IMutex;
}

namespace scene
{
	class SMesh;
	class IMeshBuffer;
	class IViewNode;

class MRAY_DLL DebugDrawManager:public IDebugDrawManager
{
private:
protected:

	enum EPrimitiveType
	{
		//list standard primitives
		EPT_Sphere,

		EPT_Count,

		//non standard
		EPT_Billboard,
		EPT_String,
		EPT_Custom
	};

	enum EBufferType
	{
		EBT_WireFrame,
		EBT_Solid,
		EBT_Textured,
		EBT_Count
	};

	struct PrimitiveObject
	{
		PrimitiveObject() :texture(0), duration(0), thickness(1)
		{
		}
		video::ITexture* texture;
		EPrimitiveType type;
		video::SColor color;
		float duration;
		float thickness;
		math::vector3d scale;	//used to scale a standard primitive
		math::vector3d pos;		//used to position a standard primitive
		std::vector<math::vector3d> vertices;
		std::vector<ushort> indices;
		std::vector<math::vector2d> uv;
	};

	typedef std::list<PrimitiveObject*> PrimitiveList;

	struct TexturedObject
	{
		video::ITexture* tex;
		uint startIndex;
		uint count;
	};

	typedef std::vector<TexturedObject> TexturedObjects;
	typedef std::map<video::ITexture*,PrimitiveList> TexturedPrimitives;

	struct BufferData
	{
		IMeshBuffer* buffer;

		video::IHardwareStreamBuffer* positionBuffer;
		video::IHardwareStreamBuffer* colorBuffer;
		video::IHardwareStreamBuffer* texBuffer;
		video::IndexData* indexData;

		int vertixCount;
		int indexCount;

		TexturedPrimitives primitives;

		TexturedObjects texObjects;
	};

	BufferData m_buffers[EBT_Count];

	PrimitiveObject m_standardPrimitives[EPT_Count];

	PrimitiveList m_deadPrimitives;

	video::IVideoDevice* m_device;
	SMesh* m_mesh;

	ThreadJobManager* m_threadManager;
	/*JobOrderPtr*/IJobPiece* m_buildPrimitivesJob;
	JobLoadCompleteDelegate m_buildPrimitivesCompleted;
	//OS::ThreadBarrier* m_buildPrimitivesJobBarrier;

	//OS::IMutex* m_primitivesMutex;

	video::RenderPass m_material;

	IViewNode* m_camera;

protected:
	void OnBuildPrimitivesCompleted(JobOrder*o,bool s);

	void AddPrimitiveToDraw(PrimitiveObject* p,EBufferType buffer);
public:
	DebugDrawManager(video::IVideoDevice* device);
	virtual ~DebugDrawManager();

	virtual void AddLine(const math::vector3d& start,const math::vector3d& end,const video::SColor& clr,float lineWidth=1,float duration=0,bool depthTest=true );
	virtual void AddArrow(const math::vector3d& start,const math::vector3d& end,const video::SColor& clr,float lineWidth=1,float duration=0,bool depthTest=true );
	virtual void AddCross(const math::vector3d& pos,float size,const video::SColor& clr,float duration=0,bool depthTest=true );
	virtual void AddSphere(const math::vector3d& pos,float radius,const video::SColor& clr,bool wireframe=true,float duration=0,bool depthTest=true );
	virtual void AddCircle(const math::vector3d& pos,float radius,const math::vector3d& dir,const video::SColor& clr,float duration=0,bool depthTest=true );
	virtual void AddAxis(const math::matrix4x4& transform,float size,float duration=0,bool depthTest=true );
	virtual void AddMesh(const math::vector3d* verts,const ushort* indicies,int vCnt,int iCnt,const video::SColor& clr,bool wireframe=true,float duration=0,bool depthTest=true );
	virtual void AddAABB(const math::box3d& aabb,const video::SColor& clr,float duration=0,bool depthTest=true );
	virtual void AddOBB(const math::matrix4x4& transform,const math::vector3d& scale,const video::SColor& clr,float duration=0,bool depthTest=true );
	virtual void AddString(const math::vector3d& pos,const core::string& txt,const video::SColor& clr,float duration=0,bool depthTest=true );

	virtual void AddBillboard(const math::vector3d& pos,const math::vector2d& size,video::ITexture* tex,const video::SColor& clr,float zrot=0,float duration=0,bool depthTest=true );

	void Clear();

	virtual void StartDraw(IViewNode* cam);
	virtual void EndDraw();

	virtual void Update(float dt);


	//////////////////////////////////////////////////////////////////////////
	PrimitiveObject* CreatePrimitive();
	void RemovePrimitive(PrimitiveObject* p);

	void _BuildDrawingLists();
};

}
}

#endif // DebugDrawManager_h__
