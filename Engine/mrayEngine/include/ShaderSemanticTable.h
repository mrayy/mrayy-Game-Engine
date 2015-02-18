
/********************************************************************
	created:	2008/11/18
	created:	18:11:2008   20:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ShaderSemanticTable.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ShaderSemanticTable
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	shader uniforms manager
*********************************************************************/

#ifndef ___ShaderSemanticTable___
#define ___ShaderSemanticTable___

#include "ISingleton.h"
#include "GCPtr.h"


#include "SColor.h"
#include "SemanticTableVariable.h"

namespace mray{
	namespace scene{
		class LightNode;
		class IViewNode;
		class Skeleton;
	}
namespace video{
	class RenderPass;
	class IRenderTarget;
	class ITexture;
	class TextureUnit;

class MRAY_DLL ShaderSemanticTable:public ISingleton<ShaderSemanticTable>
{

	//////////////////////////////////////////////////////////////////////////
	/// Active Transformation

	bool m_ViewProjCalc;
	bool m_WorldViewCalc;
	bool m_WorldViewProjCalc;

	bool m_invWorldCalc;
	bool m_invViewCalc;
	bool m_invProjCalc;

	bool m_invViewProjCalc;
	bool m_invWorldViewCalc;
	bool m_invWorldViewProjCalc;

	const math::matrix4x4* m_worldMatricies;
	int m_worldMatriciesCount;

	math::matrix4x4 m_WorldMatrix;
	math::matrix4x4 m_ViewMatrix;
	math::matrix4x4 m_ProjectionMatrix;

	math::matrix4x4 m_ViewProjMatrix;
	math::matrix4x4 m_WorldViewMatrix;
	math::matrix4x4 m_WorldViewProjectionMatrix;

	math::matrix4x4 m_InvWorldMatrix;
	math::matrix4x4 m_InvViewMatrix;
	math::matrix4x4 m_InvProjectionMatrix;

	math::matrix4x4 m_InvViewProjMatrix;
	math::matrix4x4 m_InvWorldViewMatrix;
	math::matrix4x4 m_InvWorldViewProjectionMatrix;

	//////////////////////////////////////////////////////////////////////////
	/// Previous Transformation
	math::matrix4x4 m_PrevWorldMatrix;
	math::matrix4x4 m_PrevViewProjMatrix;
	math::matrix4x4 m_InvPrevViewProjMatrix;
	//////////////////////////////////////////////////////////////////////////
	SemanticTableVariable<math::matrix4x4> m_textureMatrix;

	SemanticTableVariable<video::TextureUnit*> m_textures;


	video::RenderPass* m_pass;

	SemanticTableVariable<scene::LightNode*>  m_lights;

	scene::IViewNode* m_viewNode;

	IRenderTarget* m_renderTarget;
	
	scene::Skeleton* m_skeleton;

	float m_dtime;
	double m_time;

	math::recti m_viewRect;

	video::SColor m_clearColor;
public:
	ShaderSemanticTable();
	virtual ~ShaderSemanticTable();

	void setWorldMatricies(const math::matrix4x4* m,int cnt);

	void setWorldMatrix(const math::matrix4x4&m);
	void setViewMatrix(const math::matrix4x4&m);
	void setProjectionMatrix(const math::matrix4x4&m);

	void setPrevWorldMatrix(const math::matrix4x4&m);

	void setTextureMatrix(int stage,const math::matrix4x4&m);

	void setTextureStage(video::TextureUnit* tex,int stage);

	void setRenderPass(video::RenderPass* m);

	void addLight(scene::LightNode* light);
	void disableLights();

	void setViewNode(scene::IViewNode* v);

	void setTime(double t);
	void setDTime(float t);

	void setRenderTarget(IRenderTarget* rt);

	void setViewport(int x,int y,int w,int h);

	void setClearColor(const video::SColor &c);

	void setSkeleton(scene::Skeleton* sk){m_skeleton=sk;}
	///
	const math::matrix4x4* getWorldMatricies();
	int getWorldMatriciesCount();

	math::matrix4x4& getWorldMatrix();
	math::matrix4x4&  getViewMatrix();
	math::matrix4x4&  getProjectionMatrix();


	math::matrix4x4& getViewProjMatrix();
	math::matrix4x4& getWorldViewMatrix();
	math::matrix4x4& getWorldViewProjectionMatrix();

	math::matrix4x4& getInvWorldMatrix();
	math::matrix4x4&  getInvViewMatrix();
	math::matrix4x4&  getInvProjectionMatrix();

	math::matrix4x4& getInvViewProjMatrix();
	math::matrix4x4& getInvWorldViewMatrix();
	math::matrix4x4& getInvWorldViewProjectionMatrix();

	const math::matrix4x4& getPrevWorldMatrix();
	const math::matrix4x4& getPrevViewMatrix();

	const math::matrix4x4& getPrevViewProjMatrix();
	const math::matrix4x4& getInvPrevViewProjMatrix();

	math::matrix4x4 getPrevWorldViewMatrix();
	math::matrix4x4 getInvPrevWorldViewMatrix();

	math::matrix4x4 getPrevWorldViewProjMatrix();
	math::matrix4x4 getInvPrevWorldViewProjMatrix();

	const math::matrix4x4&  getTextureMatrix(int stage);

	video::TextureUnit* getTextureStage(int stage);

	video::RenderPass* getRenderPass();

	scene::Skeleton* getSkeleton(){return m_skeleton;}

	int getLightsCount();
	scene::LightNode* getLight(int id);
	
	const math::vector3d& getLightDir(int id);

	scene::IViewNode* getViewNode();

	double getTime();
	float getDTime();

	IRenderTarget* getRenderTarget();
	const math::recti& getViewport();

	const video::SColor & getClearColor();

	video::ITexture* getShadowMap(int id);
	math::vector2di getShadowMapSize(int id);

	const math::matrix4x4&getLightViewProj(int id);

};

}
}


#endif //___ShaderSemanticTable___

