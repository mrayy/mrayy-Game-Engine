
/********************************************************************
	created:	2009/04/09
	created:	9:4:2009   0:01
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\GameLevelManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	GameLevelManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GameLevelManager___
#define ___GameLevelManager___

#include <ISingleton.h>
#include "GameLevel.h"
#include "XMLLevelLoader.h"
#include "IResLoadListener.h"

namespace mray{
namespace gameMod{

	class IGameLevelLoadListener
	{
	public:
		virtual void onLoadDone(GCPtr<GameLevel> level)=0;
	};

class GameLevelManager:public ISingleton<GameLevelManager>
{
private:
protected:
	typedef std::map<core::string,GCPtr<XMLLevelLoader>> LevelMap;

	LevelMap m_maps;

	core::string m_activeLevel;

	IResLoadListener*m_resList;
	IResLoadListener*m_levList;

	GCPtr<GameLevel> m_loadedLevel;
public:
	GameLevelManager();
	virtual~GameLevelManager();

	bool loadCurrentLevelMultiThread(IResLoadListener*listner,IGameLevelLoadListener*levList);

	GCPtr<GameLevel> loadCurrentLevel(IResLoadListener*listner);
	const core::string& getCurrentLevelName();

	GCPtr<GameLevel> getCurrentLevel(){
		return m_loadedLevel;
	}

	void setCurrentLevel(const core::string&name);
	void addLevel(const core::string&path);
};

}
}


#endif //___GameLevelManager___
