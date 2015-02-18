


#ifndef ___SGLSLShaderFactory___
#define ___SGLSLShaderFactory___


#include <IGPUShaderFactory.h>
#include "compileConfig.h"

namespace mray{
namespace video{
	class mrayDev;
	class GPUShader;
	class IShaderConstantsCallback;
	class SGLMaterialRenderer;

class MRAY_GL_DLL SGLSLShaderFactory:public IGPUShaderFactory
{
public:
	SGLSLShaderFactory();
	virtual ~SGLSLShaderFactory();

	virtual const core::string& getType();

	virtual bool canCreateType(const core::string& type);
	
	virtual IGPUShaderProgram* createShader(EShaderProgramType type);
};

}
}


#endif




