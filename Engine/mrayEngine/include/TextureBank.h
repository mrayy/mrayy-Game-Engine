
#ifndef ___TextureBank___
#define ___TextureBank___


#include "ITexture.h"

#include "GCPtr.h"


namespace mray{
namespace video{

class MRAY_DLL TextureBank
{
	std::vector<ITexturePtr > textures;
public:
	TextureBank();
	virtual~TextureBank();

	void addTexture(const ITexturePtr& tex);
	void insertTexture(const ITexturePtr& tex,int index);
	void replaceTexture(const ITexturePtr& tex,int index);
	void deleteTexture(int index);
	const ITexturePtr&  getTexture(int index);
	int getCount(){return textures.size();}
};

MakeSharedPtrType(TextureBank)

};//video
};//mray

#endif






