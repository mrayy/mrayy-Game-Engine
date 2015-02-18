#include "stdafx.h"

#include "TextureBank.h"

namespace mray{
namespace video{

TextureBank::TextureBank()
{
#ifdef ___DEBUG___
	setDebugName("TextureBank");
#endif
}
TextureBank::~TextureBank()
{
	textures.clear();
}

void TextureBank::addTexture(const ITexturePtr& tex){
	textures.push_back(tex);
}

void TextureBank::insertTexture(const ITexturePtr& tex,int index)
{
	while(index>=textures.size())
		textures.push_back(0);

	textures[index]=tex;
}

void TextureBank::replaceTexture(const ITexturePtr& tex,int index){
	if(index>=0 && index <textures.size()){
		textures[index]=tex;
	}
}
void TextureBank::deleteTexture(int index){
	if(index>=0 && index <textures.size()){
		if(textures[index]){
			textures[index]=0;
		}
	}
}
const ITexturePtr&  TextureBank::getTexture(int index){
	if(index>=0 && index <textures.size())
		return textures[index];
	else
		return ITexturePtr::Null;
}




}
}
