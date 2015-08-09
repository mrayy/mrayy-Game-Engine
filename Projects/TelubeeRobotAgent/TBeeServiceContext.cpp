
#include "stdafx.h"
#include "TBeeServiceContext.h"

namespace mray
{
namespace TBee
{


	void TbeeServiceRenderContext::RenderText(const core::string &txt, int x, int y, const video::SColor& clr)
{

		fontAttrs.fontColor = clr;
	font->print(math::rectf(x, y+m_yOffset, 10, 10), &fontAttrs, 0, txt, guiRenderer);
	m_yOffset += fontAttrs.fontSize;
}

void TbeeServiceRenderContext::Reset()
{
	m_yOffset = 0;
	m_xOffset = 0;

	fontAttrs.fontColor.Set(0.05, 1, 0.5, 1);
	fontAttrs.fontAligment = GUI::EFA_MiddleLeft;
	fontAttrs.fontSize = 18;
	fontAttrs.hasShadow = true;
	fontAttrs.shadowColor.Set(0, 0, 0, 1);
	fontAttrs.shadowOffset = math::vector2d(2);
	fontAttrs.spacing = 2;
	fontAttrs.wrap = 0;
	fontAttrs.RightToLeft = 0;
}

}
}


