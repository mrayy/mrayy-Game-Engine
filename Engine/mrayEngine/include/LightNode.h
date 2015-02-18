
#ifndef ___LIGHT_NODE___
#define ___LIGHT_NODE___

#include "ISceneNode.h"
#include "SColor.h"
#include "IViewNode.h"
#include "IRenderTarget.h"
#include "IRenderPass.h"
#include "IRenderPassSubscriber.h"


namespace mray{
namespace scene{

	class LightNodeDesc;

enum ELightType
{
	LT_PointLight,
	LT_DirectionalLight,
	LT_SpotLight
};

class MRAY_DLL LightNode:public ISceneNode,public IViewNode,public IRenderPassSubscriber
{
	DECLARE_RTTI
protected:


	math::Frustum m_frustrum;
	math::matrix4x4 projectionMatrix;
	math::matrix4x4 viewMatrix;
	math::matrix4x4 m_prevViewMatrix;
	math::matrix4x4 m_shadowViewProjMatrix;

	math::vector3d m_target;
	math::vector3d m_dir;

	bool m_enabled;


	video::SColor m_ambientColor;
	video::SColor m_diffuseColor;
	video::SColor m_specularColor;
	float m_radius;

	ELightType m_type;

	bool m_castShadows;

	IRenderPassPtr m_renderPass;
	video::IRenderTargetPtr m_shadowMap;

	void updateFrustrum();
	virtual void fillProperties();

public:

	DECLARE_PROPERTY_TYPE(Enable,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(CastShadows,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Target,math::vector3d,MRAY_DLL);

	DECLARE_PROPERTY_TYPE(Ambient,video::SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Diffuse,video::SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Specular,video::SColor,MRAY_DLL);

	DECLARE_PROPERTY_TYPE(Radius,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Type,ELightType,MRAY_DLL);


	LightNode(const  core::string&name,int id,ISceneManager* sceneMngr);
	virtual ~LightNode();

	virtual int GetNodeType();

	bool isEnabled()const;
	bool setEnabled(const bool& e);

	virtual void preRender();
	virtual void render(IRenderPass*pass);
	virtual void update(float dt);

	void setShadowMap(const video::IRenderTargetPtr& m);
	const video::IRenderTargetPtr& getShadowMap()const;

	bool setTarget(const math::vector3d& val);
	const math::vector3d& getTarget()const;

	bool setType(const ELightType& type);
	ELightType getType()const;

	bool isCastShadows()const;
	bool setCastShadows(const bool& cs);


	bool setAmbient(const video::SColor&  val);
	const video::SColor& getAmbient()const;

	bool setDiffuse(const video::SColor&  val);
	const video::SColor&  getDiffuse()const;

	bool setSpecular(const video::SColor&  val);
	const  video::SColor&  getSpecular()const;

	bool setRadius(const float &val);
	float getRadius()const;


	const math::vector3d& getDir()const;

	void setProjection(const math::matrix4x4&m);

	virtual const math::matrix4x4&getProjectionMatrix()const;
	virtual const math::matrix4x4&getViewMatrix()const;
	virtual const math::matrix4x4&getPrevViewMatrix()const;

	virtual const math::matrix4x4&getShadowViewProjMatrix();

	virtual math::vector3d getViewPos()const;
	virtual const math::Frustum*getViewFrustrum()const;

	virtual math::box3d getTransformedBoundingBox();


	virtual bool isCulling(ICullable* node)const;

	void GetRenderPasses(std::list<IRenderPass*>& pOut);

	void prepareForShadowPass(IViewNode* cam,IRenderPass* pass);

	virtual void OnVisit(ISceneVisitor*visitor);

	void SaveToDesc(LightNodeDesc* desc)const;
	void LoadFromDesc(const LightNodeDesc* desc);

};

MakeSharedPtrType(LightNode);


}
}



#endif






