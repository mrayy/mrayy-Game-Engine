

/********************************************************************
	created:	2010/06/30
	created:	30:6:2010   19:46
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\GameLevelManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	GameLevelManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameLevelManager_h__
#define GameLevelManager_h__

#include "ISingleton.h"
#include "GameLevel.h"

namespace mray
{
namespace game
{


class MRAY_GAME_DLL GameLevelManager:public ISingleton<GameLevelManager>
{
private:
protected:
	typedef std::map<core::string,GCPtr<GameLevel>> LevelMap;
	LevelMap m_levels;

	std::vector<GameLevelInfo> m_gameLevelsInfo;

	GCPtr<GameLevel> m_activeLevel;

	void LoadInfo(const core::string&path);

public:
	GameLevelManager();
	virtual ~GameLevelManager();

	void LoadLevelsFile(const core::string& path);
	std::vector<GameLevelInfo>& GetLoadedLevelsInfo();
	void ClearLoadedLevelsInfo();

	void AddLevel(GCPtr<GameLevel> level);
	void RemoveLevel(GCPtr<GameLevel> level);

	void SetActiveLevel(GCPtr<GameLevel> level);
	GCPtr<GameLevel> GetActiveLevel();
	
};

}
}

#endif // GameLevelManager_h__
