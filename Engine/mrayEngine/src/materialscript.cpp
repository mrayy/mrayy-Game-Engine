#include "stdafx.h"

#include "MaterialScript.h"
#include <CTokenTranslator.h>
#include <RenderMaterial.h>
#include <RenderTechnique.h>
#include <RenderPass.h>
#include <LogManager.h>
#include <TextureResourceManager.h>
#include <StringConverter.h>
#include <MaterialResourceManager.h>
#include <ITexture.h>
#include <ITexture.h>
#include <ITexture.h>
#include <ITexture.h>
#include <ShaderResourceManager.h>
#include <GPUShader.h>

#include <GenericValues.h>
#include <MathValues.h>
#include <VideoValues.h>

#include "StringUtil.h"

namespace mray{
	using namespace video;
namespace script{

#define nextTokenFromList(t)		t=nextToken();if(!t){gLogManager.log(mT("End of script!"),ELL_WARNING);return false;}
#define checkTokenTypeID(t,id,err)	if(t->token!=id){gLogManager.log(err,ELL_WARNING);return false;}
#define checkTokenTypeLex(t,id,err) if(!token->lex.equals_ignore_case(id)){gLogManager.log(err+core::string(mT(",line:"))+core::StringConverter::toString(token->line),ELL_WARNING);return false;}
#define nextTokenAndChkID(t,id,err) nextTokenFromList(t);checkTokenTypeID(t,id,err)
#define nextTokenAndChkLex(t,id,err) nextTokenFromList(t);checkTokenTypeLex(t,id,err)


script::TokenDefine tokenDefintion[]={
	script::TokenDefine(mT("material")		,LEX_Material_ID),
	script::TokenDefine(mT("technique")		,LEX_Technique_ID),
	script::TokenDefine(mT("pass")			,LEX_Pass_ID),
	script::TokenDefine(mT("value")			,LEX_Value_ID),
	script::TokenDefine(mT("type")		,LEX_Type_ID),
	script::TokenDefine(mT("texture")		,LEX_Texture_ID),
	script::TokenDefine(mT("unit")		,LEX_TextureUnit_ID),
	script::TokenDefine(mT("tex1D")			,LEX_Tex1D_ID),
	script::TokenDefine(mT("tex2D")			,LEX_Tex2D_ID),
	script::TokenDefine(mT("tex3D")			,LEX_Tex3D_ID),
	script::TokenDefine(mT("TexCube")		,LEX_TexCube_ID),

	script::TokenDefine(mT("use")			,LEX_Use_ID),
	script::TokenDefine(mT("dontUse")		,LEX_DontUse_ID),
	script::TokenDefine(mT("unkwon")			,LEX_Unkwon_ID),

	script::TokenDefine(mT("repeat")		,LEX_Repeat_ID),
	script::TokenDefine(mT("clamp")			,LEX_Clamp_ID),
	script::TokenDefine(mT("clamp_to_edge")	,LEX_Clamp_To_Edge_ID),
	script::TokenDefine(mT("mirror")		,LEX_Mirror_ID),

	script::TokenDefine(mT("WrapS")			,LEX_WrapS_ID),
	script::TokenDefine(mT("WrapT")			,LEX_WrapT_ID),
	script::TokenDefine(mT("WrapR")			,LEX_WrapR_ID),


	script::TokenDefine(mT("AlphaFunc")			,LEX_AlphaFunc_ID),
	script::TokenDefine(mT("AlphaRef")			,LEX_AlphaRef_ID),

	script::TokenDefine(mT("Never")				,LEX_Never_ID),
	script::TokenDefine(mT("Equal")				,LEX_Equal_ID),
	script::TokenDefine(mT("LessEqual")			,LEX_LessEqual_ID),
	script::TokenDefine(mT("Greater")			,LEX_Greater_ID),
	script::TokenDefine(mT("NotEqual")			,LEX_NotEqual_ID),
	script::TokenDefine(mT("GreaterEqual")		,LEX_GreaterEqual_ID),
	script::TokenDefine(mT("Always")			,LEX_Always_ID),

	script::TokenDefine(mT("GenS")			,LEX_GenS_ID),
	script::TokenDefine(mT("GenT")			,LEX_GenT_ID),
	script::TokenDefine(mT("GenR")			,LEX_GenR_ID),
	script::TokenDefine(mT("GenQ")			,LEX_GenQ_ID),

	script::TokenDefine(mT("EnableGenS")			,LEX_EnableGenS_ID),
	script::TokenDefine(mT("EnableGenT")			,LEX_EnableGenT_ID),
	script::TokenDefine(mT("EnableGenR")			,LEX_EnableGenR_ID),
	script::TokenDefine(mT("EnableGenQ")			,LEX_EnableGenQ_ID),

	script::TokenDefine(mT("EyeLinear")			,LEX_EyeLinear_ID),
	script::TokenDefine(mT("ObjectLinear")		,LEX_ObjectLinear_ID),
	script::TokenDefine(mT("SphereMap")			,LEX_SphereMap_ID),

	script::TokenDefine(mT("Filtering")			,LEX_Filtering_ID),
	script::TokenDefine(mT("bilinear")			,LEX_Bilinear_ID),
	script::TokenDefine(mT("trilinear")			,LEX_Trilinear_ID),

	script::TokenDefine(mT("ambient")		,LEX_Ambient_ID),
	script::TokenDefine(mT("diffuse")		,LEX_Diffuse_ID),
	script::TokenDefine(mT("specular")		,LEX_Specular_ID),
	script::TokenDefine(mT("emissive")		,LEX_Emissive_ID),

	script::TokenDefine(mT("Blend")			,LEX_BlendEnable_ID),
	script::TokenDefine(mT("lighting")		,LEX_Lighting_ID),
	script::TokenDefine(mT("zwrite")		,LEX_ZWrite_ID),
	script::TokenDefine(mT("wireframe")		,LEX_Wireframe_ID),
	script::TokenDefine(mT("points")		,LEX_Points_ID),
	script::TokenDefine(mT("cullface")		,LEX_CullFace_ID),
	script::TokenDefine(mT("fog")			,LEX_Fog_ID),
	script::TokenDefine(mT("smooth")		,LEX_Smooth_ID),
	script::TokenDefine(mT("ztest")			,LEX_ZTest_ID),
	script::TokenDefine(mT("alpha")			,LEX_Alpha_ID),
	script::TokenDefine(mT("thickness")		,LEX_Thickness_ID),
	script::TokenDefine(mT("alphaTest")		,LEX_AlphaTestEnable_ID),

	script::TokenDefine(mT("shader")		,LEX_Shader_ID),
	script::TokenDefine(mT("shaderRef")		,LEX_ShaderRef_ID),
	script::TokenDefine(mT("shaderVP")		,LEX_ShaderVP_ID),
	script::TokenDefine(mT("shaderFP")		,LEX_ShaderFP_ID),


	script::TokenDefine(mT("BlendRenderer")			,LEX_BlendRenderer_ID),
	script::TokenDefine(mT("Solid")			,LEX_Solid_ID),
	script::TokenDefine(mT("Add")			,LEX_Add_ID),
	script::TokenDefine(mT("AddAlpha")		,LEX_AddAlpha_ID),
	script::TokenDefine(mT("Transparent")	,LEX_Transparent_ID),
	script::TokenDefine(mT("Mul")			,LEX_Mul_ID),

	script::TokenDefine(mT("SrcBlend")		,LEX_SrcBlend_ID),
	script::TokenDefine(mT("DstBlend")		,LEX_DstBlend_ID),

	script::TokenDefine(mT("Zero")			,LEX_Zero_ID),
	script::TokenDefine(mT("One")			,LEX_One_ID),
	script::TokenDefine(mT("SrcAlpha")		,LEX_SrcAlpha_ID),
	script::TokenDefine(mT("InvSrcAlpha")	,LEX_InvSrcAlpha_ID),
	script::TokenDefine(mT("DstAlpha")		,LEX_DstAlpha_ID),
	script::TokenDefine(mT("InvDstAlpha")	,LEX_InvDstAlpha_ID),
	script::TokenDefine(mT("SrcColor")		,LEX_SrcColor_ID),
	script::TokenDefine(mT("InvSrcColor")	,LEX_InvSrcColor_ID),
	script::TokenDefine(mT("DstColor")		,LEX_DstColor_ID),
	script::TokenDefine(mT("InvDstColor")	,LEX_InvDstColor_ID),
	script::TokenDefine(mT("AlphaSaturate")	,LEX_AlphaSaturate_ID),

	script::TokenDefine(mT("cullfacedir")	,LEX_CullFaceDir_ID),
	script::TokenDefine(mT("cw")	,LEX_CW_ID),
	script::TokenDefine(mT("ccw")	,LEX_CCW_ID),

	script::TokenDefine(mT("{")				,LEX_OpenBrace_ID),
	script::TokenDefine(mT("}")				,LEX_ClosedBrace_ID),
	script::TokenDefine(mT("[")				,LEX_OpenBraket_ID),
	script::TokenDefine(mT("]")				,LEX_ClosedBraket_ID),


	script::TokenDefine::EndDefineToken
};

MaterialScript::MaterialScript()
:m_tokenList(0)
{
	m_lex=new script::ILexical();
	m_lex->setTokenDefinitionTable(tokenDefintion,false);
	m_lex->setDelimChars(mT(" \t\r\n[]{}:"));
}

MaterialScript::~MaterialScript()
{
//	delete m_lex;
//	delete m_tokenList;
	m_lex=0;
	m_tokenList=0;
}
int MaterialScript::loadMaterialScript(OS::IStream* stream){
	
	m_lex->loadFromStream(stream);
	script::CTokenTranslator trans;

//	delete m_tokenList;

	m_tokenList=new script::CTokenList();
	m_tokenList->fromLexical(m_lex,&trans,true);
	GCPtr<video::RenderMaterial> mtrl;

	script::ScriptToken* t=0;

	if(!m_tokenList->isDone()){
		bool res=0;
		do{
			t=nextToken();
			if(!t)break;

			if(t->token==LEX_Material_ID){
				nextTokenFromList(t);
				core::string matName;
				matName=t->lex;
				mtrl=0;
				if(lookAhead(mT(":")))//is it inhereted from another material?
				{
					t=nextToken();//ignore the ":"
					nextTokenFromList(t);
					if(t->type!=EST_Ident){
						gLogManager.log(mT("Expected Material Name!"),ELL_ERROR);
					}
					mtrl=gMaterialResourceManager.getMaterial(t->lex);
					if(!mtrl){
						gLogManager.log(mT("Cann't find material: ")+t->lex,ELL_WARNING);
					}else{
						mtrl=mtrl->Duplicate();
					}
				}
				if(!mtrl){
					mtrl=new RenderMaterial();
				}
				mtrl->SetName(matName);
				res=parseMaterial(mtrl);
				if(res)
					gMaterialResourceManager.addResource(mtrl,mtrl->GetName());
			}else if(t->token==LEX_Shader_ID){
				parseShader();
				res=1;
			}
		}while(res);
	}

	return 0;
}
void MaterialScript::addDefinitions(script::TokenDefine*definitions){
	m_lex->setTokenDefinitionTable(tokenDefintion,false);
}

bool MaterialScript::visitInPass(script::ScriptToken*token,video::RenderPass*mtrl){
	for(int i=0;i<m_callback.size();++i){
		if(m_callback[i]->visitInPass(token,mtrl))
			return true;
	}
	return false;
}
bool MaterialScript::visitInTexture(script::ScriptToken*token,const ITexturePtr& tex){
	for(int i=0;i<m_callback.size();++i){
		if(m_callback[i]->visitInTexture(token,tex))
			return true;
	}
	return false;
}
script::ScriptToken* MaterialScript::nextToken(){

	if(m_tokenList->isDone()){
		return 0;
	}
	script::ScriptToken* token=m_tokenList->getNextToken();
	if(!token || token->token==script::ERT_EOF)
		return 0;
	return token;
}


script::ScriptToken* MaterialScript::peekToken(){

	if(m_tokenList->isDone()){
		return 0;
	}
	script::ScriptToken* token=m_tokenList->getToken(1);
	if(!token || token->token==script::ERT_EOF)
		return 0;
	return token;
}
void MaterialScript::addCallback(GCPtr<IMaterialScriptCallback> callback){
}


bool MaterialScript::lookAhead(const mchar*string){
	script::ScriptToken* token=peekToken();
	if(token && token->lex.equals_ignore_case(string))
		return true;
	return false;
}

// shader => "shader".ID.type."{".("shaderVF".path | "shaderFP".path )*."}"
GCPtr<video::GPUShader> MaterialScript::parseShader(){

	script::ScriptToken* token;
	core::string type;
	core::string name;
	nextTokenFromList(token);
	name=token->lex;

	nextTokenFromList(token);
	if(token->lex!=mT("{"))
	{
		type=token->lex;

		nextTokenAndChkLex(token,mT("{"),mT("Expected \"{\""));
	}else{
		type=mT("cg");
	}
	core::string vpPath;
	core::string fpPath;

	bool vp=false,
		fp=false;
	while(1){
		if(lookAhead(mT("}")))
			break;
		nextTokenFromList(token);
		switch(token->token){
		case LEX_ShaderVP_ID:
			{
				nextTokenFromList(token);
				vpPath=token->lex;
				vp=true;
			}break;
		case LEX_ShaderFP_ID:
			{
				nextTokenFromList(token);
				fpPath=token->lex;
				fp=true;
			}break;
		}

	}
	nextTokenAndChkLex(token,mT("}"),mT("Expected \"}\""));
// 
// 	core::stringc vpPath_s,fpPath_s;
// 	core::string_to_char(vpPath,vpPath_s);
// 	core::string_to_char(fpPath,fpPath_s);

	GCPtr<video::GPUShader> shdr=new video::GPUShader();

	if(vp&&fp){
		shdr->SetVertexShader(gShaderResourceManager.loadShaderFromFile(vpPath, video::EShader_VertexProgram, "main_vp", ShaderPredefList()));
		shdr->SetFragmentShader(gShaderResourceManager.loadShaderFromFile(fpPath, video::EShader_FragmentProgram, "main_fp", ShaderPredefList()));
	}
	return shdr;
}

//Mtrl => "material".ID."{".technique*."}"	-- not supported..
//Mtrl => "material".ID."{".attributes*."}"
bool MaterialScript::parseMaterial(video::RenderMaterial* mtrl){
	script::ScriptToken* token;
	//nextTokenAndChkID(token,LEX_Material_ID,mT("Expected \"material\""));

	nextTokenAndChkLex(token,mT("{"),mT("Expected \"{\""));
	video::RenderTechnique* t=0;
	do
	{

		token=nextToken();
		if(!token)break;
		if(token->token==LEX_Technique_ID){
			t=parseTechnique();
			if(t)
				mtrl->AddTechnique(t);
		}
		if(lookAhead(mT("}")))
			break;
	}while(t);
	nextTokenAndChkLex(token,mT("}"),mT("Expected \"}\""));
	return true;
}
//Technique => "technique"."{".Pass*."}"
video::RenderTechnique* MaterialScript::parseTechnique(){
	RenderTechnique* t=new RenderTechnique(0);

	ScriptToken* token=0;

	if(!lookAhead(mT("{")))
	{
		nextTokenFromList(token);
		t->SetName(token->lex);
	}

	nextTokenAndChkLex(token,mT("{"),mT("Expected \"{\""));
	video::RenderPass* p=0;
	do
	{

		token=nextToken();
		if(token->token==LEX_Pass_ID){
			p=parsePass();
			if(p)
				t->AddPass(p);
		}

		if(lookAhead(mT("}")))
			break;
	}while(p);
	nextTokenAndChkLex(token,mT("}"),mT("Expected \"}\""));

	return t;
}
//Pass => "pass"."{".attributes*."}"
video::RenderPass* MaterialScript::parsePass(){

	RenderPass* pass=new RenderPass(0);

	ScriptToken* token=0;

	if(!lookAhead(mT("{")))
	{
		nextTokenFromList(token);
		pass->SetName(token->lex);
	}

	nextTokenAndChkLex(token,mT("{"),mT("Expected \"{\""));
	while(parseAttribute(pass));
	nextTokenAndChkLex(token,mT("}"),mT("Expected \"}\""));

	return pass;
}

// texture_unit => "texture_unit".path.[type]."{".("filter".Filter | "tex_addr_mode".TexAddrMode )*."}"
//	type : 1d,2d,3d,cube
GCPtr<video::TextureUnit> MaterialScript::parseTexture(){

	script::ScriptToken* token;
	video::ITexturePtr  tex;
	GCPtr<video::TextureUnit> texUnit=new video::TextureUnit();
	core::string path;
	nextTokenFromList(token);
	if(token->type!=script::EST_String){
		return false;
	}
	path=token->lex;
	nextTokenFromList(token);
	ETextureType texType;
	if(token->lex!=mT("{"))
	{
		switch(token->token){
			case LEX_Tex1D_ID:
				tex=gTextureResourceManager.loadTexture1D(path,true);
				texType=ETT_1DTex;
				break;
			case LEX_Tex2D_ID:
				tex=gTextureResourceManager.loadTexture2D(path,true);
				texType=ETT_2DTex;
				break;
			case LEX_Tex3D_ID:
				tex=gTextureResourceManager.loadTexture3D(path,true);
				texType=ETT_3DTex;
				break;
			case LEX_TexCube_ID:
				{
					core::string base,ext;
					core::StringUtil::SplitPathExt(path,base,ext);
					std::vector<core::string> paths;
					paths.push_back(base+"+x."+ext);
					paths.push_back(base+"-x."+ext);
					paths.push_back(base+"+y."+ext);
					paths.push_back(base+"-y."+ext);
					paths.push_back(base+"+z."+ext);
					paths.push_back(base+"-z."+ext);
					tex=gTextureResourceManager.loadTextureCube(path,&paths[0],true);
					texType=ETT_CubeTex;
				}
				break;
		}

		nextTokenAndChkLex(token,mT("{"),mT("Expected \"{\""));
	}else{
		tex=gTextureResourceManager.loadTexture2D(path);
	}
	
	texUnit->SetTexture(tex);

	while(parseTextureAttr(texUnit));
	nextTokenAndChkLex(token,mT("}"),mT("Expected \"}\""));

	return texUnit;
}

bool MaterialScript::parseTextureAttr(GCPtr<TextureUnit> tex){
	if(lookAhead(mT("}")))
		return false;
	script::ScriptToken* token;
	nextTokenFromList(token);
	video::ETextureClamp tmpA;
	video::ETextureGenMode tmpM;
	bool tmpB;
	switch(token->token){
		case LEX_WrapS_ID:
			if(parseAddressMode(tmpA) ){
				if(tex)tex->setTextureClamp(video::ETW_WrapS,tmpA);
			}
			else return 0;
			break;
		case LEX_WrapT_ID:
			if(parseAddressMode(tmpA) && tex){
				tex->setTextureClamp(video::ETW_WrapT,tmpA);
			}
			else return 0;
			break;
		case LEX_WrapR_ID:
			if(parseAddressMode(tmpA)){
				if(tex)tex->setTextureClamp(video::ETW_WrapR,tmpA);
			}
			else return 0;
			break;

		case LEX_GenS_ID:
			if(parseGenMode(tmpM)){
				if(tex)tex->setTextureGenMode(video::ETGT_GenS,tmpM);
			}
			else return 0;
			break;
		case LEX_GenT_ID:
			if(parseGenMode(tmpM)){
				if(tex)tex->setTextureGenMode(video::ETGT_GenT,tmpM);
			}
			break;
		case LEX_GenR_ID:
			if(parseGenMode(tmpM) ){
				if(tex)tex->setTextureGenMode(video::ETGT_GenR,tmpM);
			}
			else return 0;
			break;
		case LEX_GenQ_ID:
			if(parseGenMode(tmpM) ){
				if(tex)tex->setTextureGenMode(video::ETGT_GenQ,tmpM);
			}
			else return 0;
			break;

		case LEX_EnableGenS_ID:
			if(parseBool(tmpB)  ){
				if(tex)tex->enableGenMode(video::ETGT_GenS,tmpB);
			}
			else return 0;
			break;
		case LEX_EnableGenT_ID:
			if(parseBool(tmpB)  ){
				if(tex)tex->enableGenMode(video::ETGT_GenT,tmpB);
			}
			else return 0;
			break;
		case LEX_EnableGenR_ID:
			if(parseBool(tmpB)  ){
				if(tex)tex->enableGenMode(video::ETGT_GenR,tmpB);
			}
			else return 0;
			break;
		case LEX_EnableGenQ_ID:
			if(parseBool(tmpB)  ){
				if(tex)tex->enableGenMode(video::ETGT_GenQ,tmpB);
			}
			else return 0;
			break;

		case LEX_Filtering_ID:
		{
			nextTokenFromList(token);
			switch(token->token){
			case LEX_Bilinear_ID:
				if(tex->GetTexture())
					tex->GetTexture()->setBilinearFilter(true);
				break;
			case LEX_Trilinear_ID:
				if(tex->GetTexture())
					tex->GetTexture()->setTrilinearFilter(true);
				break;
			}
			return true;
		}

		default:
			return visitInTexture(token,tex);
	}

	return true;
}

bool MaterialScript::parseAttribute(video::RenderPass* mtrl){
	if(lookAhead(mT("}")))
		return false;
	script::ScriptToken* token;
	nextTokenFromList(token);
	bool isValue=false;
	EValueType valueType;
	core::string valueName;
	if(token->token==LEX_Value_ID){
		isValue=true;
		nextTokenFromList(token);
		if(token->token==LEX_OpenBraket_ID){
			nextTokenFromList(token);
			if(token->type!=EST_String){
				gLogManager.log(mT("Value Name Expected!"),ELL_WARNING);
			}
			valueName=token->lex;
			nextTokenAndChkID(token,LEX_ClosedBraket_ID,mT("']' expected!"));
			nextTokenFromList(token);

			if(token->lex==mT("int"))valueType=EVT_INT;
			else if(token->lex==mT("float"))valueType=EVT_FLOAT;
			else if(token->lex==mT("color"))valueType=EVT_COLOR;
			else if(token->lex==mT("float"))valueType=EVT_FLOAT;
			else if(token->lex==mT("int2"))valueType=EVT_VECTOR2Di;
			else if(token->lex==mT("int3"))valueType=EVT_VECTOR3Di;
			else if(token->lex==mT("int4"))valueType=EVT_VECTOR4Di;
			else if(token->lex==mT("float2"))valueType=EVT_VECTOR2Df;
			else if(token->lex==mT("float3"))valueType=EVT_VECTOR3Df;
			else if(token->lex==mT("float4"))valueType=EVT_VECTOR4Df;
			else if(token->lex==mT("texture"))valueType=EVT_TEXTURE;
			else if(token->lex==mT("shader"))valueType=EVT_SHADER;
		}
	}
	float tmpf;
	bool res;
	video::SColor tmpClr;
	video::EState tmpS;
	switch(token->token){
		case LEX_Texture_ID:
		{
			//parse texture unit id
			int id=0;
			if(!isValue){
				nextTokenFromList(token);
				id=core::StringConverter::toInt(token->lex);
			}
			GCPtr<video::TextureUnit> tex= parseTexture();
			if(!isValue){
				mtrl->setTextureUnit(tex,id);
			}else{
				mtrl->addValue(new TextureValue(valueName,tex));
			}
		}break;
		case LEX_CullFaceDir_ID:
			{
				nextTokenFromList(token);
				if(token->token==LEX_CW_ID)
					mtrl->SetCullFaceDirection(video::ECFM_CW);
				else
					mtrl->SetCullFaceDirection(video::ECFM_CCW);
			}
			break;
		case LEX_SrcBlend_ID:
			{
				video::EBlendFactor f;
				res=parseBlendOp(f);

				if(res)
					mtrl->SetSourceBlendFactor(f);
				return res;
			}
		case LEX_DstBlend_ID:
			{
				video::EBlendFactor f;
				res=parseBlendOp(f);

				if(res)
					mtrl->SetDestinationBlendFactor(f);
				return res;
			}

		case LEX_BlendRenderer_ID:
			mtrl->setMaterialRenderer(parseRenderType());
			return true;

		case LEX_AlphaFunc_ID:
			{
				video::EAlphaFunc f;
				res=parseAlphaFunc(f);
				if(res)
					mtrl->SetAlphaFunction(f);
				return res;
			}
		case LEX_AlphaRef_ID:
			{

				res=parseFloat(tmpf);
				if(res)mtrl->SetAlphaReferenceValue(res);
			}
			return true;

		case LEX_Shader_ID:
			{
				mtrl->setRenderShader(parseShader());
				return true;
			}
		case LEX_ShaderRef_ID:
			{
				// ShaderRef => "ShaderRef" . ID
				nextTokenFromList(token);
				mtrl->setRenderShader(gShaderResourceManager.getResource(token->lex));
				return true;
			}
		case LEX_Ambient_ID:
			res=parseColor(tmpClr);
			if(res)mtrl->SetAmbient(tmpClr);
			return res;
		case LEX_Diffuse_ID:
			res=parseColor(tmpClr);
			if(res)mtrl->SetDiffuse(tmpClr);
			return res;
		case LEX_Specular_ID:
			res=parseColor(tmpClr);
			if(res)mtrl->SetSpecular(tmpClr);
			return res;
		case LEX_Emissive_ID:
			res=parseColor(tmpClr);
			if(res)mtrl->SetEmissive(tmpClr);
			return res;
		case LEX_Shininess_ID:
			res=parseFloat(tmpf);
			if(res)mtrl->SetShininess(tmpf);
			return res;
		case LEX_Alpha_ID:
			res=parseFloat(tmpf);
			if(res)mtrl->SetAlpha(tmpf);
			return res;
		case LEX_Lighting_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_Lighting,tmpS);
			return true;
		case LEX_ZWrite_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_ZWrite,tmpS);
			return true;
		case LEX_BlendEnable_ID:
			if(parseState(tmpS))
				mtrl->setRenderState(RS_Blend,tmpS);
			return true;
		case LEX_AlphaTestEnable_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_AlphaTest,tmpS);
			return true;
		case LEX_Wireframe_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_Wireframe,tmpS);
			return true;
		case LEX_Points_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_Points,tmpS);
			return true;
		case LEX_CullFace_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_CullFace,tmpS);
			return true;
		case LEX_Fog_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_Fog,tmpS);
			return true;
		case LEX_Smooth_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_Smooth,tmpS);
			return true;
		case LEX_ZTest_ID:
			parseState(tmpS);
			mtrl->setRenderState(RS_ZTest,tmpS);
			return true;

		default:
			if(isValue)
			{
				IValue*val=0;
				switch(valueType){
				case EVT_BOOLEAN:
					{
						bool v;
						parseBool(v);
						val=new BoolValue(valueName,v);
					}break;
				case EVT_INT:
					{
						nextTokenFromList(token);
						int v;
						core::StringConverter::parse(token->lex,v);
						val=new IntValue(valueName,v);
					}break;
				case EVT_FLOAT:
					{
						nextTokenFromList(token);
						float v;
						core::StringConverter::parse(token->lex,v);
						val=new FloatValue(valueName,v);
					}break;
				case EVT_COLOR:
					{
						nextTokenFromList(token);
						video::SColor v;
						core::StringConverter::parse(token->lex,v);
						val=new ColorValue(valueName,v);
					}break;
				case EVT_VECTOR2Di:
					{
						nextTokenFromList(token);
						math::vector2di v;
						core::StringConverter::parse(token->lex,v);
						val=new Vector2diValue(valueName,v);
					}break;
				case EVT_VECTOR3Di:
					{
						nextTokenFromList(token);
						math::vector3di v;
						core::StringConverter::parse(token->lex,v);
						val=new Vector3diValue(valueName,v);
					}break;
				case EVT_VECTOR4Di:
					{
						nextTokenFromList(token);
						math::vector4di v;
						core::StringConverter::parse(token->lex,v);
						val=new Vector4diValue(valueName,v);
					}break;
				case EVT_VECTOR2Df:
					{
						nextTokenFromList(token);
						math::vector2df v;
						core::StringConverter::parse(token->lex,v);
						val=new Vector2dfValue(valueName,v);
					}break;
				case EVT_VECTOR3Df:
					{
						nextTokenFromList(token);
						math::vector3df v;
						core::StringConverter::parse(token->lex,v);
						val=new Vector3dfValue(valueName,v);
					}break;
				case EVT_VECTOR4Df:
					{
						nextTokenFromList(token);
						math::vector4df v;
						core::StringConverter::parse(token->lex,v);
						val=new Vector4dfValue(valueName,v);
					}break;
				case EVT_SHADER:
					{
						nextTokenFromList(token);
						val=new ShaderValue(valueName,gShaderResourceManager.getResource(token->lex));
					}break;
				}
				if(val){
					mtrl->addValue(val);
					return true;
				}
			}
			return visitInPass(token,mtrl);
	}

	return true;
}
bool MaterialScript::parseColor(SColor&c){
	script::ScriptToken* token;
	nextTokenFromList(token);
	if(token->type==script::EST_String){
		core::StringConverter::parse(token->lex,c);
		return true;
	}
	return false;
}
bool MaterialScript::parseFloat(float&c){
	script::ScriptToken* token;
	nextTokenFromList(token);
	if(token->type==script::EST_Int || token->type==script::EST_Float){
		core::StringConverter::parse(token->lex,c);
		return true;
	}
	return false;

}
bool MaterialScript::parseBool(bool&c){
	script::ScriptToken* token;
	nextTokenFromList(token);
	core::StringConverter::parse(token->lex,c);
	return true;
}

