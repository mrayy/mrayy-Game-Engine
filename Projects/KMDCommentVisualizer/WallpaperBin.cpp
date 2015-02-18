

#include "stdafx.h"
#include "WallpaperBin.h"
#include "IOSystem.h"
#include "IDirOS.h"
#include "TextureResourceManager.h"


namespace mray
{
namespace kmd
{


WallpaperBin::WallpaperBin()
{
	m_nextPicture = 0;
}

WallpaperBin::~WallpaperBin()
{
	m_textures.clear();
}


void WallpaperBin::LoadPicturesFromDir(const core::string& path)
{
	 GCPtr<OS::IDirOS> dir= gOSystem.createDirSystem();
	 dir->changeDir(path);

	 std::vector<OS::SFileData> files= dir->getFiles();
	 for (int i = 2; i < files.size();++i)
	 {
		 if (files[i].isDir == false)
		 {
			 m_picturesNames.push_back(files[i].name);
			 m_textures.push_back(gTextureResourceManager.loadTexture2D(files[i].name));
		 }
		 else
			 LoadPicturesFromDir(files[i].name);
	 }


	 dir->changeDir(gFileSystem.getAppPath());

	 if (m_textures.size()>0)
		 m_nextPicture = (int)math::Randomizer::randRange(0, m_textures.size() - 1);
}


std::vector<core::string> WallpaperBin::GetPicturesNames()
{
	return m_picturesNames;
}

std::vector<video::ITexturePtr> WallpaperBin::GetPictures()
{
	return m_textures;
}


video::ITexturePtr WallpaperBin::GetNextRandomPicture()
{
	if (m_textures.size() > 0)
	{
		m_nextPicture = (m_nextPicture + (int)math::Randomizer::randRange(1, m_textures.size() - 1)) % m_textures.size();
		m_textures[m_nextPicture]->load(false);
		return m_textures[m_nextPicture];
	}
	else
		return video::ITexturePtr::Null;

}


}
}
