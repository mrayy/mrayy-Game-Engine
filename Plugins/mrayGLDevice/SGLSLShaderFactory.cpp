#include "stdafx.h"


#include "SGLSLShaderProgram.h"
#include <GPUShader.h>
#include <IShaderConstantsCallback.h>
#include "SGLSLShaderFactory.h"
#include "GLDev.h"
#include "IStream.h"


namespace mray{
namespace video{

	const core::string s_shaderType=mT("glsl");

SGLSLShaderFactory::SGLSLShaderFactory(){
}
SGLSLShaderFactory::~SGLSLShaderFactory(){
}

const core::string& SGLSLShaderFactory::getType(){
	return s_shaderType;
}

bool SGLSLShaderFactory::canCreateType(const core::string& type){
	return s_shaderType.equals_ignore_case(type);
}

IGPUShaderProgram* SGLSLShaderFactory::createShader(EShaderProgramType type)
{
//	if(device->getDeviceType()==GLDev::m_deviceType)
	{
		IGPUShaderProgram* prog=new SGLSLShaderProgram(type);
		
		return prog;
	}

	return 0;
}
/*
IGPUShaderProgram* SGLSLShaderFactory::createShader(video::IVideoDevice*device,EShaderProgramType type,OS::IStream* programStream,const char*entryPoint)
{
	if(device->getDeviceType()==GLDev::m_deviceType)
	{
		int sz=programStream->length();
		programStream->seek(0,OS::ESeek_Set);
		char*data=new char[sz+1];
		int r=programStream->read(data,sz);

		core::string progStr;
		core::char_to_string(data,progStr);

		IGPUShaderProgram*prog= createShader(device,type,false,progStr,entryPoint);

		delete [] data;

		return prog;
	}

	return 0;
}*/

}
}