bool MaterialScript::parseBlendOp(video::EBlendFactor&op){
	script::ScriptToken* token;
	nextTokenFromList(token);

	switch (token->token)
	{
	case LEX_Zero_ID:
		op=video::EBF_ZERO;
		break;
	case LEX_One_ID:
		op=video::EBF_ONE;
		break;
	case LEX_SrcAlpha_ID:
		op=video::EBF_SRC_ALPHA;
		break;
	case LEX_InvSrcAlpha_ID:
		op=video::EBF_ONE_MINUS_SRC_ALPHA;
		break;
	case LEX_DstAlpha_ID:
		op=video::EBF_DST_ALPHA;
		break;
	case LEX_InvDstAlpha_ID:
		op=video::EBF_ONE_MINUS_DST_ALPHA;
		break;
	case LEX_SrcColor_ID:
		op=video::EBF_SRC_COLOR;
		break;
	case LEX_InvSrcColor_ID:
		op=video::EBF_ONE_MINUS_SRC_COLOR;
		break;
	case LEX_DstColor_ID:
		op=video::EBF_DST_COLOR;
		break;
	case LEX_InvDstColor_ID:
		op=video::EBF_ONE_MINUS_DST_COLOR;
		break;
	case LEX_AlphaSaturate_ID:
		op=video::EBF_SRC_ALPHA_SATURATE;
		break;
	default:
		return false;
	}
	return true;
}
bool MaterialScript::parseState(video::EState&c){
	script::ScriptToken* token;
	nextTokenFromList(token);

	switch (token->token)
	{
	case LEX_Use_ID:
		c=video::ES_Use;
		break;
	case LEX_DontUse_ID:
		c=video::ES_DontUse;
		break;
	case LEX_Unkwon_ID:
		c=video::ES_Unknown;
		break;
	default:
		return 0;
	}
	return 1;

}
bool MaterialScript::parseAddressMode(video::ETextureClamp&c){

	script::ScriptToken* token;
	nextTokenFromList(token);
	switch (token->token)
	{
		case LEX_Clamp_ID:
			c=video::ETC_CLAMP;
			break;
		case LEX_Mirror_ID:
			c=video::ETC_MIRROR;
			break;
		case LEX_Repeat_ID:
			c=video::ETC_REPEAT;
			break;
		case LEX_Clamp_To_Edge_ID:
			c=video::ETC_CLAMP_TO_EDGE;
			break;
		default:
			return 0;
	}
	return true;
}

