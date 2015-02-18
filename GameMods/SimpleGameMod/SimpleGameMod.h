


/********************************************************************
	created:	2010/06/24
	created:	24:6:2010   17:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\gameMods\SimpleGameMod\SimpleGameMod.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\gameMods\SimpleGameMod
	file base:	SimpleGameMod
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SimpleGameMod_h__
#define SimpleGameMod_h__

#include "CompileConfig.h"
#include <IPlugin.h>

namespace mray
{
namespace game
{

class SIMPLEGAMEMOD_API SimpleGameMod:public IPlugin
{
private:
protected:
public:
	SimpleGameMod();
	virtual ~SimpleGameMod();

	virtual const mchar*getName();

	virtual void install();
	virtual void uninstall();
};

}
}


#endif // SimpleGameMod_h__
