
#include "TextureManager.h"
#include "TextureResourceManager.h"
#include "GameOptionManager.h"

namespace mray{
namespace gameMod{


TextureManager::TextureManager(){
}
TextureManager::~TextureManager(){
}


GCPtr<video::ITexture> TextureManager::loadTexture2D(const core::string &name){
	core::string searchPath;
	if(GameOptionManager::getInstance().textureQuality==ETQ_High){
		searchPath=mT("High\\");
	}
	else 
		searchPath=mT("Low\\");

	searchPath+=name;

	return gTextureResourceManager.loadTexture2D(searchPath);
}


}
}