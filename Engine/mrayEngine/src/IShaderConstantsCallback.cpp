#include "stdafx.h"

#include "IShaderConstantsCallback.h"
#include "GPUShader.h"
#include "ShaderSemanticTable.h"
#include "RenderPass.h"
#include "ITexture.h"
#include "LightNode.h"
#include "IRenderTarget.h"
#include "ITexture.h"
#include "DefaultColors.h"

#include "GenericValues.h"
#include "MathValues.h"
#include "VideoValues.h"
#include "Skeleton.h"
#include "CameraNode.h"

#include "ShaderUniformTypes.h"


namespace mray{
namespace video{

// 	TextureUnit IShaderConstantsCallback::m_sharedShadowMapTU;
// 	TextureUnit IShaderConstantsCallback::m_sharedRTTU;

IShaderConstantsCallback::IShaderConstantsCallback()
{
	m_sharedShadowMapTU.SetEdgeColor(video::DefaultColors::White);
	m_sharedShadowMapTU.setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_BORDER);
	m_sharedShadowMapTU.setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_BORDER);
	m_sharedShadowMapTU.setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_BORDER);

	m_sharedRTTU.setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
	m_sharedRTTU.setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
	m_sharedRTTU.setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);
}

IShaderConstantsCallback::~IShaderConstantsCallback()
{
}

void IShaderConstantsCallback::_SetConstants(IGPUShaderProgram*p,IShaderConstantsCallback::ESetUniformType ut)
{
	GPUUniform* u;
	UniformListIT it= p->beginUniforms();
	UniformListIT end= p->endUniforms();
	for(;it!=end;++it){
		u=it->second;
		if(ut==ESUT_Both || u->perObject && ut==ESUT_Object || !u->perObject && ut==ESUT_Global )
			setUniform(p,u);
	}
}
void IShaderConstantsCallback::setConstants(GPUShader*shader,IShaderConstantsCallback::ESetUniformType ut){
	if(!shader)return;

	if(shader->GetVertexShader())
		_SetConstants(shader->GetVertexShader(),ut);
	if(shader->GetFragmentShader())
		_SetConstants(shader->GetFragmentShader(),ut);
}

