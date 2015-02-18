
/********************************************************************
	created:	2011/11/27
	created:	27:11:2011   17:58
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIStaticImage.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIStaticImage
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIStaticImage__
#define __IGUIStaticImage__

#include "IGUIElement.h"
#include "ITexture.h"

namespace mray
{
namespace video
{
	class TextureUnit;
}
namespace GUI
{

class MRAY_DLL IGUIStaticImage:public IGUIElement
{
private:
protected:
public:
	static const GUID ElementType;

	ObjectEvent OnClick;//args=MouseButton
	ObjectEvent OnMouseMove;//args=vector2d
public:
	IGUIStaticImage(IGUIManager* creator);
	virtual~IGUIStaticImage();

	virtual void SetImage(video::ITextureCRef tex)=0;
	virtual video::ITextureCRef GetImage()=0;

	virtual video::TextureUnit* GetTextureUnit()=0;

	virtual bool SetTargetTexCoords(const math::rectf& rc)=0;
	virtual const math::rectf&  GetTargetTexCoords()=0;
};


}
}

#endif
