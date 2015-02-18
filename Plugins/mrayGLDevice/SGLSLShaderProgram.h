

#ifndef ___SGLSLShaderProgram___
#define ___SGLSLShaderProgram___

#include <gl/ExtGL.h>
#include "GLDev.h"
#include <GPUShader.h>


namespace mray{
namespace video{
	class IShaderConstantsCallback;

enum EVertexShaderType{
	VST_1_1=0,
	VST_2_0,
	VST_2_a,
	VST_3_0,
	VST_count
};

const mchar*const VertexShaderType_Names[]={
	mT("VS_1.1"),
	mT("VS_2.0"),
	mT("VS_2.a"),
	mT("VS_3.0"),
	mT("")
};


enum EPixelShaderType{
	PST_1_1=0,
	PST_1_2,
	PST_1_3,
	PST_1_4,
	PST_2_0,
	PST_2_a,
	PST_2_b,
	PST_3_0,
	PST_count
};

const mchar*const PixelShaderType_Names[]={
	mT("PS_1.1"),
	mT("PS_1.2"),
	mT("PS_1.3"),
	mT("PS_1.4"),
	mT("PS_2.0"),
	mT("PS_2.a"),
	mT("PS_2.b"),
	mT("PS_3.0"),
	mT("")
};


class MRAY_GL_DLL SGLSLShaderProgram:public IGPUShaderProgram
{
protected:
	class GLSLUniform:public GPUUniform
	{
	public:
		int loc;
	};

	UniformList uniformsInfo;
	std::vector<GCPtr<ITexture> >textures;
	core::string m_Prog;


	static const core::string m_entryPoint;

	GLhandleARB hProgram;

	bool createProgram();
	bool linkProgram();
	bool init(const char*program);
	bool createShader(GLenum shaderType,const char*shaderName);
	
	EUniformType glslToUniform(GLenum type,size_t&outSize);
	virtual void unloadInternal();


	virtual uint calcSizeInternal();
public:
	SGLSLShaderProgram(EShaderProgramType type);
	virtual ~SGLSLShaderProgram();



	virtual bool LoadShader(const core::string&program, const char*entryPoint, const std::vector<core::string>& predef) ;
	virtual bool LoadFromPath(const core::string&path, const char*entryPoint, const std::vector<core::string>& predef);

	virtual const core::string& getShaderType();

	virtual const core::string& GetEntryPoint(){return m_entryPoint;}

	virtual void use();
	virtual void unuse();

	virtual void setTexture(const core::string&name,TextureUnit* tex);
	virtual void setMatrixArray(GPUUniform*u,const math::matrix4x4* arr,int count);
	virtual bool setConstant(GPUUniform*u,const float*vals,int count);
	virtual bool setConstant(const core::string&name,const float*vals,int count);

	virtual void createArray(const char*name,int length);
	virtual void setArrayElementValue(const char*name,int index,const float*vals,int count);

	// true if the Vertex and Pixel Uniforms are in the same list (ex. GLSL)
	virtual bool UniformListShared();

	virtual void setUniformAlias(const core::string&name,const core::string&alias);

	virtual int getUniformCount();
	virtual GPUUniform* getUniform(const core::string&name);
	virtual UniformListIT beginUniforms();
	virtual UniformListIT endUniforms();

	virtual int getAttributeIndex(EMeshStreamType attribute,int index);
	virtual bool isAttributeValid(EMeshStreamType attribute,int index);
};

}
}



#endif


