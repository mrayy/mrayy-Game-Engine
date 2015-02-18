

#ifndef ___IShaderConstantsCallback___
#define ___IShaderConstantsCallback___

#include "compileConfig.h"
#include "GCptr.h"
#include "TextureUnit.h"

namespace mray{
	class IValue;
namespace video{
	class IGPUShaderProgram;
	class GPUShader;
	class GPUUniform;


class MRAY_DLL IShaderConstantsCallback
{
public:
	enum ESetUniformType
	{
		ESUT_Both,
		ESUT_Object,
		ESUT_Global,
	};

protected:
	virtual void setUniform(IGPUShaderProgram*shader,GPUUniform* u);
	virtual void setUniformByValue(IGPUShaderProgram*shader,GPUUniform* u,IValue*v);

	void _SetConstants(IGPUShaderProgram*p,ESetUniformType ut);

	TextureUnit m_sharedShadowMapTU;
	TextureUnit m_sharedRTTU;
public:

	IShaderConstantsCallback();
	virtual ~IShaderConstantsCallback();

	virtual void setConstants(GPUShader *shader,ESetUniformType ut=ESUT_Both);
};

MakeSharedPtrType(IShaderConstantsCallback);

}
}





#endif



