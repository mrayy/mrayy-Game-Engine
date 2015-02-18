
/********************************************************************
	created:	2008/12/25
	created:	25:12:2008   18:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\src\GPUNullShaderProgram.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\src
	file base:	GPUNullShaderProgram
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GPUNullShaderProgram___
#define ___GPUNullShaderProgram___

#include "IGPUShaderProgram.h"

namespace mray{
namespace video{

class MRAY_DLL GPUNullShaderProgram:public IGPUShaderProgram
{
protected:
	UniformList m_nullList;
	virtual uint calcSizeInternal(){
		return sizeof(GPUNullShaderProgram);
	}
	static const core::string s_Type;
public:

	GPUNullShaderProgram(EShaderProgramType t):IGPUShaderProgram(t)
	{
	}
	virtual~GPUNullShaderProgram(){
	}


	virtual const core::string& getShaderType()
	{
		return s_Type;
	}

	virtual bool LoadShader(const core::string&program, const char*entryPoint, const std::vector<core::string>& predef){ return true; }
	virtual bool LoadFromPath(const core::string&path, const char*entryPoint, const std::vector<core::string>& predef) { return true; }

	virtual const core::string& GetEntryPoint(){return core::string::Empty;}

	virtual void use(){}
	virtual void unuse(){}


	virtual void setTexture(const core::string&name,TextureUnit* tex){}
	virtual void setMatrixArray(GPUUniform*u,const math::matrix4x4* arr,int count){}
	virtual bool setConstant(GPUUniform*u,const float*vals,int count){return true;}
	virtual bool setConstant(const core::string&name,const float*vals,int count){return true;}

	virtual void createArray(const char*name,int length){}
	virtual void setArrayElementValue(const char*name,int index,const float*vals,int count){}

	// true if the Vertex and Pixel Uniforms are in the same list (ex. GLSL)
	virtual bool UniformListShared(){return true;}

	virtual void setUniformAlias(const core::string&name,const core::string&alias){}

	virtual int getUniformCount(){return 0;}
	virtual GPUUniform* getUniform(const core::string&name){return 0;}
	virtual UniformListIT beginUniforms(){return m_nullList.begin();}
	virtual UniformListIT endUniforms(){return m_nullList.end();}

	virtual int getAttributeIndex(EMeshStreamType attribute,int index){return false;}
	virtual bool isAttributeValid(EMeshStreamType attribute,int index){return true;}
};

}
}


#endif //___GPUNullShaderProgram___

