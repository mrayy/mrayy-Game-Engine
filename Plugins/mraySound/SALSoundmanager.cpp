

#include "SALSoundManager.h"
#include "SALSound.h"
#include <LogManager.h>
#include <SceneManager.h>
#include <SCameraNode.h>
#include <math.h>
#include <IFileSystem.h>
#include <macros.h>
#include <TraceManager.h>

#include <SoundResourceManager.h>
/*
#include "IMP3Loader.h"
#include "IOGGLoader.h"
#include "IWaveLoader.h"
*/


namespace mray{
namespace sound{
	
core::string gBuffer=mT("");

SALSoundManager::SALSoundManager():m_listener(0),
m_attachToNode(0){

	traceFunction(eSound);
	//initilize OpenAL

	gLogManager.startSection(mT("OpenAL"));

	m_Device=alcOpenDevice(0); // select the "preferred device"
	if(!m_Device){
		checkForError(mT("SALSoundManager() - alcOpenDevice"));
		gLogManager.endSection(false);
		return;
	}
	m_Context=alcCreateContext(m_Device,0);
	checkForError(mT("SALSoundManager() - alcCreateContext"));
	alcMakeContextCurrent(m_Context);
	checkForError(mT("SALSoundManager() - alcMakeContextCurrent"));
	//Check for EAX 2.0 support

	bool bEAX=alIsExtensionPresent("EAX2.0");

	core::string helper;
	core::char_to_string(alGetString(AL_VENDOR),helper);
	gLogManager.log(mT("OpenAL Vendor is ")+helper,ELL_INFO);
	core::char_to_string(alGetString(AL_VERSION),helper);
	gLogManager.log(mT("OpenAL Version is ")+helper,ELL_INFO);
	core::char_to_string(alGetString(AL_RENDERER),helper);
	gLogManager.log(mT("OpenAL Renderer is ")+helper,ELL_INFO);
	core::char_to_string(alGetString(AL_EXTENSIONS),helper);
	gLogManager.log(mT("OpenAL Extensions is ")+helper,ELL_INFO);

	checkForError(mT("SALSoundManager() - Init"));
	gLogManager.endSection(true);
/*
	//create loaders
	loaders::ISoundLoader*loader;
	soundLoaders.push_back(loader=new  loaders::IWaveLoader);
	soundLoaders.push_back(loader=new  loaders::IOGGLoader);
	soundLoaders.push_back(loader=new  loaders::IMP3Loader);
*/

	
}

SALSoundManager::~SALSoundManager(){

	
	soundsList.clear();
/*
	
	sndDataFiles.clear();
	soundLoaders.clear();
*/
	alcMakeContextCurrent(0);
	alcDestroyContext(m_Context);
	alcCloseDevice(m_Device);
	


	
}


void SALSoundManager::checkForError(const mchar*info){
	/*int error = alGetError();

	if(error != AL_NO_ERROR)
		gLogManager.log(info,alGetString(error),ELL_ERROR);*/

	int error = alutGetError();
	if(error != ALUT_ERROR_NO_ERROR){
		core::string errMsg;
		core::char_to_string(alutGetErrorString(error),errMsg);
		gLogManager.log(info+errMsg,ELL_ERROR);
	}

}

void SALSoundManager::runSounds(float dt){
	
	traceFunction(eSound);

	math::vector3d vel=0;
	math::vector3d upVec;
	if(m_listener && m_attachToNode){
		m_listenerPos=m_listener->getAbsolutePosition();
		m_listenerDir=m_listener->getOrintation()*math::vector3d::ZAxis;//-(cam->target()-m_listenerPos);
		//m_listenerDir.Normalize();
		upVec=m_listener->getOrintation()*math::vector3d::YAxis;

		/*
		m_listenerDir.x=(float)math::sind(cam->rotate.y)*math::sind(cam->rotate.x);
		m_listenerDir.y=(float)math::cosd(cam->rotate.x);
		m_listenerDir.z=(float)math::cosd(cam->rotate.y)*math::sind(cam->rotate.x);*/
	}else{
		m_listenerDir.x=0;
		m_listenerDir.y=0;
		m_listenerDir.z=1;
		upVec.y=1;

	}


	vel=(m_listenerPos-lastPos)*dt;
	lastPos=m_listenerPos;


	int channelsPlaying;
//	static int lastPlaying=0;
	//FATAL_ERROR(channelsPlaying>20,"Max Channels Reach!"));
/*
	if(lastPlaying!=channelsPlaying)
	{
		static char buffer[32];
		sprintf_s(buffer,32,"%d"),channelsPlaying);
        gLogManager.log(buffer, ELL_INFO);
		lastPlaying=channelsPlaying;
	}
*/


	float dir[6];
	dir[0]=m_listenerDir.x;
	dir[1]=m_listenerDir.y;
	dir[2]=m_listenerDir.z;
	dir[3]=upVec.x;
	dir[4]=upVec.y;
	dir[5]=upVec.z;
	alListener3f(AL_POSITION,m_listenerPos.x,m_listenerPos.y,m_listenerPos.z);
	checkForError(mT("SALSoundManager::runSounds() - alListener3f(AL_POSITION)"));
	alListenerfv(AL_ORIENTATION,dir);
	checkForError(mT("SALSoundManager::runSounds() - alListenerfv(AL_ORIENTATION)"));

	//update sounds
	std::list<GCPtr<ISound>>::iterator it=soundsList.begin();
	std::list<GCPtr<ISound>>::iterator it2;
	for(;it!=soundsList.end();){
		bool remove=0;
		ISound* snd=(*it);
		if(snd->isAttachedToListener())
			snd->setPosition(m_listenerPos);
			
		snd->update(dt);

		if(!snd->isPlaying() && snd->isDieAfterEnd()){
			remove=1;
		}
		if(remove){
			it2=it+1;
			soundsList.remove(it);
			it=it2;
		}else
			++it;
	}

	
}

const mchar*SALSoundManager::getALVersion()const{
	core::char_to_string(alGetString(AL_VERSION),gBuffer);
	return gBuffer.c_str();
}
const mchar*SALSoundManager::getALVendor()const{
	core::char_to_string(alGetString(AL_VENDOR),gBuffer);
	return gBuffer.c_str();
}
const mchar*SALSoundManager::getALRenderer()const{
	core::char_to_string(alGetString(AL_RENDERER),gBuffer);
	return gBuffer.c_str();
}
const mchar*SALSoundManager::getALExtensions()const{
	core::char_to_string(alGetString(AL_EXTENSIONS),gBuffer);
	return gBuffer.c_str();
}

void SALSoundManager::attachToListener(scene::IMovable*node){
	m_listener=node;
}
scene::IMovable* SALSoundManager::getListenerNode(){
	return m_listener;
}
/*
GCPtr<ISoundStreamFrame>SALSoundManager::getSound(const  mchar *filename){
	for(int i=0;i<sndDataFiles.size();++i){
		if(sndDataFiles[i]->sndFileName.equals_ignore_case(filename))
			return sndDataFiles[i];
	}
	return 0;
}*/

GCPtr<ISound>  SALSoundManager::playSound(const  mchar *sndName,math::vector3d pos,bool isStream,float vol,bool loop,ESoundType type){
	GCPtr<ISound> snd=loadSound(sndName,isStream,type);
	if(!snd)return 0;
	snd->setPosition(pos);
	snd->setVolume(vol);
	snd->setLooping(loop);
	snd->play();
	snd->setDieAfterEnd(1);
	return snd;
}
 

GCPtr<ISound> SALSoundManager::loadSound(const  mchar *filename,bool isStream,ESoundType type){
	traceFunction(eSound);
	GCPtr<ISound> snd=0;

	GCPtr<ISoundStreamFrame>dataFile=gSoundResourceManager.loadSound(filename);

	if(dataFile){
		snd=new  SALSound(this,filename);
		snd->setData(dataFile);
		addSound(snd);
		
		return snd;
	}
	
	return 0;
}

void SALSoundManager::addSound(GCPtr<ISound>snd){
	soundsList.push_back(snd);
}
void SALSoundManager::removeSound(GCPtr<ISound>snd){
	std::list<GCPtr<ISound>>::iterator it=soundsList.begin();
	for(;it!=soundsList.end();++it)
		if((*it)==snd){
			soundsList.remove(it);
			return;
		}
}
void SALSoundManager::stopAllSounds(){
	std::list<GCPtr<ISound>>::iterator it=soundsList.begin();
	for(;it!=soundsList.end();++it){
		(*it)->stop();
	}
}

void SALSoundManager:: playAllSounds(){

}
void SALSoundManager:: pauseAllSounds(){

}
void SALSoundManager:: resumeAllSounds(){

}


void SALSoundManager::setDistanceFactor(float factor){
	distance_factor=factor;
}


GCPtr<ISoundManager> OpenALManagerCreater::create(){
	return new SALSoundManager();
}

bool OpenALManagerCreater::canCreate(const mchar*type){
	core::string s(type);
	return s.equals_ignore_case(getType());
}
const mchar* OpenALManagerCreater::getType(){
	return mT("OpenAL");
}

const mchar* OpenALManagerCreater::getDescription(){
	return mT("OpenAL Sound Manager");
}

}
}