bool MaterialScript::parseGenMode(video::ETextureGenMode&c){

	script::ScriptToken* token;
	nextTokenFromList(token);
	switch (token->token)
	{
	case LEX_EyeLinear_ID:
		c=video::ETGM_EyeLinear;
		break;
	case LEX_ObjectLinear_ID:
		c=video::ETGM_ObjectLinear;
		break;
	case LEX_SphereMap_ID:
		c=video::ETGM_SphereMap;
		break;
	default:
		return 0;
	}
	return true;
}

bool MaterialScript::parseAlphaFunc(video::EAlphaFunc&c){

	script::ScriptToken* token;
	nextTokenFromList(token);
	switch (token->token)
	{
	case LEX_Never_ID:
		c=video::EAF_Never;
		break;
	case LEX_Equal_ID:
		c=video::EAF_Equal;
		break;
	case LEX_Greater_ID:
		c=video::EAF_Greater;
		break;
	case LEX_NotEqual_ID:
		c=video::EAF_NotEqual;
		break;
	case LEX_GreaterEqual_ID:
		c=video::EAF_GreaterEqual;
		break;
	case LEX_Always_ID:
		c=video::EAF_Always;
		break;
	default:
		return false;
	}
	return true;
}

video::EMaterialRenderType MaterialScript::parseRenderType(){

	script::ScriptToken* token;
	token=nextToken();
	if(!token)
		return video::MRT_SOLID;
	switch (token->token)
	{
		case LEX_Solid_ID:
			return video::MRT_SOLID;
		case LEX_Add_ID:
			return video::MRT_ADD;
		case LEX_AddAlpha_ID:
			return video::MRT_ADDALPHA;
		case LEX_Transparent_ID:
			return video::MRT_TRANSPARENT;
		case LEX_Mul_ID:
			return video::MRT_MUL;
	}
	return video::MRT_SOLID;
}

}
}

