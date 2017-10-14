#include "stdafx.h"


#include <gl/glew.h>
#include "SGLCGShaderProgram.h"
#include "GLDev.h"
#include "GLTexture.h"
//#include "GLTexture1D.h"
#include <IFileSystem.h>
//#include "GLTexture.h"
//#include "GLTextureCube.h"
#include <TraceManager.h>

#include <StringConverter.h>

#include <DeviceCapabilites.h>
#include <VideoLoggerSystem.h>
#include <Engine.h>

#include <LogManager.h>
#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>

namespace mray{
namespace video{

CGprofile SGLCGShaderProgram::s_CgVertexProfile;
CGprofile SGLCGShaderProgram::s_CgFragmentProfile;
CGprofile SGLCGShaderProgram::s_CgGeometryProfile;

bool cgInitialized=false;

std::vector<float> SGLCGShaderProgram::m_matrixArray;

static const core::string s_SGLCGShaderProgram_type=mT("cg");


SGLCGShaderProgram::SGLCGShaderProgram(EShaderProgramType type, CGcontext context)
	:IGPUShaderProgram(type)
{
	traceFunction(eVideo);
	m_context = context;
	m_loaded = false;
	if (!cgInitialized){
		cgInitialized = true;

		if (checkForError(mT("creating m_context"))){
			return;
		}
		s_CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		cgGLSetOptimalOptions(s_CgVertexProfile);
		if (checkForError(mT("selecting vertex profile"))){
			//return;
		}

		s_CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		cgGLSetOptimalOptions(s_CgFragmentProfile);
		if (checkForError(mT("selecting fragment profile"))){
			//return;
		}

		s_CgGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
		cgGLSetOptimalOptions(s_CgGeometryProfile);
		if (checkForError(mT("selecting Geometry profile"))){
			//return;
		}


#ifdef _DEBUG
		cgGLSetDebugMode(true);
#else
		cgGLSetDebugMode(false);
#endif
	}
	//cgGLSetManageTextureParameters(m_context, CG_TRUE);

	cgGetError();

	if (m_type == EShader_VertexProgram)
		m_CgProfile = s_CgVertexProfile;
	else if (m_type == EShader_FragmentProgram)
		m_CgProfile = s_CgFragmentProfile;
	else if (m_type == EShader_GeometryProgram)
		m_CgProfile = s_CgGeometryProfile;
	else
	{
		gLogManager.log(mT("Shader Type not supported"), ELL_WARNING);
		return;
	}
	textures.resize(gEngine.getDevice()->getCapabilities()->getMaxTextureUnits());

}



bool SGLCGShaderProgram::LoadFromPath(const core::string&path, const char*entryPoint, const std::vector<core::string>& predef)
{


	m_entryPoint = entryPoint;

	const char** pargs = cgGLGetOptimalOptions(m_CgProfile);

	const char* compilerArguments[] = { 0 };// {"-po", "ATI_draw_buffers", 0}; //,"-profileopts""dcls","-DTEST_ARG2=1"

	core::string prog;
	core::stringc progPath;
	m_Program = path;
	gFileSystem.getCorrectFilePath(m_Program, prog);
	core::string_to_char(prog.c_str(), progPath);
	if (progPath == "")
	{
		gLogManager.log(mT("Cann't find shader program: ") + prog, ELL_WARNING);
	}
	m_CgProgram =
		cgCreateProgramFromFile(
		m_context,              /* Cg runtime m_context */
		CG_SOURCE,                /* Program in human-readable form */
		progPath.c_str(),
		m_CgProfile,        /* Profile */
		entryPoint,				/* Entry function name */
		compilerArguments);                    /* compiler options */

	if (!_linkProgram())
	{
		unloadInternal();
		return false;
	}
	m_loaded = true;
	return true;
}
bool SGLCGShaderProgram::LoadShader(const core::string&program, const char*entryPoint, const std::vector<core::string>& predef)
{


	m_entryPoint = entryPoint;

	const char** pargs = cgGLGetOptimalOptions(m_CgProfile);

	const char* compilerArguments[] = { 0 };// {"-po", "ATI_draw_buffers", 0}; //,"-profileopts""dcls","-DTEST_ARG2=1"


	core::stringc prog;
	core::string_to_char(program.c_str(), prog);
	m_CgProgram =
		cgCreateProgram(
		m_context,              /* Cg runtime m_context */
		CG_SOURCE,                /* Program in human-readable form */
		prog.c_str(),
		m_CgProfile,        /* Profile */
		entryPoint,				/* Entry function name */
		compilerArguments);                    /*  compiler options */


	if (!_linkProgram())
	{
		unloadInternal();
		return false;
	}
	m_loaded = true;
	return true;
}

bool SGLCGShaderProgram::_linkProgram()
{
	if (checkForError(mT("creating shader program from file"))){
		const char* prog=cgGetProgramString(m_CgProgram, CG_COMPILED_PROGRAM);
		core::string progStr;
		core::char_to_string(prog,progStr);
		int pos;
		glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &pos);
		gLogManager.log(progStr,ELL_INFO);
		gLogManager.log(mT("Position=")+core::StringConverter::toString(pos),ELL_INFO);
		return false;
	}
  
