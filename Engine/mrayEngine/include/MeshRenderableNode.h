
/********************************************************************
	created:	2008/11/02
	created:	2:11:2008   0:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\MeshRenderableNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	MeshRenderableNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MeshRenderableNode___
#define ___MeshRenderableNode___

#include "IRenderable.h"
#include "SMesh.h"

namespace mray{
namespace scene{

class MRAY_DLL MeshRenderableNode:public IRenderable
{
	DECLARE_RTTI
protected:
	SMeshPtr m_mesh;

	bool m_hasDrown;
	bool m_customRender;

	bool m_drawInCustomLayer;
	uint m_customLayer;
public:
	MeshRenderableNode(const SMeshPtr& mesh);
	virtual~MeshRenderableNode();

	virtual bool isCustomRender(){return m_customRender;}

	bool isDrawInCustomLayer();
	void setDrawInCustomLayer(bool s);

	void setCustomLayer(uint l);
	uint getCustomLayer();

	virtual bool preRender(IRenderPass*pass);
	virtual bool render(IRenderPass*pass);
	virtual void update(float dt);
	virtual void AddUpdateJobs(ThreadJobManager* mngr,float dt);

	virtual void setMaterial(const video::RenderMaterialPtr& mtrl,int i);
	virtual const video::RenderMaterialPtr& getMaterial(int i);
	virtual int getMaterialCount();


	virtual void setShader(int mesh,const video::GPUShaderPtr& shader);
	virtual void setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback);
	virtual const video::GPUShaderPtr& getShader(int i);

	virtual void setMesh(const SMeshPtr& mesh);
	virtual SMesh* getMesh();
	virtual math::box3d getBoundingBox();
};

}
}


#endif //___MeshRenderableNode___

