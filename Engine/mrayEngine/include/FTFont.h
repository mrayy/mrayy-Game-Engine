
#ifndef ___FTFont___
#define ___FTFont___


#include "BaseFont.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace mray{
namespace GUI{

// true type font
class MRAY_DLL FTFont:public BaseFont
{
	uint m_ttSize;
	uint m_ttResolution;
	int m_ttfMaxBearingY;
	bool m_AntialiasColor;


	FT_Face m_fontFace;

	virtual uint calcSizeInternal();
public:
	FTFont(const core::string&name,uint ttSize,uint ttResolution);
	virtual~FTFont();


	void loadFont(OS::IStream*font);
	void loadFont(xml::XMLElement*elem);	
	

};


}
}






#endif

