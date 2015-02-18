

#ifndef ___IRenderable___
#define ___IRenderable___

#include "IObject.h"


#include "GPUShader.h"
#include "GCPtr.h"
#include "RenderMaterial.h"
#include "IShaderConstantsCallback.h"

#include "TypedProperty.h"

namespace mray
{
	class ThreadJobManager;
namespace scene{
	class IRenderPass;
	class ISceneNode;
	class SMesh;
	class ISceneVisitor;

class MRAY_DLL IRenderable:public IObject
{
protected:
	int m_layer;
	ISceneNode* m_owner;

	bool m_shadowCaster;
	bool m_shadowReciver;

	int m_targetRenderGroup;
	bool m_customRenderGroup;

	bool m_useIdentityMatrix;

	virtual void _OnAttachedToOwner(){};
	virtual void _OnDeattachedFromOwner(){};

public:
	DECLARE_PROPERTY_TYPE(ShadowCaster,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ShadowReciver,bool,MRAY_DLL);
public:


	DECLARE_RTTI
	IRenderable();
	virtual ~IRenderable(){}

	virtual bool isCustomRender(){return false;}

	void SetUseIdentityMatrix(bool i);
	bool IsUseIdentityMatrix();

	int GetTargetRenderGroup();
	void SetTargetRenderGroup(int g);

	bool HasCustomRenderGroup();
	void SetHasCustomRenderGroup(bool e);

	virtual void setOwner(ISceneNode*o);
	ISceneNode* getOwner(){return m_owner;}

	int getLayer(){return m_layer;}
	void setLayer(int l){m_layer=l;}

	virtual bool preRender(IRenderPass*pass)=0;
	virtual bool render(IRenderPass*pass)=0;
	virtual void update(float dt)=0;
	virtual void AddUpdateJobs(ThreadJobManager* mngr,float dt){}

	virtual SMesh* getMesh();

	virtual const bool& isShadowCaster() {return m_shadowCaster;}
	virtual bool setShadowCaster(const bool &val) {m_shadowCaster=val;return true;}

	virtual const bool& isShadowReciver() {return m_shadowReciver;}
	virtual bool setShadowReciver(const bool &val) {m_shadowReciver=val;return true;}
	
	virtual void setMaterial(const video::RenderMaterialPtr& mtrl,int i=0)=0;
	virtual const video::RenderMaterialPtr& getMaterial(int i=0)=0;
	virtual int getMaterialCount()=0;

	virtual void setShader(int mesh,const video::GPUShaderPtr& shader)=0;
	virtual void setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback)=0;

	virtual const video::GPUShaderPtr& getShader(int i)=0;

	virtual math::box3d getBoundingBox();
	void NotifyChangeBoundingBox();

	virtual void OnVisit(ISceneVisitor*visitor);
};

MakeSharedPtrType(IRenderable);

typedef std::list<IRenderablePtr> RenderableList;

}
}





#endif



