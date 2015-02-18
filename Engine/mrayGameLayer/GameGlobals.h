

/********************************************************************
	created:	2010/06/26
	created:	26:6:2010   20:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\GameGlobals.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	GameGlobals
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameGlobals_h__
#define GameGlobals_h__

#include "CompileConfig.h"
#include "ISingleton.h"
#include "XMLElement.h"
#include "marray.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL GameGlobals
{
private:
protected:
	std::vector<core::string> m_searchPath;
	std::vector<core::string> m_requiredPlugins;

	std::vector<int> m_installedPlugins;
public:
	GameGlobals();
	virtual ~GameGlobals();
	
	void AddRequiredPlugin(const core::string&plugin);
	void AddSearchPath(const core::string&path);

	void SetupSearchPaths();

	void InstallPlugins();
	void UninstallPlugins();

	void ClearPlugins();
	void ClearSearchPaths();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // GameGlobals_h__