void IShaderConstantsCallback::setUniform(IGPUShaderProgram*shader,GPUUniform* u){

	static float tmpf;
	static math::vector3d tmpV;
	scene::LightNode* tmpL;
	static math::recti tmpRc;
	static math::vector2d tmpV2;
	IRenderTarget* rt;
	ITexture* tex;

	switch(u->semantic){
		case ESS_WorldMatricies:
			shader->setMatrixArray(u,ShaderSemanticTable::getInstance().getWorldMatricies(),ShaderSemanticTable::getInstance().getWorldMatriciesCount());
			break;
		case ESS_WorldMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getWorldMatrix().getMatPointer(),16);
			break;
		case ESS_ViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getViewMatrix().getMatPointer(),16);
			break;
		case ESS_ProjectionMatrix:
			{
				;
				//shader->setConstant(u, ShaderSemanticTable::getInstance().getViewNode()->getProjectionMatrix().getMatPointer(),16);
				shader->setConstant(u,ShaderSemanticTable::getInstance().getProjectionMatrix().getMatPointer(),16);
			}
			break;
		case ESS_ViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getViewProjMatrix().getMatPointer(),16);
			break;
		case ESS_WorldViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getWorldViewMatrix().getMatPointer(),16);
			break;
		case ESS_WorldViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getWorldViewProjectionMatrix().getMatPointer(),16);
			break;
		case ESS_InvWorldMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvWorldMatrix().getMatPointer(),16);
			break;
		case ESS_InvViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvViewMatrix().getMatPointer(),16);
			break;
		case ESS_InvProjectionMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvProjectionMatrix().getMatPointer(),16);
			break;
		case ESS_InvViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvViewProjMatrix().getMatPointer(),16);
			break;
		case ESS_InvWorldViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvWorldViewMatrix().getMatPointer(),16);
			break;
		case ESS_InvWorldViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvWorldViewProjectionMatrix().getMatPointer(),16);
			break;
		case ESS_PrevWorldMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getPrevWorldMatrix().getMatPointer(),16);
			break;
		case ESS_PrevViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getPrevViewMatrix().getMatPointer(),16);
			break;
		case ESS_PrevViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getPrevViewProjMatrix().getMatPointer(),16);
			break;
		case ESS_PrevWorldViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getPrevWorldViewMatrix().getMatPointer(),16);
			break;
		case ESS_PrevWorldViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getPrevWorldViewProjMatrix().getMatPointer(),16);
			break;
		case ESS_InvPrevWorldMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvWorldMatrix().getMatPointer(),16);
			break;
		case ESS_InvPrevViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvViewMatrix().getMatPointer(),16);
			break;
		case ESS_InvPrevViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvPrevViewProjMatrix().getMatPointer(),16);
			break;
		case ESS_InvPrevWorldViewMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvPrevWorldViewMatrix().getMatPointer(),16);
			break;
		case ESS_InvPrevWorldViewProjMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getInvPrevWorldViewProjMatrix().getMatPointer(),16);
			break;
		case ESS_TexMatrix:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(u->index).getMatPointer(),16);
			break;
		case ESS_Texture:
			{
				core::string nameStr;
				core::char_to_string(u->name.c_str(),nameStr);
			shader->setTexture(nameStr,ShaderSemanticTable::getInstance().getTextureStage(u->index));
			}
			break;
		case ESS_LightPos:
			tmpL=ShaderSemanticTable::getInstance().getLight(u->index);
			if(!tmpL)return;
			tmpV=tmpL->getAbsolutePosition();
			shader->setConstant(u,&tmpV.x,3);
			break;
		case ESS_LightDir:
			tmpV=ShaderSemanticTable::getInstance().getLightDir(u->index);
			shader->setConstant(u,&tmpV.x,3);
			break;

		case ESS_LightDiff:
			tmpL=ShaderSemanticTable::getInstance().getLight(u->index);
			if(!tmpL)return;
			shader->setConstant(u,tmpL->getDiffuse().ptr(),4);
			break;
		case ESS_Ambient:
			if(ShaderSemanticTable::getInstance().getRenderPass())
				shader->setConstant(u,ShaderSemanticTable::getInstance().getRenderPass()->GetAmbient().ptr(),4);
			break;
		case ESS_Diffuse:

			if(ShaderSemanticTable::getInstance().getRenderPass())
				shader->setConstant(u,ShaderSemanticTable::getInstance().getRenderPass()->GetDiffuse().ptr(),4);
			break;
		case ESS_Specular:
			if(ShaderSemanticTable::getInstance().getRenderPass())
				shader->setConstant(u,ShaderSemanticTable::getInstance().getRenderPass()->GetSpecular().ptr(),4);
			break;
		case ESS_Emissive:
			

			if(ShaderSemanticTable::getInstance().getRenderPass())
				shader->setConstant(u,ShaderSemanticTable::getInstance().getRenderPass()->GetEmissive().ptr(),4);
			break;
		case ESS_Shininess:
			if(ShaderSemanticTable::getInstance().getRenderPass())
			{
				tmpf=ShaderSemanticTable::getInstance().getRenderPass()->GetShininess();
				shader->setConstant(u,&tmpf,1);
			}
			break;
		case ESS_Alpha:
			if(ShaderSemanticTable::getInstance().getRenderPass())
			{
				tmpf=ShaderSemanticTable::getInstance().getRenderPass()->GetAlpha();
				shader->setConstant(u,&tmpf,1);
			}
			break;
		case ESS_ViewPos:
			if(ShaderSemanticTable::getInstance().getViewNode()){
				tmpV=ShaderSemanticTable::getInstance().getViewNode()->getViewPos();
				shader->setConstant(u,&tmpV.x,3);
			}
			break;
		case ESS_ZFar:
			{
				scene::CameraNode* c=dynamic_cast<scene::CameraNode*>(ShaderSemanticTable::getInstance().getViewNode());
				if(c){
					tmpf=c->getZFar();
					shader->setConstant(u,&tmpf,1);
				}
			}
			break;
		case ESS_ZNear:
			{
				scene::CameraNode* c=dynamic_cast<scene::CameraNode*>(ShaderSemanticTable::getInstance().getViewNode());
				if(c){
					tmpf=c->getZNear();
					shader->setConstant(u,&tmpf,1);
				}
			}
			break;
		case ESS_FarCorner:
			{
				scene::IViewNode* v=ShaderSemanticTable::getInstance().getViewNode();
				if(v){
					tmpV=v->getViewMatrix()*v->getViewFrustrum()->getFarRUp();
					shader->setConstant(u,&tmpV.x,3);
				}
			}
			break;
		case ESS_Time:
			tmpf=ShaderSemanticTable::getInstance().getTime();
			shader->setConstant(u,&tmpf,1);
			break;
		case ESS_DTime:
			tmpf=ShaderSemanticTable::getInstance().getDTime();
			shader->setConstant(u,&tmpf,1);
			break;
		case ESS_ViewPortSize:
			tmpV2=ShaderSemanticTable::getInstance().getViewport().getSize();
			shader->setConstant(u,&tmpV2.x,2);
			break;
		case ESS_ClearColor:
			
			shader->setConstant(u,ShaderSemanticTable::getInstance().getClearColor().ptr(),4);
			break;
		case ESS_rtColor:
			rt=ShaderSemanticTable::getInstance().getRenderTarget();
			if(rt)
			{
				m_sharedRTTU.SetTexture(rt->GetColorTexture());
				shader->setTexture(mT("rtColor"),&m_sharedRTTU);
			}
			else
				shader->setTexture(mT("rtColor"),0);
			break;
		case ESS_rtDepth:
			/*rt=ShaderSemanticTable::getInstance().getRenderTarget();
			if(rt)
				shader->setTexture("rtDepth",rt->getDepthTexture());
			else
				shader->setTexture("rtDepth",0);*/
			break;
		case ESS_shadowMap:
			tex=ShaderSemanticTable::getInstance().getShadowMap(0);
			m_sharedShadowMapTU.SetTexture(tex);
			shader->setTexture(mT("shadowMap"),&m_sharedShadowMapTU);
			break;
		case ESS_shadowMapSize:
			tmpV2=ShaderSemanticTable::getInstance().getShadowMapSize(0);
			shader->setConstant(u,&tmpV2.x,2);
			break;
		case ESS_LightViewProj:
			shader->setConstant(u,ShaderSemanticTable::getInstance().getLightViewProj(0).getMatPointer(),16);
			break;
		case ESS_SkinMatricies:
			if(ShaderSemanticTable::getInstance().getSkeleton())
			{
				const std::vector<math::matrix4x4>& arr=ShaderSemanticTable::getInstance().getSkeleton()->GetBonesMatricies();
				if(arr.size()>0)
					shader->setMatrixArray(u,&arr[0],arr.size());
			}
			break;
		default:
			{
				RenderPass* mtrl=ShaderSemanticTable::getInstance().getRenderPass();
				if(!mtrl)break;
				IValue*v= mtrl->getValue(core::StringConverter::toString(u->name));
				if(!v)break;
				setUniformByValue(shader,u,v);
			}
			break;
	}
	tmpL=0;
	rt=0;
}

