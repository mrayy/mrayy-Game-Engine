
/********************************************************************
created:	2011/04/28
created:	28:4:2011   22:28
filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGPUShaderProgram.h
file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
file base:	IGPUShaderProgram
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/



#ifndef __IGPUShaderProgram__
#define __IGPUShaderProgram__

#include "IResource.h"
#include "videoCommon.h"
#include "GPUUniform.h"


namespace mray
{
namespace video
{

class ITexture;
class TextureUnit;
class IVideoDevice;

enum EShaderProgramType
{
	EShader_VertexProgram,
	EShader_FragmentProgram,
	EShader_GeometryProgram
};

typedef std::vector<core::string> ShaderPredefList;

class MRAY_DLL IGPUShaderProgram:public IResource
{
private:
protected:
	core::string m_name;
	EShaderProgramType m_type;

public:
	IGPUShaderProgram(EShaderProgramType type):m_type(type)
	{}
	virtual~IGPUShaderProgram(){}

	virtual const core::string& getShaderType()=0;

	EShaderProgramType GetProgramType(){return m_type;}

	const core::string& getName(){return m_name;}

	GPUUniform* operator[](const core::string& name)
	{
		return getUniform(name);
	}

	virtual bool LoadShader(const core::string&program, const char*entryPoint, const ShaderPredefList& predef) = 0;
	virtual bool LoadFromPath(const core::string&path, const char*entryPoint, const ShaderPredefList& predef) = 0;

	virtual const core::string& GetEntryPoint()=0;

	virtual void use()=0;
	virtual void unuse()=0;

	virtual void setTexture(const core::string&name,TextureUnit* tex)=0;
	virtual void setMatrixArray(GPUUniform*u,const math::matrix4x4* arr,int count)=0;
	virtual bool setConstant(GPUUniform*u,const float*vals,int count)=0;
	virtual bool setConstant(const core::string&name,const float*vals,int count)=0;

	virtual void createArray(const char*name,int length)=0;
	virtual void setArrayElementValue(const char*name,int index,const float*vals,int count)=0;

	// true if the Vertex and Pixel Uniforms are in the same list (ex. GLSL)
	virtual bool UniformListShared()=0;

	virtual void setUniformAlias(const core::string&name,const core::string&alias)=0;

	virtual int getUniformCount()=0;
	virtual GPUUniform* getUniform(const core::string&name)=0;
	virtual UniformListIT beginUniforms()=0;
	virtual UniformListIT endUniforms()=0;

	virtual int getAttributeIndex(EMeshStreamType attribute,int index)=0;
	virtual bool isAttributeValid(EMeshStreamType attribute,int index)=0;

};

MakeSharedPtrType(IGPUShaderProgram);

}
}

#endif
