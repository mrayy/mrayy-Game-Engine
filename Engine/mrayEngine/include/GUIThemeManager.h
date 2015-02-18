
#ifndef ___GUIThemeManager___
#define ___GUIThemeManager___



#include "mString.h"
#include "ISingleton.h"


namespace mray{
	namespace OS
	{
		class IStream;
	}
namespace GUI
{

	class IGUITheme;

class MRAY_DLL GUIThemeManager:public ISingleton<GUIThemeManager>
{
	typedef std::map<core::string,IGUITheme*> ThemesMap;
	ThemesMap m_Themes;
	IGUITheme* m_activeTheme;
public:

	GUIThemeManager();
	virtual~GUIThemeManager();

	IGUITheme* getActiveTheme();

	bool setActiveTheme(const core::string &type);
	IGUITheme* loadTheme(OS::IStream* stream);
	void addTheme(IGUITheme* Theme);
	IGUITheme* getTheme(const core::string &type);
	void removeTheme(const core::string &type);
	void clearThemes();

	virtual void listThemes(std::vector<core::string>&outList);

};


}
}




#endif


