


/********************************************************************
	created:	2012/02/20
	created:	20:2:2012   9:23
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IDeviceListener.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IDeviceListener
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IDeviceListener__
#define __IDeviceListener__


namespace mray
{
namespace video
{

	class IVideoDevice;
	class RenderPass;
	class TextureUnit;
	class IGPUShaderProgram;

class IDeviceListener
{
private:
protected:
public:
	IDeviceListener(){}
	virtual~IDeviceListener(){}
	
	virtual void OnPreRender(IVideoDevice* device){};
	virtual void OnPostRender(IVideoDevice* device){};

	virtual void OnMaterialUsed(IVideoDevice* device,RenderPass* v){};
	virtual void OnTextureUsed(IVideoDevice* device,TextureUnit* v){};
	virtual void OnShaderUsed(IVideoDevice* device,IGPUShaderProgram* v){};

	virtual void OnDeviceShutdown(IVideoDevice* device){}

};


}
}

#endif
