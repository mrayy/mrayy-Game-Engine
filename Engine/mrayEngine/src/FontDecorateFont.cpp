#include "stdafx.h"


#include "FontDecorateFont.h"
#include "StringConverter.h"
#include "FontResourceManager.h"


namespace mray
{
namespace GUI
{
	const core::string FontDecorateFont::Type = "Font";

FontDecorateFont::FontDecorateFont()
{
	m_Font=0;
	m_SavedFont=0;
}
FontDecorateFont::~FontDecorateFont()
{
}

void FontDecorateFont::ParseParameter(const core::UTFString &p)
{
	m_Font=gFontResourceManager.getFontByName(p.GetAsString());
}

void FontDecorateFont::OnEnter(TextContextAttributes*context)
{
	m_SavedFont=context->font;
	context->font=m_Font;
}
void FontDecorateFont::OnExit(TextContextAttributes*context)
{
	context->font=m_SavedFont;
}

}
}



