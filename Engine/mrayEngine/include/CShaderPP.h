



#ifndef ___CShaderPP___
#define ___CShaderPP___


#include "IPostProcessing.h"
#include "GPUShader.h"
#include "TextureUnit.h"
#include "IRenderTarget.h"

namespace mray{
namespace video{


class MRAY_DLL CShaderPP:public IPostProcessing
{
protected:
	GPUShaderPtr m_shader;
	IRenderTargetPtr m_renderTarget;
	math::vector2d m_targetSize;
	TextureUnitPtr m_texUnit;
	video::IVideoDevice*device;
public:
	CShaderPP(const GPUShaderPtr& shader,video::IVideoDevice*dev,const math::vector2d&size,bool depthBuff=false);
	virtual ~CShaderPP();

	virtual void Setup(const math::rectf& targetVP);
	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget*getOutput();
};

}
}

#endif