void IShaderConstantsCallback::setUniformByValue(IGPUShaderProgram*shader,GPUUniform* u,IValue*v)
{

	static float color[4];
	static float tmpi;
	static float tmpf;
	static math::vector4d tmp4V;
	static math::vector3d tmp3V;
	static math::vector2d tmp2V;
	static math::recti tmpRc;
	static math::vector2d tmpV2;


	switch (v->getType())
	{
	case EVT_COLOR:
		{
			if(u->type!=EUT_Float4)break;
			ColorValue*val=dynamic_cast<ColorValue*>(v);
			shader->setConstant(u,val->value.ptr(),4);
		}break;
	case EVT_INT:
		{
			if(u->type!=EUT_Int1)break;
			IntValue*val=dynamic_cast<IntValue*>(v);
			tmpf=val->value;
			shader->setConstant(u,&tmpf,1);
		}break;
	case EVT_VECTOR2Di:
		{
			if(u->type!=EUT_Int2)break;
			Vector2diValue*val=dynamic_cast<Vector2diValue*>(v);
			tmp2V=val->value;
			shader->setConstant(u,&tmp2V.x,2);
		}break;
	case EVT_VECTOR3Di:
		{
			if(u->type!=EUT_Int3)break;
			Vector3diValue*val=dynamic_cast<Vector3diValue*>(v);
			tmp3V=val->value;
			shader->setConstant(u,&tmp3V.x,3);
		}break;
	case EVT_VECTOR4Di:
		{
			if(u->type!=EUT_Int4)break;
			Vector4diValue*val=dynamic_cast<Vector4diValue*>(v);
			tmp4V.set(val->value.x,val->value.y,val->value.z,val->value.w);
			shader->setConstant(u,&tmp4V.x,4);
		}break;
	case EVT_FLOAT:
		{
			if(u->type!=EUT_Float1)break;
			FloatValue*val=dynamic_cast<FloatValue*>(v);
			shader->setConstant(u,&val->value,1);
		}break;
	case EVT_VECTOR2Df:
		{
			if(u->type!=EUT_Float2)break;
			Vector2dfValue*val=dynamic_cast<Vector2dfValue*>(v);
			shader->setConstant(u,&val->value.x,2);
		}break;
	case EVT_VECTOR3Df:
		{
			if(u->type!=EUT_Float3)break;
			Vector3dfValue*val=dynamic_cast<Vector3dfValue*>(v);
			shader->setConstant(u,&val->value.x,3);
		}break;
	case EVT_VECTOR4Df:
		{
			if(u->type!=EUT_Float4)break;
			Vector4dfValue*val=dynamic_cast<Vector4dfValue*>(v);
			shader->setConstant(u,&val->value.x,4);
		}break;

	case EVT_MATRIX3x3:
		{
			if(u->type!=EUT_Matrix3x3)break;
			Matrix3x3Value*val=dynamic_cast<Matrix3x3Value*>(v);
			shader->setConstant(u,val->value.pointer(),9);
		}break;
	case EVT_MATRIX4x4:
		{
			if(u->type!=EUT_Matrix4x4)break;
			Matrix4x4Value*val=dynamic_cast<Matrix4x4Value*>(v);
			shader->setConstant(u,val->value.getMatPointer(),16);
		}break;
	case EVT_TEXTURE:
		{
			TextureValue*val=dynamic_cast<TextureValue*>(v);
			if(u->type!= EUT_Sampler1D && u->type!= EUT_Sampler2D && 
				u->type!= EUT_Sampler3D && u->type!= EUT_SamplerCube)
				break;
			core::string nameStr;
			core::char_to_string(u->name.c_str(),nameStr);
			if(val->value)
				shader->setTexture(nameStr,val->value);
			else
				shader->setTexture(nameStr,0);
		}break;
	}
}

