

/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   13:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\IGameLevelAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	IGameLevelAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGameLevelAction_h__
#define IGameLevelAction_h__

#include "IGameAction.h"

namespace mray
{
namespace game
{
	class GameLevel;

class IGameLevelAction:public IGameAction
{
private:
protected:
public:
	IGameLevelAction()
	{}
	virtual ~IGameLevelAction(){}

	virtual void Init(GameLevel*level)=0;

	virtual void SetEditorMode(bool e)=0;
	
};

}
}


#endif // IGameLevelAction_h__
