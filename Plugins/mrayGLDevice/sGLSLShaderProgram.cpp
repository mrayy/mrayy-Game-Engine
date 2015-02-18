#include "stdafx.h"

#include "SGLSLShaderProgram.h"
#include <IShaderConstantsCallback.h>
#include <IFileSystem.h>
#include <ILexical.h>
#include <Engine.h>

#include <LogManager.h>
namespace mray{
namespace video{


static const core::string s_SGLSLShaderProgram_type=mT("glsl");
const core::string SGLSLShaderProgram::m_entryPoint=mT("main");


SGLSLShaderProgram::SGLSLShaderProgram(EShaderProgramType type)
	:IGPUShaderProgram(type), hProgram(0)
{
	textures.resize(gEngine.getDevice()->getCapabilities()->getMaxTextureUnits());
	for (int i = 0; i < textures.size(); ++i)
		textures[i] = 0;

}

bool SGLSLShaderProgram::LoadShader(const core::string&program, const char*entryPoint, const std::vector<core::string>& predef)
{

	return init(program.c_str());
}
bool SGLSLShaderProgram::LoadFromPath(const core::string&path, const char*entryPoint, const std::vector<core::string>& predef)
{
	 core::string temp;

	core::stringc prog;
	core::stringc line;
	m_Prog = temp;
	GCPtr<OS::IStream>vfile = gFileSystem.createTextFileReader(temp.c_str());
	if (!vfile){
		gLogManager.log(mT("cann't find shader program :"), temp, ELL_WARNING);
		return false;
	}
	GCPtr<script::ILexical>vparser = new script::ILexical();
	vparser->loadFromStream(vfile);
	while (!vfile->eof()){
		core::string_to_char(vparser->getLine(), line);
		prog += line;
		prog += '\n';
	}

	return LoadShader(prog, entryPoint, predef);
}


SGLSLShaderProgram::~SGLSLShaderProgram(){
	unloadInternal();
}

void SGLSLShaderProgram::unloadInternal()
{
	IResource::unloadInternal();

	if (hProgram){
		glDeleteObjectARB(hProgram);
		hProgram = 0;
	}

	textures.clear();
}

uint SGLSLShaderProgram::calcSizeInternal(){
	uint usize=sizeof(GPUUniform)*uniformsInfo.size();
	return usize+sizeof(SGLSLShaderProgram);
}

bool SGLSLShaderProgram::init(const char*program){
	if(!createProgram())return false;
	
#if defined(GL_ARB_vertex_shader) && defined (GL_ARB_fragment_shader)

	GLuint type;
	if(GetProgramType()==EShader_VertexProgram)
		type=GL_VERTEX_SHADER_ARB;
	else type=GL_FRAGMENT_SHADER_ARB;

	if (!createShader(type, program))
	{
		unloadInternal();
		return false;
	}
	//if(!createShader(GL_FRAGMENT_SHADER_ARB,pixelProgram))return;
#endif

	if (!linkProgram())
	{
		unloadInternal();
		return false;
	}

	return true;
}

const core::string& SGLSLShaderProgram::getShaderType(){return s_SGLSLShaderProgram_type;}

void SGLSLShaderProgram::setTexture(const core::string&name,TextureUnit* tex){
}

	
void SGLSLShaderProgram::use(){
	if(hProgram)
		glUseProgramObjectARB(hProgram);
	/**
	if(baseMtrl)
		baseMtrl->set(mat);
	else{
		for(int i=0;i<textures.size();++i)
			((GLDev*)getDevice())->useTexture(i,textures[i]);
	}
	if(m_callback)
		m_callback->setConstants(this);*/
}

void SGLSLShaderProgram::unuse(){
	glUseProgramObjectARB(0);
	/*
	if(baseMtrl)
		baseMtrl->unset();*/
}


bool SGLSLShaderProgram::createProgram(){
	hProgram=glCreateProgramObjectARB();
	return hProgram!=0;
}

bool SGLSLShaderProgram::createShader(GLenum shaderType,const char*shaderName){
	GLhandleARB shader=glCreateShaderObjectARB(shaderType);

	glShaderSourceARB(shader,1,&shaderName,0);
	glCompileShaderARB(shader);

	int state=0;
#ifdef GL_ARB_shader_objects
	glGetObjectParameterivARB(shader,GL_OBJECT_COMPILE_STATUS_ARB,&state);
#endif
	if(!state){
		gLogManager.log(mT("GLSL shader Compile-Failed"),ELL_WARNING);
		int maxLen=0;
		int len;
#ifdef GL_ARB_shader_objects
	glGetObjectParameterivARB(shader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&maxLen);
#endif
		char*info=new char[maxLen];
		glGetInfoLogARB(shader,maxLen,&len,info);
		gLogManager.log(mT("GLSL Error log"),core::StringConverter::toString(info),ELL_WARNING);
		delete [] info;
		return 0;
	}else
		gLogManager.log(mT("GLSL shader Compile-Done"),ELL_SUCCESS);

	glAttachObjectARB(hProgram,shader);
	return 1;
}

bool SGLSLShaderProgram::linkProgram(){
	glLinkProgramARB(hProgram);
	
	int state=0;
#ifdef GL_ARB_shader_objects
	glGetObjectParameterivARB(hProgram,GL_OBJECT_LINK_STATUS_ARB,&state);
#endif
	if(!state){
		gLogManager.log(mT("GLSL shader Link-Failed"),ELL_WARNING);
		int maxLen=0;
		int len;
#ifdef GL_ARB_shader_objects
	glGetObjectParameterivARB(hProgram,GL_OBJECT_INFO_LOG_LENGTH_ARB,&maxLen);
#endif
		char*info=new char[maxLen];
		glGetInfoLogARB(hProgram,maxLen,&len,info);
		gLogManager.log(mT("GLSL Error log"),core::StringConverter::toString(info),ELL_WARNING);
		delete [] info;
		return 0;
	}else
		gLogManager.log(mT("GLSL shader Link-Done"),ELL_SUCCESS);

	//get uniform
	int num=0;
#ifdef GL_ARB_shader_objects
	glGetObjectParameterivARB(hProgram,GL_OBJECT_ACTIVE_UNIFORMS_ARB,&num);
#endif
	if(!num)return 1;

	int maxLen=0;
#ifdef GL_ARB_shader_objects
	glGetObjectParameterivARB(hProgram,GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB,&maxLen);
#endif
	if(!maxLen){
		gLogManager.log(mT("GLSL-Failed to retrive uniforms information"),ELL_WARNING);
		return 0;
	}

	char*buff=new char[maxLen];
	uniformsInfo.clear();

	for(int i=0;i<num;++i){
		GLSLUniform* u;
		mraySystem::memSet(buff,0,maxLen);

		GLenum type;

		int size=0;
		glGetActiveUniformARB(hProgram,i,maxLen,0,&size,&type,buff);

		u=new GLSLUniform();
		u->type=glslToUniform(type,u->elemSize);

		u->name=buff;
		u->setSemanticByString(u->name);
		u->loc=i;
		uniformsInfo.insert(UniformList::value_type(u->name,u));
	}
	delete [] buff;

	return 1;
}



void SGLSLShaderProgram::setMatrixArray(GPUUniform*u,const math::matrix4x4* arr,int count){
	//glUniformMatrix4fvARB(i,count/16,0,vals);
}

bool SGLSLShaderProgram::setConstant(GPUUniform*u,const float*vals,int count)
{
	GLSLUniform* uf=(GLSLUniform*)u;
	if(!uf)
		return 0;
	int i=uf->loc;
	int type=uf->type;
	switch(type){
		case EUT_Int1:
			glUniform1ivARB(i,count,(const int*)vals);
			break;
		case EUT_Int2:
			glUniform2ivARB(i,count,(const int*)vals);
			break;
		case EUT_Int3:
			glUniform3ivARB(i,count,(const int*)vals);
			break;
		case EUT_Int4:
			glUniform4ivARB(i,count,(const int*)vals);
			break;
		case EUT_Float1:
			glUniform1fvARB(i,count,vals);
			break;
		case EUT_Float2:
			glUniform2fvARB(i,count/2,vals);
			break;
		case EUT_Float3:
			glUniform3fvARB(i,count/3,vals);
			break;
		case EUT_Float4:
			glUniform4fvARB(i,count/4,vals);
			break;
		case EUT_Matrix2x2:
			glUniformMatrix2fvARB(i,count/4,0,vals);
			break;
		case EUT_Matrix3x3:
			glUniformMatrix3fvARB(i,count/9,0,vals);
			break;
		case EUT_Matrix4x4:
			glUniformMatrix4fvARB(i,count/16,0,vals);
			break;
		default:
			glUniform1ivARB(i,count,(int*)vals);
	};
	return 1;
}
bool SGLSLShaderProgram::setConstant(const core::string&name,const float*vals,int count){
	

	core::stringc nameStr;
	core::string_to_char(name,nameStr);
	UniformListIT it=uniformsInfo.find(nameStr);
	if(it==uniformsInfo.end())
		return 0;
	return setConstant(it->second,vals,count);;

}

//void SGLSLShaderProgram::setPixelShaderConstane(const float*data,int startReg,int constAmount);

EUniformType SGLSLShaderProgram::glslToUniform(GLenum type,size_t&outSize){
	switch(type){
	case GL_INT:
		return EUT_Int1;

	case GL_FLOAT:
		return EUT_Float1;

	case GL_FLOAT_VEC2_ARB:
		return EUT_Float2;

	case GL_FLOAT_VEC3_ARB:
		return EUT_Float3;

	case GL_FLOAT_VEC4_ARB:
		return EUT_Float4;

	case GL_FLOAT_MAT2_ARB:
		return EUT_Matrix2x2;

	case GL_FLOAT_MAT3_ARB:
		return EUT_Matrix3x3;

	case GL_FLOAT_MAT4_ARB:
		return EUT_Matrix4x4;
	}
	return EUT_Unkown;

}


void SGLSLShaderProgram::createArray(const char*name,int length){
}
void SGLSLShaderProgram::setArrayElementValue(const char*name,int index,const float*vals,int count){
}

bool SGLSLShaderProgram::UniformListShared(){
	return true;
}

int SGLSLShaderProgram::getUniformCount(){
	return uniformsInfo.size();
}
GPUUniform* SGLSLShaderProgram::getUniform(const core::string&name){

	core::stringc nameStr;
	core::string_to_char(name,nameStr);
	UniformListIT it=uniformsInfo.find(nameStr);
	if(it==uniformsInfo.end())
		return 0;
	return it->second;
}
 UniformListIT SGLSLShaderProgram::beginUniforms(){
	 return uniformsInfo.begin();
 }
 UniformListIT SGLSLShaderProgram::endUniforms(){
	 return uniformsInfo.end();
 }
 void SGLSLShaderProgram::setUniformAlias(const core::string&name,const core::string&alias){
 }



 int SGLSLShaderProgram::getAttributeIndex(EMeshStreamType attribute,int index){
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
 bool SGLSLShaderProgram::isAttributeValid(EMeshStreamType attribute,int index){
	 switch(attribute){
		case EMST_Position:
		case EMST_Normal:
		case EMST_Color:
		case EMST_Specular:
		case EMST_Texcoord:
			return false;
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
