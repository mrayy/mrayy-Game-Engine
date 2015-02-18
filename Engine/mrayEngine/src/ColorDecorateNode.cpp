#include "stdafx.h"

#include "ColorDecorateNode.h"
#include "StringConverter.h"


namespace mray
{
namespace GUI
{
	const core::string ColorDecorateNode::Type = "Color";

ColorDecorateNode::ColorDecorateNode()
{
}
ColorDecorateNode::~ColorDecorateNode()
{
}

void ColorDecorateNode::ParseParameter(const core::UTFString &p)
{
	m_Color=core::StringConverter::toColor(p.GetAsString());
}

void ColorDecorateNode::OnEnter(TextContextAttributes*context)
{
	m_SavedColor=context->fontAttributes.fontColor;
	context->fontAttributes.fontColor=m_Color;
}
void ColorDecorateNode::OnExit(TextContextAttributes*context)
{
	context->fontAttributes.fontColor=m_SavedColor;
}

}
}