	cgGLLoadProgram(m_CgProgram);
	if(checkForError(mT("loading shader program")))
		return false;

	loadUniforms();
	if(checkForError(mT("loading Uniforms")))
		return false;

	
	return true;
}

SGLCGShaderProgram::~SGLCGShaderProgram(){

	traceFunction(eVideo);
//	cgDestroyProgram(m_CgVertexProgram);
	if(checkForError(mT("SGLCGShaderProgram::~SGLCGShaderProgram()-cgDestroyProgram()")))return;

	UniformList::iterator it=m_Uniforms.begin();
	for(;it!=m_Uniforms.end();++it)
		delete it->second;
	m_Uniforms.clear();

	textures.clear();
}
uint SGLCGShaderProgram::calcSizeInternal(){
	uint usize=sizeof(CGUniform)*(m_Uniforms.size());
	return usize+sizeof(SGLCGShaderProgram);
}

const core::string& SGLCGShaderProgram::getShaderType(){return s_SGLCGShaderProgram_type;}

bool SGLCGShaderProgram::hasError()
{
	return cgGetError()!=CG_NO_ERROR;

}
bool SGLCGShaderProgram::checkForError(const mchar*msg){
//	traceFunction(eVideo);
	CGerror error=cgGetError();;


	if (error != CG_NO_ERROR) {
		const char *str = cgGetErrorString(error);
		core::string errorMsg;
		errorMsg=getName()+mT(":");
		errorMsg+=msg;
		errorMsg+=mT("[ CG error] :"); 
		errorMsg+=core::StringConverter::toString(str);
		if (error == CG_COMPILER_ERROR) {
			errorMsg+=mT(" - Compiler error=\n");
			errorMsg+=core::StringConverter::toString(cgGetLastListing(m_context));
		}
		//FATAL_ERROR(1,errorMsg.c_str())
		//gVideoLoggerSystem.log(errorMsg.c_str(),ELL_WARNING);
		return 1;
	}
	return 0;
}

EUniformType SGLCGShaderProgram::cgToUniform(CGtype type,size_t&outSize){
	switch(type){
		case CG_INT:
		case CG_INT1:
			outSize=1;
			return EUT_Int1;
		case CG_INT2:
			outSize=2;
			return EUT_Int2;
		case CG_INT3:
			outSize=3;
			return EUT_Int3;
		case CG_INT4:
			outSize=4;
			return EUT_Int4;
		case CG_FLOAT:
		case CG_FLOAT1:
		case CG_HALF:
		case CG_HALF1:
			outSize=1;
			return EUT_Float1;
		case CG_FLOAT2:
		case CG_HALF2:
			outSize=2;
			return EUT_Float2;
		case CG_FLOAT3:
		case CG_HALF3:
			outSize=3;
			return EUT_Float3;
		case CG_FLOAT4:
		case CG_HALF4:
			outSize=4;
			return EUT_Float4;
		case CG_FLOAT2x2:
		case CG_HALF2x2:
			outSize=4;
			return EUT_Matrix2x2;
		case CG_FLOAT2x3:
		case CG_HALF2x3:
			outSize=6;
			return EUT_Matrix2x3;
		case CG_FLOAT2x4:
		case CG_HALF2x4:
			outSize=8;
			return EUT_Matrix2x4;
		case CG_FLOAT3x3:
		case CG_HALF3x3:
			outSize=9;
			return EUT_Matrix3x3;
		case CG_FLOAT3x4:
		case CG_HALF3x4:
			outSize=12;
			return EUT_Matrix3x4;
		case CG_FLOAT4x2:
		case CG_HALF4x2:
			outSize=8;
			return EUT_Matrix4x2;
		case CG_FLOAT4x3:
		case CG_HALF4x3:
			outSize=12;
			return EUT_Matrix4x3;
		case CG_FLOAT4x4:
		case CG_HALF4x4:
			outSize=16;
			return EUT_Matrix4x4;
		case CG_SAMPLER1D:
			return EUT_Sampler1D;
		case CG_SAMPLER2D:
			return EUT_Sampler2D;
		case CG_SAMPLER3D:
			return EUT_Sampler3D;
		case CG_SAMPLERCUBE:
			return EUT_SamplerCube;
		case CG_ARRAY:
			return EUT_Array;
		default:
			return EUT_Unkown;
	}
}

void SGLCGShaderProgram::fillUniformArray(CGparameter param,UniformList*arr,size_t m_contextArraySize){
	traceFunction(eVideo);
	CGUniform* uniform;
	while(param){
		CGtype type=cgGetParameterType(param);
		if(cgGetParameterVariability(param)==CG_UNIFORM
			&& cgGetParameterDirection(param) != CG_OUT){

			switch(type){
				case CG_STRUCT:
				{
					uniform=new CGUniform();
					uniform->param=param;
					uniform->name=cgGetParameterName(param);
					uniform->setSemanticByString(uniform->name);
					arr->insert(UniformList::value_type(uniform->name,uniform));

					fillUniformArray(cgGetFirstStructParameter(param),arr);
				}break;
				case CG_ARRAY:
				{

					uniform=new CGUniform();
					uniform->param=param;
					uniform->name=cgGetParameterName(param);
					uniform->setSemanticByString(uniform->name);
					uniform->type=cgToUniform(type,uniform->elemSize);

					arr->insert(UniformList::value_type(uniform->name,uniform));

					uniform->arraySize=cgGetArraySize(param,0);
/*					for(int i{}i<arrSize;++i)
						fillUniformArray(cgGetArrayParameter(param,i),arr,arrSize);
*/
				}break;
				default:
				{
					uniform=new CGUniform();
					uniform->param=param;
					uniform->name=cgGetParameterName(param);
					uniform->setSemanticByString(uniform->name);
					uniform->startIndex=cgGetParameterResourceIndex(param);
					//uniform->index=cgGetParameterIndex(param);
					uniform->type=cgToUniform(type,uniform->elemSize);
					uniform->arraySize=m_contextArraySize;
					arr->insert(UniformList::value_type(uniform->name,uniform));

				//	CGresource res=cgGetParameterResource(param);
				}
			}
		}
		param=cgGetNextParameter(param);
	}
	
}
void SGLCGShaderProgram::loadUniforms(){
	traceFunction(eVideo);
	CGparameter param=cgGetFirstParameter(m_CgProgram,CG_PROGRAM );
	fillUniformArray(param,&m_Uniforms);

}
	
void SGLCGShaderProgram::use(){
	traceFunction(eVideo);
	if(!m_loaded)
		return;
	cgGLBindProgram(m_CgProgram);
	//checkForError(mT("binding shader program"));
	
	cgGLEnableProfile(m_CgProfile);
	//checkForError(mT("enabling shader profile"));

	if(hasError())
		cgGLDisableProfile(m_CgProfile);
	/*
	if(m_BaseMtrl)
		m_BaseMtrl->set(mat);
	else{
		for(int i{}i<textures.size();++i)
			getDevice()->useTexture(i,textures[i]);
	}
	if(m_callback)
		m_callback->setConstants(this);*/
}
void SGLCGShaderProgram::unuse(){
	traceFunction(eVideo);
	if(!m_loaded)
		return;

	cgGLBindProgram(m_CgProgram);
	cgGLDisableProfile(m_CgProfile);

	//checkForError(mT("disabling shader profile"));
	/*
	if(m_BaseMtrl)
		m_BaseMtrl->unset();*/
}

void SGLCGShaderProgram::setTexture(const core::string&name,video::TextureUnit*tex){
	traceFunction(eVideo);
	if(!tex || !tex->GetTexture() || tex->GetTexture()->getDeviceType()!=GLDev::m_deviceType)return;
	
	core::stringc nameStr;
	core::string_to_char(name,nameStr);
	UniformListIT it=m_Uniforms.find(nameStr);
	if(it==m_Uniforms.end())return;
	CGUniform* uniform=(CGUniform*)it->second;
	if( uniform->isSampler()==0)return;

	GLTexture* gtex=dynamic_cast<GLTexture*>(tex->GetTexture().pointer());
	int id=gtex->getTextureID();
	if(0)
	{

		int idx=uniform->startIndex;
		if(idx<0)
			idx=0;

		gEngine.getDevice()->useTexture(idx,tex);


		if(glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB+idx);
	}
	cgGLSetTextureParameter(uniform->param,id);
	cgGLEnableTextureParameter(uniform->param);
	//cgGLSetupSampler(uniform->param,id);
	//checkForError(mT("setting texture unit."));
	checkForError(mT("setting texture unit."));
}

void SGLCGShaderProgram::setMatrixArray(GPUUniform*u,const math::matrix4x4* arr,int count){
	traceFunction(eVideo);

	if(u->type!=EUT_Array)
		return;
	CGUniform*uniform=(CGUniform*)u;
	if(!uniform)
		return;
/*
	if(count>MAX_MATRICIES)
		count=MAX_MATRICIES;*/

	m_matrixArray.resize(12*count);

	//int idx{}
	for (int i=0;i<count;i++)
	{
		IOSSystem::memCopy(&m_matrixArray[i*12],arr[i].getMatPointer(),12*sizeof(float));
		/*const math::matrix4x4& m=arr[i];
		for(int j{}j<4;++j)
		{
			for(int k{}k<3;++k)
			{
				m_matrixArray[idx++]=m[k][j];
			}
		}*/
	}
	cgGLSetMatrixParameterArrayfr(uniform->param,0,count,&m_matrixArray[0]);
//	cgGLSetMatrixParameterArrayfc(uniform->param,0,count,m_matrixArray);
	checkForError(mT("setMatrixArray"));
}

void SGLCGShaderProgram::setUniformVal(CGparameter param,EUniformType type,const float*vals,int count){
	traceFunction(eVideo);
	
	switch(type){
		case EUT_Int1:
		case EUT_Float1:
			cgGLSetParameter1fv(param,vals);
			break;
		case EUT_Int2:
		case EUT_Float2:
			cgGLSetParameter2fv(param,vals);
			break;
		case EUT_Int3:
		case EUT_Float3:
			cgGLSetParameter3fv(param,vals);
			break;
		case EUT_Int4:
		case EUT_Float4:
			cgGLSetParameter4fv(param,vals);
			break;
		case EUT_Matrix2x2:
		case EUT_Matrix3x3:
		case EUT_Matrix4x4:
			cgGLSetMatrixParameterfr(param,vals);
			break;
		default:
			return;
	}
	checkForError(mT("setting uniform value"));
}

bool SGLCGShaderProgram::setConstant(GPUUniform*u,const float*vals,int count){
	CGUniform*uniform=(CGUniform*)u;
	if(!uniform)
		return 0;
	setUniformVal(uniform->param,uniform->type,vals,count);
	return 1;
}
bool SGLCGShaderProgram::setConstant(const core::string&name,const float*vals,int count){
	traceFunction(eVideo);
	

	CGparameter param;
	EUniformType utype;
	core::stringc nameStr;
	core::string_to_char(name,nameStr);
	UniformListIT it=m_Uniforms.find(nameStr);

	if(it==m_Uniforms.end()){
		//try to fetch it then
		param= cgGetNamedParameter(m_CgProgram,nameStr.c_str());
		if(!param)	return 0;
		CGUniform* uniform=new CGUniform();
		uniform->param=param;
		uniform->name=cgGetParameterName(param);
		uniform->setSemanticByString(uniform->name);
		uniform->startIndex=cgGetParameterResourceIndex(param);
		//	CGresource res=cgGetParameterResource(param);
		CGtype type= cgGetParameterType(param);
		utype=uniform->type=cgToUniform(type,uniform->elemSize);
		uniform->arraySize=cgGetArraySize(param,0);;
		m_Uniforms.insert(UniformList::value_type(uniform->name,uniform));
	}else{

		CGUniform*uniform=(CGUniform*)(*it).second;

		param=uniform->param;
		utype=uniform->type;
	}

	setUniformVal(param,utype,vals,count);

	return 1;
}


void SGLCGShaderProgram::createArray(const char*name,int length)
{	
	UniformListIT it;
	it=m_Uniforms.find(name);
	if(it==m_Uniforms.end())
		return;

	CGUniform* uniform=(CGUniform*)it->second;

	cgCreateParameterArray(m_context,cgGetParameterType(uniform->param),length);

}
void SGLCGShaderProgram::setArrayElementValue(const char*name,int index,const float*vals,int count)
{
	UniformListIT it;
	it=m_Uniforms.find(name);
	if(it==m_Uniforms.end())
		return;

	CGUniform* uniform=(CGUniform*)it->second;


	size_t sz;
	CGparameter param= cgGetArrayParameter(uniform->param,index);
	setUniformVal(param,cgToUniform(cgGetParameterType(param),sz),vals,count);

}

bool SGLCGShaderProgram::UniformListShared(){
	return false;
}
void SGLCGShaderProgram::setUniformAlias(const core::string&name,const core::string&alias){
	core::stringc nameStr;
	core::string_to_char(name,nameStr);
	UniformList::iterator it=m_Uniforms.find(nameStr);
	if(it!=m_Uniforms.end())
	{
	}
}

int SGLCGShaderProgram::getUniformCount()
{
	return m_Uniforms.size();
}
GPUUniform*  SGLCGShaderProgram::getUniform(const core::string&name)
{
	core::stringc nameStr;
	core::string_to_char(name,nameStr);
	UniformListIT it= m_Uniforms.find(nameStr);
	if(it==m_Uniforms.end())
		return 0;
	return it->second;
}
UniformListIT SGLCGShaderProgram::beginUniforms(){
	return m_Uniforms.begin();
}
UniformListIT SGLCGShaderProgram::endUniforms(){
	return m_Uniforms.end();
}

int SGLCGShaderProgram::getAttributeIndex(EMeshStreamType attribute,int index){
	switch(attribute){
		case EMST_Position:
			return 0;
		case EMST_BlendWeights:
			return 1;
		case EMST_Normal:
			return 2;
		case EMST_Color:
			return 3;
		case EMST_Specular:
			return 4;
		case EMST_BlendIndicies:
			return 7;
		case EMST_Texcoord:
			return 8+index;
		case EMST_Tangent:
			return 14;
		case EMST_Binormal:
			return 15;
	}
	FATAL_ERROR(true,mT("getAttributeIndex(): using unkown Attribute!"));
	return 0;
}
bool SGLCGShaderProgram::isAttributeValid(EMeshStreamType attribute,int index){
	switch(attribute){
		case EMST_Position:
		case EMST_Normal:
		case EMST_Color:
		case EMST_Specular:
		case EMST_Texcoord:
			return true;
		case EMST_Tangent:
		case EMST_Binormal:
		case EMST_BlendWeights:
		case EMST_BlendIndicies:
			return true;
	}
	return false;
}

}
}



