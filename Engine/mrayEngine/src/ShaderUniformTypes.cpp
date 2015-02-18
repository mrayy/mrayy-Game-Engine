

#include "stdafx.h"
#include "ShaderUniformTypes.h"


namespace mray
{
namespace video
{

UniformSettings ShaderSemantic_Str[]={
	UniformSettings("Unkown",false),

	UniformSettings("WorldMatricies",true),

	UniformSettings("WorldMat",true),
	UniformSettings("ViewMat",false),
	UniformSettings("ProjMat",false),
	UniformSettings("ViewProjMat",true),
	UniformSettings("WorldViewMat",true),
	UniformSettings("WorldViewProjMat",true),

	UniformSettings("InvWorldMat",true),
	UniformSettings("InvViewMat",false),
	UniformSettings("InvProjMat",false),
	UniformSettings("InvViewProjMat",true),
	UniformSettings("InvWorldViewMat",true),
	UniformSettings("InvWorldViewProjMat",true),

	UniformSettings("PrevWorldMat",true),
	UniformSettings("PrevViewMat",false),
	UniformSettings("PrevViewProjMat",false),
	UniformSettings("PrevWorldViewMat",true),
	UniformSettings("PrevWorldViewProjMat",true),

	UniformSettings("InvPrevWorldMat",true),
	UniformSettings("InvPrevViewMat",false),
	UniformSettings("InvPrevViewProjMat",false),
	UniformSettings("InvPrevWorldViewMat",true),
	UniformSettings("InvPrevWorldViewProjMat",true),

	UniformSettings("TexMat",false),
	UniformSettings("Tex",false),
	UniformSettings("TexSize",false),
	UniformSettings("LightPos",false),
	UniformSettings("LightDir",false),
	UniformSettings("LightDiff",false),

	UniformSettings("Ambient",false),
	UniformSettings("Diffuse",false),
	UniformSettings("Specular",false),
	UniformSettings("Emissive",false),
	UniformSettings("Shininess",false),
	UniformSettings("Alpha",false),

	UniformSettings("ViewPos",false),
	UniformSettings("ZNear",false),
	UniformSettings("ZFar",false),
	UniformSettings("FarCorner",false),

	UniformSettings("Time",false),
	UniformSettings("dt",false),

	UniformSettings("viewPortSize",false),
	UniformSettings("ClearColor",false),
	UniformSettings("rtColor",false),
	UniformSettings("rtDepth",false),

	UniformSettings("shadowMap",false),
	UniformSettings("shadowMapSize",false),
	UniformSettings("LightViewProj",false),

	UniformSettings("JointPalette",true)
	//UniformSettings("SkinMatricies",false)
};

MRAY_DLL const UniformSettings* UniformSemanticToString(EShaderSemantic s)
{
	if(s<ESS_Count)
		return &ShaderSemantic_Str[s];
	return 0;
}


}
}

