#include "stdafx.h"

#include "TextDecorateNode.h"
#include "IFont.h"

namespace mray
{
namespace GUI
{

	const core::string TextDecorateNode::Type="Text";
TextDecorateNode::TextDecorateNode(const core::string&text)
{
	m_renderText.Set(text);
}

TextDecorateNode::~TextDecorateNode()
{
}

void TextDecorateNode::Draw(TextContextAttributes*context,IGUIRenderer*renderer,const math::rectf*clip)
{
	if(!context->font)
		return;
	math::vector2d pos=context->pos;
	pos.y-=m_size.y*0.5;
	math::rectf rc(pos,pos+m_size);
	context->font->print(rc,&context->fontAttributes,&rc,m_renderText,renderer);

	context->pos.x+=m_size.x;
}
void TextDecorateNode::CalculateSize(TextContextAttributes*context)
{
	if(!context->font)
		return;
	m_size=context->font->getTextDimension(m_renderText,&context->fontAttributes,0);
}

}
}

