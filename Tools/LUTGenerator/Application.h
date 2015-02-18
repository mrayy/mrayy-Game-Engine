

/********************************************************************
	created:	2011/05/02
	created:	2:5:2011   15:22
	filename: 	d:\Development\mrayEngine\Tools\LUTGenerator\Application.h
	file path:	d:\Development\mrayEngine\Tools\LUTGenerator
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Application__
#define __Application__


namespace mray
{

class Application:public CMRayApplication
{
private:
protected:
	video::ITexture* m_generatedTexture;
	video::ITexturePtr m_texture2D;
public:
	Application();
	virtual~Application();
	
	virtual void init(const OptionContainer &extraOptions);

	video::ITexturePtr Generate3DLUTTexture();
	video::ITexturePtr Convert3DTextureTo2DTexture(const video::ITexturePtr& tex);
	video::ITexturePtr Convert2DTextureTo3DTexture(const video::ITexturePtr& tex,int rows,int cols);
};


}

#endif
