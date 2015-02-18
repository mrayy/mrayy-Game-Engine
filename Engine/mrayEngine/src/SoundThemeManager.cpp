#include "stdafx.h"

#include "SoundThemeManager.h"
#include "IDirOS.h"
#include "IOSystem.h"
#include "IFileSystem.h"




namespace mray{
namespace sound{

SoundThemeManager::SoundThemeManager(){
	m_BackgroundVolume=1;
	m_GameVolume=1;
}

SoundThemeManager::~SoundThemeManager(){
	for(int i=0;i<m_themes.size();++i){
/*		for(int j=0;j<m_themes[i].m_sounds.size();j++){
			SaveRelease(m_themes[i].m_sounds[j]);
		}*/
		m_themes[i].m_sounds.clear();
	}
	m_themes.clear();
}

void SoundThemeManager::loadThemesFromDirs(const core::string& dir){

	GCPtr<OS::IDirOS>fos=gOSystem.createDirSystem();
	core::string path=gFileSystem.getAppPath();
	fos->changeDir(path+dir);
	//fos->changeDir(dir.c_str());

	int cnt=fos->getFilesCount();
	core::string fname,theme;

	for(int i=0;i<cnt;++i){
		if(fos->isFileDirectory(i)){
			theme=fos->getShortFileName(i);
			if(theme.find('.')==0)continue;
			addTheme(theme.c_str());
			fos->changeDir(fos->getShortFileName(i));
			for(int j=0;j<fos->getFilesCount();j++){
				fname=fos->getShortFileName(j);
				if(fname.find(".mp3")!=-1){
					fname=fos->getFullFileName(j);
					addSoundToTheme(fname.c_str(),theme.c_str());
				}
				
			}
			fos->changeDir(mT(".."));
		}
	}

	if(m_activeTheme==mT("") && m_themes.size()){
		m_activeTheme=m_themes[0].themeName;
	}
	fos->changeDir(path.c_str());
}


SoundThemeManager::STheme* SoundThemeManager::getThemeByName(const core::string& themeName){
	int i=getThemeIndex(themeName);
	if(i!=-1)return &m_themes[i];
	return 0;
}

SoundThemeManager::STheme* SoundThemeManager::getThemeByIndex(int index){
	if(index<0 || index>=m_themes.size())return 0;
	return &m_themes[index];
}

int SoundThemeManager::getThemeIndex(const core::string& themeName){
	for(int i=0;i<m_themes.size();++i){
		if(m_themes[i].themeName.equals_ignore_case(themeName))
			return i;
	}
	return -1;
}
int SoundThemeManager::getThemesCount(){
	return m_themes.size();
}

void SoundThemeManager::addTheme(const core::string& themeName){
	if(getThemeByName(themeName))return;
	STheme t;
	t.themeName=themeName;
	m_themes.push_back(t);
}

void SoundThemeManager::addSoundToTheme(const core::string& sndName,const core::string& themeName){
	STheme*t=getThemeByName(themeName);
	if(!t)return;
	t->m_sounds.push_back(sndName);
}

core::string SoundThemeManager::getRandomSoundName(const core::string& themeName){
	STheme*t=getThemeByName(themeName);
	if(!t)return mT("");
	int cnt=t->m_sounds.size();
	if(cnt==0)return mT("");

	return t->m_sounds[math::Randomizer::rand(cnt)].c_str();
}

void SoundThemeManager::setActiveTheme(const core::string&theme){
	m_activeTheme=theme;
}
const core::string& SoundThemeManager::getActiveTheme(){
	return m_activeTheme;
}


core::string SoundThemeManager::getRandomSoundName(){
	return getRandomSoundName(m_activeTheme);
}


void SoundThemeManager::setBGVolume(float v){
	m_BackgroundVolume=v;
}
float SoundThemeManager::getBGVolume(){
	return m_BackgroundVolume;
}

void SoundThemeManager::setGameVolume(float v){
	m_GameVolume=v;
}
float SoundThemeManager::getGameVolume(){
	return m_GameVolume;
}

}
}

