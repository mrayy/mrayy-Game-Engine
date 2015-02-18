

/********************************************************************
	created:	2012/09/02
	created:	2:9:2012   21:27
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ShaderUniformTypes.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ShaderUniformTypes
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ShaderUniformTypes___
#define ___ShaderUniformTypes___


#include "CompileConfig.h"
#include "mstring.h"

namespace mray
{
namespace video
{


enum EShaderSemantic{
	ESS_Unkown,

	ESS_WorldMatricies,

	ESS_WorldMatrix,
	ESS_ViewMatrix,
	ESS_ProjectionMatrix,
	ESS_ViewProjMatrix,
	ESS_WorldViewMatrix,
	ESS_WorldViewProjMatrix,

	ESS_InvWorldMatrix,
	ESS_InvViewMatrix,
	ESS_InvProjectionMatrix,
	ESS_InvViewProjMatrix,
	ESS_InvWorldViewMatrix,
	ESS_InvWorldViewProjMatrix,

	ESS_PrevWorldMatrix,
	ESS_PrevViewMatrix,
	ESS_PrevViewProjMatrix,
	ESS_PrevWorldViewMatrix,
	ESS_PrevWorldViewProjMatrix,

	ESS_InvPrevWorldMatrix,
	ESS_InvPrevViewMatrix,
	ESS_InvPrevViewProjMatrix,
	ESS_InvPrevWorldViewMatrix,
	ESS_InvPrevWorldViewProjMatrix,

	ESS_TexMatrix,
	ESS_Texture,
	ESS_TextureSize,
	ESS_LightPos,
	ESS_LightDir,
	ESS_LightDiff,

	ESS_Ambient,
	ESS_Diffuse,
	ESS_Specular,
	ESS_Emissive,
	ESS_Shininess,
	ESS_Alpha,

	ESS_ViewPos,

	ESS_ZNear,
	ESS_ZFar,
	ESS_FarCorner,

	ESS_Time,
	ESS_DTime,

	ESS_ViewPortSize,
	ESS_ClearColor,

	ESS_rtColor,
	ESS_rtDepth,

	ESS_shadowMap,
	ESS_shadowMapSize,

	ESS_LightViewProj,

	ESS_SkinMatricies,

	ESS_Count
};

struct UniformSettings
{
	UniformSettings(const char*n,bool perO){
		name=n;
		perObject=perO;
	}
	core::stringc name;
	bool perObject;
};


MRAY_DLL const UniformSettings* UniformSemanticToString(EShaderSemantic s);


}
}

#endif
