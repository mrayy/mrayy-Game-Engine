
/********************************************************************
	created:	2009/05/02
	created:	2:5:2009   20:10
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\GameOptionManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	GameOptionManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GameOptionManager___
#define ___GameOptionManager___

#include <ISingleton.h>

#include "TextureManager.h"

namespace mray{
namespace gameMod{

class GameOptionManager:public ISingleton<GameOptionManager>
{
private:
protected:
	core::string m_confingFileName;
public:
	GameOptionManager(const core::string&confingFileName);
	virtual~GameOptionManager();

	virtual void exportSettings(const core::string&confingFileName);

public:

	ETextureQuality textureQuality;
	float viewDistance;
	bool postEffect;
	
	float gameSoundVol;
};

}
}


#endif //___GameOptionManager___
