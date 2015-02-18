
#include "CSoundthemeManager.h"
#include <IDirOS.h>
#include <IOSystem.h>
#include <IFileSystem.h>
#include <Randomizer.h>



namespace mray{
namespace gameMod{

CSoundthemeManager::CSoundthemeManager(){
	m_BackgroundVolume=1;
	m_GameVolume=1;
}

CSoundthemeManager::~CSoundthemeManager(){
	for(int i=0;i<m_themes.size();++i){
/*		for(int j=0;j<m_themes[i].m_sounds.size();j++){
			SaveRelease(m_themes[i].m_sounds[j]);
		}*/
		m_themes[i].m_sounds.clear();
	}
	m_themes.clear();
}

void CSoundthemeManager::loadThemesFromDirs(const core::string& dir){

	GCPtr<OS::IDirOS>fos=gOSystem.createDirSystem();
	core::string path=gFileSystem.getAppPath();
	fos->changeDir(path.c_str());
	fos->changeDir(dir.c_str());

	int cnt=fos->getFilesCount();
	core::string fname,theme;

	for(int i=0;i<cnt;++i){
		if(fos->isFileDirectory(i)){
			theme=fos->getShortFileName(i);
			if(theme.findFirst('.')==0)continue;
			addTheme(theme.c_str());
			fos->changeDir(fos->getShortFileName(i));
			for(int j=0;j<fos->getFilesCount();j++){
				fname=fos->getShortFileName(j);
				if(fname.find(".mp3")!=-1){
					fname=fos->getFullFileName(j);
					addSoundToTheme(fname.c_str(),theme.c_str());
				}
				
			}
			fos->changeDir(L"..");
		}
	}

	if(m_activeTheme==mT("") && m_themes.size()){
		m_activeTheme=m_themes[0].themeName;
	}
	fos->changeDir(path.c_str());
}


CSoundthemeManager::STheme* CSoundthemeManager::getThemeByName(const core::string& themeName){
	int i=getThemeIndex(themeName);
	if(i!=-1)return &m_themes[i];
	return 0;
}

CSoundthemeManager::STheme* CSoundthemeManager::getThemeByIndex(int index){
	if(index<0 || index>=m_themes.size())return 0;
	return &m_themes[index];
}

int CSoundthemeManager::getThemeIndex(const core::string& themeName){
	for(int i=0;i<m_themes.size();++i){
		if(m_themes[i].themeName.equals_ignore_case(themeName))
			return i;
	}
	return -1;
}
int CSoundthemeManager::getThemesCount(){
	return m_themes.size();
}

void CSoundthemeManager::addTheme(const core::string& themeName){
	if(getThemeByName(themeName))return;
	STheme t;
	t.themeName=themeName;
	m_themes.push_back(t);
}

void CSoundthemeManager::addSoundToTheme(const core::string& sndName,const core::string& themeName){
	STheme*t=getThemeByName(themeName);
	if(!t)return;
	t->m_sounds.push_back(sndName);
}

const mchar*CSoundthemeManager::getRandomSoundName(const core::string& themeName){
	STheme*t=getThemeByName(themeName);
	if(!t)return mT("");
	int cnt=t->m_sounds.size();
	if(cnt==0)return mT("");

	return t->m_sounds[math::Randomizer::rand(cnt)].c_str();
}

void CSoundthemeManager::setActiveTheme(const core::string&theme){
	m_activeTheme=theme;
}
const core::string& CSoundthemeManager::getActiveTheme(){
	return m_activeTheme;
}


const mchar* CSoundthemeManager::getRandomSoundName(){
	return getRandomSoundName(m_activeTheme);
}


void CSoundthemeManager::setBGVolume(float v){
	m_BackgroundVolume=v;
}
float CSoundthemeManager::getBGVolume(){
	return m_BackgroundVolume;
}

void CSoundthemeManager::setGameVolume(float v){
	m_GameVolume=v;
}
float CSoundthemeManager::getGameVolume(){
	return m_GameVolume;
}

}
}

