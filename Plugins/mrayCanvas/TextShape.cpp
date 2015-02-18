

#include "Stdafx.h"
#include "TextShape.h"
#include "ICanvasObject.h"
#include "CanvasManager.h"

namespace mray
{
namespace canvas
{

TextShape::TextShape()
:m_color(0,0,0,1)
{
}
TextShape::~TextShape()
{
}

void TextShape::Draw(EMouseStatus status)
{
	if(!m_owner)
		return;

	m_owner->GetCanvasManager()->DrawString(m_string,m_rect,m_color);
}

bool TextShape::IsPointIn(const math::vector2d& pt)
{
	return m_rect.IsPointInside(pt);
}


}
}