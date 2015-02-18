#include "stdafx.h"

#include "SkyBoxManager.h"
#include "TextureResourceManager.h"

namespace mray{
namespace scene{


SkyBoxManager::SkyBoxManager(){

}
SkyBoxManager::~SkyBoxManager(){
	m_textures.clear();
}

video::ITexturePtr  SkyBoxManager::loadSkyFromFolder(const core::string&path,const core::string&ext,const core::string&skyName){
	std::vector<core::string> files;

	files.push_back(path+"left."+ext);
	files.push_back(path+"right."+ext);
	files.push_back(path+"front."+ext);
	files.push_back(path+"back."+ext);
	files.push_back(path+"up."+ext);
	files.push_back(path+"bottom."+ext);

	video::ITexturePtr  skyTex=gTextureResourceManager.loadTextureCube(skyName,&files[0]);
	m_textures[skyName]=skyTex;
	return skyTex;

	/*
	core::string name;
	name=path+mT("left.")+ext;
	GCPtr<video::ImageInfo> left= gTextureResourceManager.loadImageInfo(name,video::ETT_2DTex);
	name=path+mT("right.")+ext;
	GCPtr<video::ImageInfo> right= gTextureResourceManager.loadImageInfo(name,video::ETT_2DTex);
	name=path+mT("front.")+ext;
	GCPtr<video::ImageInfo> front= gTextureResourceManager.loadImageInfo(name,video::ETT_2DTex);
	name=path+mT("back.")+ext;
	GCPtr<video::ImageInfo> back= gTextureResourceManager.loadImageInfo(name,video::ETT_2DTex);
	name=path+mT("up.")+ext;
	GCPtr<video::ImageInfo> up= gTextureResourceManager.loadImageInfo(name,video::ETT_2DTex);
	name=path+mT("bottom.")+ext;
	GCPtr<video::ImageInfo> bottom= gTextureResourceManager.loadImageInfo(name,video::ETT_2DTex);

	if(!left ||!right ||!front ||!back ||!bottom ||!up)
		return 0;
	video::ITexturePtr  skyTex= gTextureResourceManager.createEmptyTextureCube(skyName,false);
	const video::ImageInfo* info[6]={right,left,bottom,up,front,back};
	
	skyTex->loadSurfaces(info,6);

	m_textures[skyName]=skyTex;
	return skyTex;*/
}
video::ITexturePtr  SkyBoxManager::getTexture(const core::string&name){
	SkyboxMap::iterator it= m_textures.find(name);
	if(it!=m_textures.end())
		return it->second;
	return 0;
}

void SkyBoxManager::listResources(std::vector<core::string>&outList)
{
	SkyboxMap::iterator it= m_textures.begin();
	for (;it!=m_textures.end();++it)
	{
		outList.push_back(it->first);
	}
}


}
}

