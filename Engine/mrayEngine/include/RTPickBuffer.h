

/********************************************************************
	created:	2010/06/14
	created:	14:6:2010   11:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RTPickBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RTPickBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef RTPickBuffer_h__
#define RTPickBuffer_h__

#include "CompileConfig.h"
#include "ITexture.h"
#include "IPickBuffer.h"
#include "RenderPass.h"
#include "IRenderTarget.h"

#include <stack>


namespace mray
{
namespace video
{

	class IVideoDevice;

class MRAY_DLL RTPickBuffer:public IPickBuffer
{
private:
protected:

	struct IDRenderObject
	{
		IDRenderObject(uint id)
		{
			this->id=id;
		}
		uint id;

		struct ROperation
		{
			ROperation():node(0),bd(0)
			{}
			ROperation(scene::IRenderable*n,scene::MeshBufferData*b):node(n),bd(b)
			{}
			scene::IRenderable*node;
			scene::MeshBufferData*bd;
		};
		std::vector<ROperation> objs;
	};
	typedef std::stack<IDRenderObject> IDObjectsMap;

	IDObjectsMap m_objects;
	math::matrix4x4 m_viewMatrix;
	math::matrix4x4 m_projMatrix;

	ITexturePtr m_rtTex;
	IVideoDevice* m_device;
	IRenderTargetPtr m_rt;
	
	RenderPass m_mtrl;
	
	math::vector2di m_vpSize;
	math::vector2d m_vpRatio;

	LockedPixelBox m_lockedBox;

public:
	RTPickBuffer(IVideoDevice* device,const math::vector2di& rtSize,const math::vector2di& vpSize);
	virtual ~RTPickBuffer();

	virtual void Clear();
	void SetViewportSize(const math::vector2di& vpSize);
	void SetRenderTargetSize(const math::vector2di& rtSize);

	virtual void SetViewMatrix(const math::matrix4x4& m);
	virtual void SetProjectionMatrix(const math::matrix4x4& m);

	virtual void PushPickID(uint id);

	virtual void AddRenderObject(scene::MeshBufferData*bd,scene::IRenderable*obj);

	virtual void PopPickID();

	virtual void Render();

	const IRenderTargetPtr& GetRenderTarget(){return m_rt;}

	uint GetID(int x,int y);
};

}
}

#endif // RTPickBuffer_h__

