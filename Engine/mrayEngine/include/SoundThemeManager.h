
/********************************************************************
	created:	2009/04/08
	created:	8:4:2009   19:26
	filename: 	i:\Programing\GameEngine\mrayEngine\src\SoundThemeManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\src
	file base:	SoundThemeManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SoundThemeManager___
#define ___SoundThemeManager___

#include "mstring.h"


namespace mray{
namespace sound{

class MRAY_DLL SoundThemeManager
{
public:
	struct STheme{
		core::string themeName;
		std::vector<core::string> m_sounds;
	};
protected:

	std::vector<STheme> m_themes;
	core::string m_activeTheme;

	float m_BackgroundVolume;
	float m_GameVolume;
public:
	SoundThemeManager();
	virtual ~SoundThemeManager();

	void loadThemesFromDirs(const core::string& dir);

	void addTheme(const core::string& themeName);
	void addSoundToTheme(const core::string& sndName,const core::string& themeName);

	STheme* getThemeByName(const core::string& themeName);
	STheme* getThemeByIndex(int index);
	int getThemeIndex(const core::string& themeName);
	int getThemesCount();

	void setActiveTheme(const core::string&theme);
	const core::string& getActiveTheme();

	core::string getRandomSoundName();
	core::string getRandomSoundName(const core::string& themeName);

	void setBGVolume(float v);
	float getBGVolume();

	void setGameVolume(float v);
	float getGameVolume();

};

}
}

#endif


