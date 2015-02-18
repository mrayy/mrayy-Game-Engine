

/********************************************************************
	created:	2009/02/02
	created:	2:2:2009   20:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\FontAttributes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	FontAttributes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	font descriptor
*********************************************************************/

#ifndef ___FontAttributes___
#define ___FontAttributes___

#include "mString.h"

#include "SColor.h"
#include "ISerializable.h"

namespace mray{
namespace GUI{

	enum EFontAligment
	{
		EFA_TopLeft,
		EFA_TopCenter,
		EFA_TopRight,

		EFA_MiddleLeft,
		EFA_MiddleCenter,
		EFA_MiddleRight,

		EFA_BottomLeft,
		EFA_BottomCenter,
		EFA_BottomRight,
	};

class MRAY_DLL FontAttributes:public ISerializable
{
public:

	core::string	fontName;
	float			fontSize;
	video::SColor	fontColor;
	video::SColor	shadowColor;
	bool			hasShadow;
	math::vector2d	shadowOffset;
	int				spacing;
	int				lineSpacing;
	bool			wrap;
	bool			RightToLeft;

	EFontAligment	fontAligment;

public:
	FontAttributes();
	virtual~FontAttributes();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}

#endif //___FontAttributes___