/*
void IShaderConstantsCallback::setUniform(GPUShader*shader,GCPtr<GPUUniform> u,bool VertexU){
	if(u->name.equals_ignore_case("ViewMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getViewMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getViewMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("WorldMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getWorldMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getWorldMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("ProjMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getProjectionMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getProjectionMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("WorldViewMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getWorldViewMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getWorldViewMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("WorldViewProjMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getWorldViewProjectionMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getWorldViewProjectionMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("InvViewMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getInvViewMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getInvViewMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("InvWorldMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getInvWorldMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getInvWorldMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("InvProjMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getInvProjectionMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getInvProjectionMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("InvWorldViewMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getInvWorldViewMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getInvWorldViewMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("InvWorldViewProjMat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getInvWorldViewProjectionMatrix().getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getInvWorldViewProjectionMatrix().getMatPointer(),16);
	}else if(u->name.equals_ignore_case("tex0")){
			shader->setTexture(0,ShaderSemanticTable::getInstance().getTextureStage(0));
	}else if(u->name.equals_ignore_case("tex1")){
		shader->setTexture(1,ShaderSemanticTable::getInstance().getTextureStage(1));
	}else if(u->name.equals_ignore_case("tex2")){
		shader->setTexture(2,ShaderSemanticTable::getInstance().getTextureStage(2));
	}else if(u->name.equals_ignore_case("tex3")){
		shader->setTexture(3,ShaderSemanticTable::getInstance().getTextureStage(3));
	else if(u->name.equals_ignore_case("tex0mat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(0).getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(0).getMatPointer(),16);
	}else if(u->name.equals_ignore_case("tex1mat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(1).getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(1).getMatPointer(),16);
	}else if(u->name.equals_ignore_case("tex2mat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(2).getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(2).getMatPointer(),16);
	}else if(u->name.equals_ignore_case("tex3mat")){
		if(VertexU)
			shader->setVertexShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(3).getMatPointer(),16);
		else
			shader->setPixelShaderConstant(u,ShaderSemanticTable::getInstance().getTextureMatrix(3).getMatPointer(),16);
	}else if(u->name.equals_ignore_case("ambient")){
		float color[4];
		ShaderSemanticTable::getInstance().getMaterial()->ambientColor.FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("diffuse")){
		float color[4];
		ShaderSemanticTable::getInstance().getMaterial()->diffuseColor.FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("specular")){
		float color[4];
		ShaderSemanticTable::getInstance().getMaterial()->specularColor.FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("shininess")){
		float v=ShaderSemanticTable::getInstance().getMaterial()->shininess;
		if(VertexU)
			shader->setVertexShaderConstant(u,&v,1);
		else
			shader->setPixelShaderConstant(u,&v,1);
	}else if(u->name.equals_ignore_case("alpha")){
		float v=ShaderSemanticTable::getInstance().getMaterial()->Alpha*math::i255;
		if(VertexU)
			shader->setVertexShaderConstant(u,&v,1);
		else
			shader->setPixelShaderConstant(u,&v,1);
	}else if(u->name.equals_ignore_case("CamPos")){
		
		math::vector3d p=ShaderSemanticTable::getInstance().getViewCamera()->getAbsolutePosition();
		if(VertexU)
			shader->setVertexShaderConstant(u,&p.x,3);
		else
			shader->setPixelShaderConstant(u,&p.x,3);
	}else  if(u->name.equals_ignore_case("light0Pos")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(0);
		if(!l)return;
		math::vector3d p=l->getAbsolutePosition();
		if(VertexU)
			shader->setVertexShaderConstant(u,&p.x,3);
		else
			shader->setPixelShaderConstant(u,&p.x,3);
	}else if(u->name.equals_ignore_case("light1Pos")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(1);
		if(!l)return;
		math::vector3d p=l->getAbsolutePosition();
		if(VertexU)
			shader->setVertexShaderConstant(u,&p.x,3);
		else
			shader->setPixelShaderConstant(u,&p.x,3);
	}else if(u->name.equals_ignore_case("light2Pos")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(2);
		if(!l)return;
		math::vector3d p=l->getAbsolutePosition();
		if(VertexU)
			shader->setVertexShaderConstant(u,&p.x,3);
		else
			shader->setPixelShaderConstant(u,&p.x,3);
	}else if(u->name.equals_ignore_case("light3Pos")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(3);
		if(!l)return;
		math::vector3d p=l->getAbsolutePosition();
		if(VertexU)
			shader->setVertexShaderConstant(u,&p.x,3);
		else
			shader->setPixelShaderConstant(u,&p.x,3);
	}else if(u->name.equals_ignore_case("light0Diff")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(0);
		if(!l)return;
		float color[4];
		l->getLightProperities().diffuseColor().FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("light1Diff")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(1);
		if(!l)return;
		float color[4];
		l->getLightProperities().diffuseColor().FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("light2Diff")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(2);
		if(!l)return;
		float color[4];
		l->getLightProperities().diffuseColor().FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("light3Diff")){
		GCPtr<scene::LightNode> l=ShaderSemanticTable::getInstance().getLight(3);
		if(!l)return;
		float color[4];
		l->getLightProperities().diffuseColor().FloatArray(color);
		if(VertexU)
			shader->setVertexShaderConstant(u,color,4);
		else
			shader->setPixelShaderConstant(u,color,4);
	}else if(u->name.equals_ignore_case("time")){
		float v=ShaderSemanticTable::getInstance().getTime();
		if(VertexU)
			shader->setVertexShaderConstant(u,&v,1);
		else
			shader->setPixelShaderConstant(u,&v,1);
	}else  if(u->name.equals_ignore_case("dt")){
		float v=ShaderSemanticTable::getInstance().getDTime();
		if(VertexU)
			shader->setVertexShaderConstant(u,&v,1);
		else
			shader->setPixelShaderConstant(u,&v,1);
	} 
}*/

}
}




