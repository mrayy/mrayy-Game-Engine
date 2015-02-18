
/********************************************************************
	created:	2009/01/04
	created:	4:1:2009   22:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\Engine\MaterialScript.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\Engine
	file base:	MaterialScript
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MaterialScript___
#define ___MaterialScript___

#include "GCPtr.h"
#include "IStream.h"
#include "ILexical.h"
#include "ITexture.h"
#include "ISingleton.h"
#include "RenderMaterial.h"
#include "GPUShader.h"

namespace mray{
namespace video{
	class SColor;
	class TextureUnit;
	enum EBlendFactor;
	enum EState;
	enum ETextureClamp;
	enum ETextureGenMode;
	enum EMaterialRenderType;
	enum EAlphaFunc;
	class RenderTechnique;
	class RenderPass;

}
namespace script{

	enum{
		LEX_Material_ID		=script::ERT_Count+1,
		LEX_Technique_ID,
		LEX_Pass_ID,


		LEX_OpenBrace_ID		,
		LEX_ClosedBrace_ID		,
		LEX_OpenBraket_ID		,
		LEX_ClosedBraket_ID	,

		LEX_Value_ID			,
		LEX_Type_ID			,


		LEX_Use_ID				,
		LEX_DontUse_ID			,
		LEX_Unkwon_ID			,

		LEX_Texture_ID			,
		LEX_TextureUnit_ID	,
		LEX_Tex1D_ID		,
		LEX_Tex2D_ID		,
		LEX_Tex3D_ID		,
		LEX_TexCube_ID		,

		LEX_WrapS_ID		,
		LEX_WrapT_ID		,
		LEX_WrapR_ID		,

		LEX_Repeat_ID		,
		LEX_Clamp_ID		,
		LEX_Clamp_To_Edge_ID	,
		LEX_Mirror_ID	,

		LEX_EnableGenS_ID	,
		LEX_EnableGenT_ID	,
		LEX_EnableGenR_ID	,
		LEX_EnableGenQ_ID	,

		LEX_AlphaFunc_ID	,
		LEX_AlphaRef_ID	,

		LEX_Never_ID	,
		LEX_Equal_ID	,
		LEX_LessEqual_ID	,
		LEX_Greater_ID	,
		LEX_NotEqual_ID	,
		LEX_GreaterEqual_ID	,
		LEX_Always_ID	,

		LEX_GenS_ID			,
		LEX_GenT_ID			,
		LEX_GenR_ID			,
		LEX_GenQ_ID			,

		LEX_EyeLinear_ID	,
		LEX_ObjectLinear_ID	,
		LEX_SphereMap_ID	,

		LEX_Filtering_ID	,
		LEX_Bilinear_ID	,
		LEX_Trilinear_ID	,

		LEX_Ambient_ID		,
		LEX_Diffuse_ID		,
		LEX_Specular_ID	,
		LEX_Emissive_ID	,
		LEX_Shininess_ID	,

		LEX_BlendRenderer_ID	,
		LEX_Solid_ID			,
		LEX_Add_ID				,
		LEX_AddAlpha_ID			,
		LEX_Transparent_ID		,
		LEX_Mul_ID			,

		LEX_SrcBlend_ID		,
		LEX_DstBlend_ID		,

		LEX_Zero_ID		,
		LEX_One_ID		,
		LEX_SrcAlpha_ID		,
		LEX_InvSrcAlpha_ID	,
		LEX_DstAlpha_ID		,
		LEX_InvDstAlpha_ID	,
		LEX_SrcColor_ID		,
		LEX_InvSrcColor_ID	,
		LEX_DstColor_ID		,
		LEX_InvDstColor_ID	,
		LEX_AlphaSaturate_ID	,


		LEX_BlendEnable_ID	,
		LEX_AlphaTestEnable_ID	,
		LEX_Lighting_ID	,
		LEX_ZWrite_ID		,
		LEX_Wireframe_ID	,
		LEX_Points_ID		,
		LEX_CullFace_ID	,
		LEX_Fog_ID			,
		LEX_Smooth_ID		,
		LEX_ZTest_ID		,
		LEX_Alpha_ID		,

		LEX_CullFaceDir_ID,
		LEX_CW_ID,
		LEX_CCW_ID,

		LEX_Thickness_ID	,

		LEX_ShaderRef_ID		,
		LEX_Shader_ID		,
		LEX_ShaderVP_ID		,
		LEX_ShaderFP_ID		,

		LEX_MATERIAL_COUNT
	};

class IMaterialScriptCallback 
{
public:
	virtual bool visitInPass(script::ScriptToken*token,video::RenderPass*)=0;
	virtual bool visitInTexture(script::ScriptToken*token,const video::ITexturePtr& tex)=0;
};

class MaterialScript: public ISingleton<MaterialScript>
{
	GCPtr<script::ILexical> m_lex;
	GCPtr<script::CTokenList> m_tokenList;

	script::ScriptToken* nextToken();
	script::ScriptToken* peekToken();
	bool lookAhead(const mchar*string);

	bool parseColor(video::SColor&c);
	bool parseFloat(float&c);
	bool parseBool(bool&c);
	bool parseState(video::EState&c);
	bool parseAddressMode(video::ETextureClamp&c);
	bool parseGenMode(video::ETextureGenMode&c);
	video::EMaterialRenderType parseRenderType();

	bool parseMaterial(video::RenderMaterial* mtrl);
	video::RenderTechnique* parseTechnique();
	video::RenderPass* parsePass();
	GCPtr<video::TextureUnit> parseTexture();
	bool parseAttribute(video::RenderPass* mtrl);

	bool parseAlphaFunc(video::EAlphaFunc&f);

	bool parseBlendOp(video::EBlendFactor&op);

	GCPtr<video::GPUShader> parseShader();

	bool parseTextureAttr(GCPtr<video::TextureUnit> tex);

	std::vector<GCPtr<IMaterialScriptCallback>> m_callback;

	bool visitInPass(script::ScriptToken*token,video::RenderPass* mtrl);
	bool visitInTexture(script::ScriptToken*token,const video::ITexturePtr& tex);



	friend class IMaterialScriptCallback;
public:
	MaterialScript();
	virtual ~MaterialScript();

	int loadMaterialScript(OS::IStream* stream);
	void addDefinitions(script::TokenDefine*definitions);

	void addCallback(GCPtr<IMaterialScriptCallback> callback);
};

}
}


#endif //___MaterialScript___
