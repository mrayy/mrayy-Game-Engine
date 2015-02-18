

#include "stdafx.h"
#include "FileTextureSource.h"
#include "TextureResourceManager.h"
#include "StringUtil.h"

namespace mray
{
namespace video
{
const core::string FileTextureSource::FactorySourceType=mT("FileTexture");

FileTextureSource::FileTextureSource()
{
	m_type=ETT_2DTex;
}

FileTextureSource::~FileTextureSource()
{
}


bool FileTextureSource::_Load()
{
	switch(m_type)
	{
	case ETT_1DTex:
		m_texture=TextureResourceManager::getInstance().loadTexture1D(m_fileName);
		break;
	case ETT_2DTex:
		m_texture=TextureResourceManager::getInstance().loadTexture2D(m_fileName);
		break;
	case ETT_3DTex:
		m_texture=TextureResourceManager::getInstance().loadTexture3D(m_fileName);
		break;
	case ETT_CubeTex:
		{

			core::string base,ext;
			core::StringUtil::SplitPathExt(m_fileName,base,ext);
			std::vector<core::string> paths;
			paths.push_back(base+"+x."+ext);
			paths.push_back(base+"-x."+ext);
			paths.push_back(base+"+y."+ext);
			paths.push_back(base+"-y."+ext);
			paths.push_back(base+"+z."+ext);
			paths.push_back(base+"-z."+ext);
			m_texture=gTextureResourceManager.loadTextureCube(m_fileName,&paths[0],true);
		}
		break;

	}
	if(!m_texture.isNull())
		m_texture->load();
	return !m_texture.isNull();
}

bool FileTextureSource::_Unload()
{
	if(m_texture.isNull())
		return false;
	m_texture->unload();
	return true;
}



ITextureCRef FileTextureSource::GetTexture()
{
	return m_texture;
}

xml::XMLElement* FileTextureSource::LoadFromXML(xml::XMLElement* e)
{
	m_fileName=e->getValueString("Path");
	if(e->getValueString("Type")!="")
		m_type=(ETextureType)core::StringConverter::toEnum(mT("ETextureType"),e->getValueString("Type"));

	return e;
}


IMPLEMENT_TEXTURE_FACTORY(FileTextureSource);

}
}

