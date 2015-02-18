#include "stdafx.h"

#include "SizeDecorateNode.h"
#include "StringConverter.h"

namespace mray
{
namespace GUI
{
	const core::string SizeDecorateNode::Type = "Size";

SizeDecorateNode::SizeDecorateNode()
{
	m_Size=10;
	m_SavedSize=0;
}
SizeDecorateNode::~SizeDecorateNode()
{
}

void SizeDecorateNode::ParseParameter(const core::UTFString &p)
{
	m_Size=core::StringConverter::toFloat(p.GetAsString());
}

void SizeDecorateNode::OnEnter(TextContextAttributes*context)
{
	m_SavedSize=context->fontAttributes.fontSize;
	context->fontAttributes.fontSize=m_Size;
}
void SizeDecorateNode::OnExit(TextContextAttributes*context)
{
	context->fontAttributes.fontSize=m_SavedSize;
}

}
}


