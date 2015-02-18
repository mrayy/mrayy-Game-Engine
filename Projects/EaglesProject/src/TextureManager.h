
/********************************************************************
	created:	2009/04/16
	created:	16:4:2009   16:48
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\include\TextureManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\include
	file base:	TextureManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TextureManager___
#define ___TextureManager___

#include <ISingleton.h>
#include <ITexture.h>

namespace mray{
namespace gameMod{

enum ETextureQuality
{
	ETQ_High,
	ETQ_Low
};

class TextureManager:public ISingleton<TextureManager>
{
private:
protected:
public:
	TextureManager();
	virtual~TextureManager();

	GCPtr<video::ITexture> loadTexture2D(const core::string &name);
};

}
}

#endif //___TextureManager___
