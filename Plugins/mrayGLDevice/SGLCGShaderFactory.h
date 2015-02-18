


#ifndef ___SGLCGShaderFactory___
#define ___SGLCGShaderFactory___


#include <IGPUShaderFactory.h>
#include <cg\cg.h>
#include "compileConfig.h"

namespace mray{
namespace video{
	class mrayDev;
	class GPUShader;
	class IShaderConstantsCallback;
	class SGLMaterialRenderer;

class MRAY_GL_DLL SGLCGShaderFactory:public IGPUShaderFactory
{
	CGcontext m_context;
public:
	SGLCGShaderFactory();
	virtual ~SGLCGShaderFactory();

	virtual const core::string& getType();

	virtual bool canCreateType(const core::string&type);

	virtual IGPUShaderProgram* createShader(EShaderProgramType type);
};

}
}


#endif




