#include "stdafx.h"

#include "ImageSet.h"
#include "XMLElement.h"

#include "TextureResourceManager.h"


namespace mray
{
namespace video
{

ImageRegion::ImageRegion(ImageSet* owner):m_owner(owner)
{
}

ImageRegion::~ImageRegion()
{
}


void ImageRegion::SetName(const core::string&name)
{
	m_name=name;
}
const core::string& ImageRegion::GetName()
{
	return m_name;
}


void ImageRegion::SetSrcRect(const math::rectf& rc)
{
	m_srcRect=rc;
}

const math::rectf& ImageRegion::GetSrcRect()
{
	return m_srcRect;
}

void ImageRegion::SetTextureRect(const math::rectf& rc)
{
	m_textureRect=rc;
}

const math::rectf& ImageRegion::GetTextureRect()
{
	return m_textureRect;
}

ImageSet* ImageRegion::GetOwner()
{
	return m_owner;
}


void ImageRegion::Deserialize(xml::XMLElement*elem)
{
	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
	attr=elem->getAttribute(mT("SourceRectPos"));
	if(attr)
		m_srcRect.ULPoint=core::StringConverter::toVector2d(attr->value);
	attr=elem->getAttribute(mT("SourceRectSize"));
	if(attr)
		m_srcRect.BRPoint=m_srcRect.ULPoint+core::StringConverter::toVector2d(attr->value);
	attr=elem->getAttribute(mT("TexcoordPos"));
	if(attr)
		m_textureRect.ULPoint=core::StringConverter::toVector2d(attr->value);
	attr=elem->getAttribute(mT("TexcoordSize"));
	if(attr)
		m_textureRect.BRPoint=m_textureRect.ULPoint+core::StringConverter::toVector2d(attr->value);
}

xml::XMLElement* ImageRegion::Serialize(xml::XMLElement*elem)
{
	xml::XMLElement* ret=new xml::XMLElement(mT("ImageRegion"));
	elem->addSubElement(ret);
	elem->addAttribute(mT("Name"),m_name);
	elem->addAttribute(mT("SourceRect"),core::StringConverter::toString(m_srcRect));
	elem->addAttribute(mT("TextureRect"),core::StringConverter::toString(m_textureRect));
	return ret;
}


//////////////////////////////////////////////////////////////////////////

ImageSet::ImageSet()
{
	m_texture=new TextureUnit;
}

ImageSet::~ImageSet()
{
	ClearRegions();
}

void ImageSet::loadInternal()
{

	m_texture->SetTexture(gTextureResourceManager.loadTexture2D(m_textureName));

	if(m_texture->GetTexture())
		m_texture->GetTexture()->useResource();
}

void ImageSet::unloadInternal()
{
	if(m_texture->GetTexture())
		m_texture->GetTexture()->unuseResource();
	m_texture->SetTexture(ITexturePtr::Null);
}

uint ImageSet::calcSizeInternal()
{
	return sizeof(ImageSet)+m_regions.size()*sizeof(ImageRegion);
}
void ImageSet::SetName(const core::string&name)
{
	m_name=name;
}
const core::string& ImageSet::GetName()
{
	return m_name;
}

ImageRegion* ImageSet::CreateRegion(const core::string&name)
{
	ImageRegion* r=new ImageRegion(this);
	r->SetName(name);
	m_regions[name]=r;
	return r;
}

TextureUnit* ImageSet::GetTexture()
{
	return m_texture.pointer();
}

const core::string& ImageSet::GetTextureName()
{
	return m_textureName;
}

void ImageSet::SetTextureName(const core::string&tex)
{
	m_textureName=tex;
	reload();
}

void ImageSet::RemoveRegion(const core::string&name)
{
	RegionMap::iterator it= m_regions.find(name);
	if(it== m_regions.end())return;
	delete it->second;
	m_regions.erase(it);
}


ImageRegion* ImageSet::GetRegion(const core::string&name)
{
	RegionMap::iterator it= m_regions.find(name);
	if(it== m_regions.end())return 0;
	return it->second;
}


void ImageSet::ClearRegions()
{
	RegionMap::iterator it= m_regions.begin();
	for (;it!=m_regions.end();++it)
	{
		delete it->second;
	}
	m_regions.clear();
}

void ImageSet::Deserialize(xml::XMLElement*elem)
{
	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
	attr=elem->getAttribute(mT("TextureName"));
	if(attr)
		m_textureName=attr->value;

	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("ImageRegion")))
		{
			ImageRegion*r=new ImageRegion(this);
			r->Deserialize(e);
			m_regions[r->GetName()]=r;
		}
	}

}


xml::XMLElement* ImageSet::Serialize(xml::XMLElement*elem)
{
	xml::XMLElement* ret=new xml::XMLElement(mT("ImageSet"));
	elem->addSubElement(ret);
	ret->addAttribute(mT("Name"),m_name);
	ret->addAttribute(mT("TextureName"),m_textureName);


	RegionMap::iterator it= m_regions.begin();
	for (;it!=m_regions.end();++it)
	{
		it->second->Serialize(ret);
	}

	return ret;
}


}
}