
#include "GameLevelManager.h"
#include "XMLLevelLoader.h"
#include <CommandManager.h>
#include <IFileSystem.h>


namespace mray{
namespace gameMod{


	class CToggleGameCamera:public ICommand
	{
		bool m_enabled;
		core::string m_msg;
	public:
		CToggleGameCamera(){
			m_enabled=true;
		}

		virtual const mchar* getCommandName(){
			return mT("camera");
		}

		virtual const mchar* getCommandInfo(){
			return mT("Toggle Between Cameras");
		}

		virtual int getCommandMinArgCount(){return 0;}

		virtual int getCommandMaxArgCount(){return 0;}

		virtual bool onCommand(core::array<core::string>& args){
			GCPtr<GameLevel> level= GameLevelManager::getInstance().getCurrentLevel();
			if(!level){
				m_msg=mT("No Active Game set!");
				return false;
			}
			m_msg=mT("");
			level->toggleCamera();
			return true;
		}

		virtual const mchar* getLastMessage(){return m_msg.c_str();}

		virtual bool isEnabled(){
			return m_enabled;
		}
		virtual void enable(bool e){
			m_enabled=e;
		}
	};

	//////////////////////////////////////////////////////////////////////////

GameLevelManager::GameLevelManager(){
	CommandManager::getInstance().addCommand(new CToggleGameCamera());
}
GameLevelManager::~GameLevelManager(){
	LevelMap::iterator it=m_maps.begin();
	for (;it!=m_maps.end();++it)
	{
		it->second=0;
	}
	m_maps.clear();
}

GCPtr<GameLevel> GameLevelManager::loadCurrentLevel(IResLoadListener*listner){

	LevelMap::iterator it= m_maps.find(m_activeLevel);
	if(it==m_maps.end())
		return 0;
	m_loadedLevel=new GameLevel();
	it->second->loadLevel(m_loadedLevel,listner);
	return m_loadedLevel;
}
const core::string& GameLevelManager::getCurrentLevelName(){
	return m_activeLevel;
}

void GameLevelManager::setCurrentLevel(const core::string&name){
	m_activeLevel=name;
}
void GameLevelManager::addLevel(const core::string&path){
	GCPtr<XMLLevelLoader> loader=new XMLLevelLoader();
	bool res=loader->loadInfo(gFileSystem.openFile(path));
	if(res){
		loader->getInfo()->path=path;
		m_maps[loader->getInfo()->name]=loader;
	}
}

}
}
