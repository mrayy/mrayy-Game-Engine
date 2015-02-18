
/********************************************************************
	created:	2009/04/08
	created:	8:4:2009   19:26
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\CSoundthemeManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	CSoundthemeManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CSoundthemeManager___
#define ___CSoundthemeManager___

#include <ISingleton.h>

namespace mray{
namespace gameMod{

class CSoundthemeManager:public ISingleton<CSoundthemeManager>
{
public:
	struct STheme{
		core::string themeName;
		core::array<core::string> m_sounds;
	};
protected:

	core::array<STheme> m_themes;
	core::string m_activeTheme;

	float m_BackgroundVolume;
	float m_GameVolume;
public:
	CSoundthemeManager();
	virtual ~CSoundthemeManager();

	void loadThemesFromDirs(const core::string& dir);

	void addTheme(const core::string& themeName);
	void addSoundToTheme(const core::string& sndName,const core::string& themeName);

	STheme* getThemeByName(const core::string& themeName);
	STheme* getThemeByIndex(int index);
	int getThemeIndex(const core::string& themeName);
	int getThemesCount();

	void setActiveTheme(const core::string&theme);
	const core::string& getActiveTheme();

	const mchar*getRandomSoundName();
	const mchar*getRandomSoundName(const core::string& themeName);

	void setBGVolume(float v);
	float getBGVolume();

	void setGameVolume(float v);
	float getGameVolume();

};

}
}

#endif


