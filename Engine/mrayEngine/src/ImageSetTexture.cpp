#include "stdafx.h"

#include "ImageSetTexture.h"
#include "ImageSetResourceManager.h"

namespace mray
{
namespace video
{

ImageSetTexture::ImageSetTexture()
{
	m_imageSet=0;
	m_image=0;
}
ImageSetTexture::ImageSetTexture(const core::string&set,const core::string&image):m_imageSetName(set),m_imageName(image)
{
	SetImageSetName(set);
}
ImageSetTexture::~ImageSetTexture()
{
}

void ImageSetTexture::SetImageSetName(const core::string&name)
{
	m_imageSetName=name;
	m_imageSet= gImageSetResourceManager.loadImageSet(m_imageSetName);
	if(m_imageSet){
		m_image=m_imageSet->GetRegion(m_imageName);
	}else m_image=0;
}
const core::string& ImageSetTexture::GetImageSetName()const
{
	return m_imageSetName;
}

void ImageSetTexture::SetImageName(const core::string&name)
{
	m_imageName=name;
	if(m_imageSet){
		m_image=m_imageSet->GetRegion(m_imageName);
	}else m_image=0;
}
const core::string& ImageSetTexture::GetImageName()const
{
	return m_imageName;
}

ImageSet* ImageSetTexture::GetImageSet()const
{
	return m_imageSet;
}
ImageRegion* ImageSetTexture::GetImage()const
{
	return m_image;
}

void ImageSetTexture::Deserialize(xml::XMLElement*elem)
{
	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Set"));
	if(attr)
		m_imageSetName=attr->value;
	attr=elem->getAttribute(mT("Image"));
	if(attr)
		m_imageName=attr->value;

	SetImageSetName(m_imageSetName);
}
xml::XMLElement* ImageSetTexture::Serialize(xml::XMLElement*elem)
{
	xml::XMLElement* ret=new xml::XMLElement(mT("ImageSetTexture"));
	elem->addSubElement(ret);
	elem->addAttribute(mT("Set"),m_imageSetName);
	elem->addAttribute(mT("Image"),m_imageName);
	return ret;
}


}
}

