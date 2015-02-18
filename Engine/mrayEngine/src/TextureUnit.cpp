#include "stdafx.h"

#include "TextureUnit.h"

#include "TextureResourceManager.h"

#include "StringUtil.h"
#include "TextureSourceFactory.h"


namespace mray{
namespace video{


TextureUnit::TextureUnit():
trans(0),rotate(0),scale(1)
{
	for(int i=0;i<ETGT_Count;++i){
		m_enableGenMode[i]=0;
		m_texGenMode[i]=ETGM_EyeLinear;
	}
	m_textureWrap[ETW_WrapS]=ETC_REPEAT;
	m_textureWrap[ETW_WrapT]=ETC_REPEAT;
	m_textureWrap[ETW_WrapR]=ETC_REPEAT;
}
TextureUnit::TextureUnit(TextureUnit*o)
{
	for(int i=0;i<ETGT_Count;++i){
		m_enableGenMode[i]=o->m_enableGenMode[i];
		m_texGenMode[i]=o->m_texGenMode[i];
	}
	m_textureWrap[ETW_WrapS]=o->m_textureWrap[ETW_WrapS];
	m_textureWrap[ETW_WrapT]=o->m_textureWrap[ETW_WrapT];
	m_textureWrap[ETW_WrapR]=o->m_textureWrap[ETW_WrapR];
	SetTexture(o->GetTexture());
	trans=o->trans;
	rotate=o->rotate;
	scale=o->scale;
}
TextureUnit::~TextureUnit()
{
	UnuseResources();
}


void TextureUnit::SetTexture(const ITexturePtr&t)
{
	m_texture=t;
}
const ITexturePtr& TextureUnit::GetTexture()
{
	return m_texture;
}

void TextureUnit::SetTextureSource(ITextureSourceCRef t)
{
	m_textureSource=t;
	if(!m_textureSource.isNull())
	{
		m_texture=m_textureSource->GetTexture();
	}
}

void TextureUnit::setTextureGenMode(ETextureGenType type,ETextureGenMode mode){
	m_texGenMode[type]=mode;
}
ETextureGenMode TextureUnit::getTextureGenMode(ETextureGenType type){
	return m_texGenMode[type];
}
void TextureUnit::enableGenMode(ETextureGenType type,bool e){
	m_enableGenMode[type]=e;
}
bool TextureUnit::isEnableGenMode(ETextureGenType type){
	return m_enableGenMode[type];
}


void TextureUnit::updateMatrix(){
	transform.makeIdentity();
	if(trans!=0)
		transform.setTranslation(trans);
	if(rotate!=0)
		transform.setRotationDegrees(rotate);
	if(scale!=1){
		math::matrix4x4 mat;
		mat.setScale(scale);
		transform=transform*mat;
	}
}
xml::XMLElement* TextureUnit::loadXMLSettings(xml::XMLElement*e){
	xml::XMLAttribute*attr;
	xml::XMLElement* elem=e->getSubElement(mT("TextureUnit"));
	if(!elem)return 0;
	
	attr=elem->getAttribute(mT("Translate"));
	if(attr)
		core::StringConverter::parse(attr->value,trans);
	attr=elem->getAttribute(mT("Rotate"));
	if(attr)
		core::StringConverter::parse(attr->value,rotate);
	attr=elem->getAttribute(mT("Scale"));
	if(attr)
		core::StringConverter::parse(attr->value,scale);

	attr=elem->getAttribute(mT("AddressS"));
	if(attr)
		m_textureWrap[ETW_WrapS]= (ETextureClamp)core::StringConverter::toEnum(attr->value,"ETextureClamp");
	attr=elem->getAttribute(mT("AddressT"));
	if(attr)
		m_textureWrap[ETW_WrapT]= (ETextureClamp)core::StringConverter::toEnum(attr->value,"ETextureClamp");
	attr=elem->getAttribute(mT("AddressR"));
	if(attr)
		m_textureWrap[ETW_WrapR]= (ETextureClamp)core::StringConverter::toEnum(attr->value,"ETextureClamp");

	ETextureType type=ETT_2DTex;
	attr=elem->getAttribute(mT("TextureType"));
	if(attr)
		type=(ETextureType)core::StringConverter::toEnum(mT("ETextureType"),attr->value);

	xml::XMLElement* texSrc=elem->getSubElement("TextureSource");
	if(texSrc)
	{
		attr=texSrc->getAttribute("Factory");
		if(attr)
		{
			m_textureSource=TextureSourceFactory::getInstance().CreateObject(attr->value);
			if(m_textureSource)
			{
				m_textureSource->LoadFromXML(texSrc);
				m_textureSource->Load();
				m_texture=m_textureSource->GetTexture();
			}
		}
	}
	if(!m_texture)
	{

		attr=elem->getAttribute(mT("Texture"));
		if(attr)
		{
			m_texture=gTextureResourceManager.getResource(attr->value);
			if(m_texture.pointer()==0)
			{
				switch (type)
				{
				case ETT_1DTex:
					m_texture=gTextureResourceManager.loadTexture1D(attr->value);
					break;
				case ETT_2DTex:
					m_texture=gTextureResourceManager.loadTexture2D(attr->value);
					break;
				case ETT_3DTex:
					m_texture=gTextureResourceManager.loadTexture3D(attr->value);
					break;
				case ETT_CubeTex:
					{
						core::string base,ext;
						core::StringUtil::SplitPathExt(attr->value,base,ext);
						std::vector<core::string> paths;
						paths.push_back(base+"+x."+ext);
						paths.push_back(base+"-x."+ext);
						paths.push_back(base+"+y."+ext);
						paths.push_back(base+"-y."+ext);
						paths.push_back(base+"+z."+ext);
						paths.push_back(base+"-z."+ext);
						m_texture=gTextureResourceManager.loadTextureCube(attr->value,&paths[0],true);
					}
					break;
				}
			}
		}
	}
	return elem;

}
xml::XMLElement* TextureUnit::exportXMLSettings(xml::XMLElement*elem){
	xml::XMLElement*e=new xml::XMLElement(mT("TextureUnit"));
	elem->addSubElement(e);
	e->addAttribute(mT("Translate"),core::StringConverter::toString(trans));
	e->addAttribute(mT("Rotate"),core::StringConverter::toString(rotate));
	e->addAttribute(mT("Scale"),core::StringConverter::toString(scale));

	if(m_texture.pointer())
	{
		e->addAttribute(mT("TextureType"),core::StringConverter::toString((int)m_texture->getType(),core::string(mT("ETextureType"))));
		e->addAttribute(mT("Texture"),m_texture->getResourceName());
	}
	return e;
}

}
}
