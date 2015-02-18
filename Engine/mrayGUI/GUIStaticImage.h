

/********************************************************************
	created:	2011/11/27
	created:	27:11:2011   18:01
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIStaticImage.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIStaticImage
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIIMAGE___
#define ___GUIIMAGE___

#include "CompileConfig.h"
#include "IGUIStaticImage.h"
#include "IDelegate.h"
#include "FontAttributes.h"
#include "TextureUnit.h"

#include "GUIElementFactoryDef.h"


namespace mray{
namespace GUI{

	enum EImageStretchMode
	{
		EImage_None,
		EImage_Stretch,
		EImage_Tile,
		EImage_Center,	//ratio is 1, image is centered
		EImage_Zoom,	// ratio is 1, image is scaled to fit
		EImage_Fit		// ratio is 1, image is shrinked to fit
	};

class MRAYGUI_API GUIStaticImage:public IGUIStaticImage
{
	DECLARE_RTTI
protected:
	video::TextureUnitPtr m_textureUnit;
	math::rectf m_texCoords;
	core::string m_source;
	EImageStretchMode m_stretchMode;
	bool m_clipping;

	float m_angle;

	virtual void fillProperties();
	bool _OnMouseEvent(MouseEvent* e);
public:

	DECLARE_PROPERTY_TYPE(StretchMode, EImageStretchMode, MRAYGUI_API);
	DECLARE_PROPERTY_TYPE(TexCoords, math::rectf, MRAYGUI_API);
	DECLARE_PROPERTY_TYPE(Source,core::string,MRAYGUI_API);
	DECLARE_PROPERTY_TYPE(Clipping, bool, MRAYGUI_API);
public:

	GUIStaticImage(IGUIManager* creator);
	~GUIStaticImage();

	virtual bool SetAngle(float angle){ m_angle = angle; return true; }
	float GetAngle(){ return m_angle; }

	virtual void SetImage(video::ITextureCRef tex);
	video::ITextureCRef GetImage();

	video::TextureUnit* GetTextureUnit();

	void SetStretchMode(EImageStretchMode m);
	EImageStretchMode  GetStretchMode(){ return m_stretchMode ; }

	virtual bool SetTargetTexCoords(const math::rectf& rc);
	const math::rectf&  GetTargetTexCoords();

	virtual bool SetSourceImage(const core::string&path);
	const core::string& GetSourceImage();

	virtual bool SetClipping(bool clipping);
	bool GetClipping();

	virtual void Draw(const math::rectf*vp);

	IGUIElement* Duplicate();
	FontAttributes* GetFontAttributes();
};
DECLARE_ELEMENT_FACTORY(GUIStaticImage);

}
}

#endif

