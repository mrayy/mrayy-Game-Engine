


/********************************************************************
	created:	2010/11/05
	created:	5:11:2010   20:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIBatchRenderer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIBatchRenderer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIBatchRenderer_h__
#define GUIBatchRenderer_h__

#include "IGUIRenderer.h"
#include "Point4d.h"
#include "GCPtr.h"


//#include "IGUIRenderOperation.h"

namespace mray
{
namespace video
{
	class IVideoDevice;
	class TextureUnit;
	class IHardwareStreamBuffer;
	class IndexData;
}
namespace scene
{
	class IMeshBuffer;
}
namespace GUI
{


class MRAY_DLL GUIBatchRenderer:public IGUIRenderer
{
private:
protected:

	ushort m_maxBatchSize;

	scene::IMeshBuffer* m_mesh;
	video::IndexData* m_indicies;

	video::IVideoDevice* m_device;
	/*
	typedef std::list<IGUIRenderOperationPtr> OperationList;
	typedef std::map<video::TextureUnit*,OperationList> OperationMap;
	
	OperationMap m_operations;*/
	video::IHardwareStreamBuffer* m_posStream;
	video::IHardwareStreamBuffer* m_tcStream;
	video::IHardwareStreamBuffer* m_clrStream;

	math::vector2d* m_posPtr;
	math::vector2d* m_tcPtr;
	video::SColor* m_clrPtr;

	video::TextureUnit* m_lastTexture;

	bool m_locked;

	bool m_batchRender;

	void _lockBuffers();
	void _unlockBuffers();
	//void _RenderGroup(const OperationList&grp);
	//bool _ProcessSingleOperation(IGUIRenderOperation*op);

public:
	GUIBatchRenderer();
	virtual ~GUIBatchRenderer();

	void SetDevice(video::IVideoDevice* device){m_device=device;}
	video::IVideoDevice* GetDevice(){return m_device;}
	void SetRenderMode(bool batched);
	
	//void AddRenderOperation(const IGUIRenderOperationPtr& op,bool forceImediate=false);
	
	virtual void Prepare();

	virtual void AddQuad(video::TextureUnit*tex, const math::rectf&pos,const math::rectf&tc,const video::SColor*clr,const math::rectf* clip=0,float angle=0);
	virtual void AddQuad(video::TextureUnit*tex, const math::rectf&pos,const math::rectf&tc,const video::SColor&clr,const math::rectf* clip=0,float angle=0);

	virtual void Flush();
	//void Clear();
};

MakeSharedPtrType(GUIBatchRenderer);

}
}


#endif // GUIBatchRenderer_h__
