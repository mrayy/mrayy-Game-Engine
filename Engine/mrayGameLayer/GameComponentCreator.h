


/********************************************************************
	created:	2012/02/27
	created:	27:2:2012   9:19
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\GameComponentCreator.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	GameComponentCreator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GameComponentCreator__
#define __GameComponentCreator__

#include "compileConfig.h"
#include "mstring.h"
#include "ISingleton.h"
#include <map>

namespace mray
{
namespace game
{
class IGameComponentFactory;
class IGameComponent;
class GameEntityManager;

class MRAY_GAME_DLL GameComponentCreator:public ISingleton<GameComponentCreator>
{
private:
	struct FactoryInfo
	{
		FactoryInfo():enabled(true),factory(0)
		{
		}
		FactoryInfo(IGameComponentFactory* f):enabled(true),factory(f)
		{}
		bool enabled;
		IGameComponentFactory* factory;
	};
protected:
	static GameComponentCreator s_instance;

	typedef std::map<core::string,int> AliasMap;
	AliasMap m_aliasMap;
	typedef std::map<int,FactoryInfo> FactoryMap;
	FactoryMap m_factories;

	FactoryInfo* _GetFactoryInfo(const core::string&name);
public:
	GameComponentCreator();
	virtual~GameComponentCreator();

	void RegisterFactory(IGameComponentFactory* factory);
	void UnregisterFactory(const core::string&name);
	IGameComponentFactory* GetFactory(const core::string&name,bool shouldEnabled=true);
	void ClearFactories();

	void SetFactoryEnabled(const core::string& name, bool e);
	bool IsFactoryEnabled(const core::string& name);

	bool AddAlias(const core::string& factory,const core::string& alias);
	void RemoveAlias(const core::string& alias);

	IGameComponent* CreateObject(const core::string&name,GameEntityManager*mngr);

};

#define REGISTER_COMPONENT_FACTORY(CompName)\
	game::GameComponentCreator::getInstance().RegisterFactory(&GameComponentFactory##CompName::Instance);

#define UNREGISTER_COMPONENT_FACTORY(CompName)\
	game::GameComponentCreator::getInstance().UnregisterFactory(GameComponentFactory##CompName::Instance.GetType());

}
}

#endif
