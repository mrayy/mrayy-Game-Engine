#include "stdafx.h"

#include "ImageDecorateNode.h"
#include "IFont.h"
#include "DefaultColors.h"
#include "TextureResourceManager.h"
#include "IVideoDevice.h"
#include "StringUtil.h"

namespace mray
{
namespace GUI
{
	const core::string ImageDecorateNode::Type = "Image";


ImageDecorateNode::ImageDecorateNode()
{
	m_image=new video::TextureUnit();
	m_size=20;//default size
}

ImageDecorateNode::~ImageDecorateNode()
{
}

void ImageDecorateNode::ParseParameter(const core::UTFString &p)
{
	std::vector<core::string> parameters=core::StringUtil::Split(p.GetAsString(),mT(","));
	if(parameters.size()==0)
		return;

	m_image->SetTexture(gTextureResourceManager.loadTexture2D(parameters[0]));
	if(parameters.size()>=2)
		m_size.x=core::StringConverter::toFloat(parameters[1]);
	if(parameters.size()>=3)
		m_size.y=core::StringConverter::toFloat(parameters[2]);
	else
		m_size.y=m_size.x;
}

void ImageDecorateNode::Draw(TextContextAttributes*context,const math::rectf*clip)
{
	if(!context->device)
		return;

	math::vector2d pos=context->pos;
	pos.y-=m_size.y*0.5;
	math::rectf rc(pos,pos+m_size);
	context->device->useTexture(0,m_image);
	context->device->draw2DImage(math::rectf(pos,pos+m_size),video::DefaultColors::White);

	context->pos.x+=m_size.x;
}
void ImageDecorateNode::CalculateSize(TextContextAttributes*context)
{
}

}
}

