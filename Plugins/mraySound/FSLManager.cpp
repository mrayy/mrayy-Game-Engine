#include "stdafx.h"
#include "IFileSystem.h"
#include "SoundResourceManager.h"

#include "FSLManager.h"
#include "FSLSound.h"
#include "FSLSoundBuffer.h"
#include "ISoundListener.h"
#include "FSLListener.h"
#include "SoundLoggerSystem.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "IThreadFunction.h"

#include <FreeSL.h>

namespace mray{
namespace sound{

class FSLUpdateFunction:public OS::IThreadFunction
{
public:
	virtual void setup()
	{
	}

	virtual void execute(OS::IThread*caller,void*arg)
	{
		while(caller->isActive())
		{
			fslUpdate();
			OS::IThreadManager::getInstance().sleep(10);
		}
	}
};


FSLManager::FSLManager()
{
	m_listener=0;

	m_streamingFunc=new FSLUpdateFunction();
	m_streamingThread=OS::IThreadManager::getInstance().createThread(m_streamingFunc);

	gSoundLoggerSystem.log(mT("Creating FreeSL sound system"),ELL_INFO);
	if(!fslInit(FSL_SS_EAX2 ))
	{
		gSoundLoggerSystem.log(mT("Faild to create FreeSL sound system using EAX2, trying ALUT"),ELL_WARNING);
		if (!fslInit(FSL_SS_ALUT))
			gSoundLoggerSystem.log(mT("Faild to create FreeSL sound system using ALUT"),ELL_WARNING);
	}

	int maxSources=fslGetMaxSources();

	gSoundLoggerSystem.log(mT("Max audio sources:")+core::StringConverter::toString(maxSources),ELL_INFO,EVL_Normal);

	fslInitSourceManager(maxSources);

	m_streamingThread->start(0);
}
FSLManager::~FSLManager()
{
	gSoundLoggerSystem.log(mT("Shutting down FreeSL sound system"),ELL_INFO);
	Clear();

	OS::IThreadManager::getInstance().killThread(m_streamingThread);
	delete m_streamingThread;
	delete m_streamingFunc;

	fslShutDown();
	//delete m_listener;

}

ISoundPtr FSLManager::CreateManualSound(const core::string&name)
{
/*	if(!stream)
		return 0;
	
	int chCount= stream->GetChannelsCount();
	ESampleFormat fmt=stream->GetBitsPerSample();
	FSLenum format;
	if(chCount==1)
	{
		if(fmt==ESF_8)
			format=FSL_FORMAT_MONO8;
		else
			format=FSL_FORMAT_MONO16;
	}else if(chCount==2)
	{
		if(fmt==ESF_8)
			format=FSL_FORMAT_STEREO8;
		else
			format=FSL_FORMAT_STEREO16;
	}else
		return 0;
	FSLbuffer buffer =fslCreateBuffer();
	fslSetBufferData(buffer,format,stream->getData(),stream->getDataSize(),stream->GetSamplesPerSec());*/

	//FSLbuffer buffer=dynamic_cast<FSLSoundBuffer*>(stream)->getBuffer();
	ISoundPtr ret= new FSLSound(this,name,fslCreateEmptySound());

	addSound(ret);

	return ret;
}


ISoundStreamFrame* FSLManager::CreateManualStream()
{
	return new FSLSoundBuffer();
}

ISoundPtr FSLManager::playSound(const core::string&sndName,const math::vector3d& pos,bool isStream,float vol,bool loop,ESoundType type){

	ISoundPtr ret=loadSound(sndName,isStream,type);
	FSLSound* snd=dynamic_cast<FSLSound*>(ret.pointer());
	if(!snd)
		return 0;

	snd->setPosition(pos);
	snd->setVolume(vol);
	snd->setLooping(loop);
	snd->play();
	snd->setDieAfterEnd(1);
	return ret;
}

ISoundStreamFrame* FSLManager::getSound(const core::string&filename)
{
	core::stringc fileStr;
	core::string_to_char(filename,fileStr);
	GUID fileID(fileStr);
	SoundStreamMap::iterator it= m_sndDataFiles.find(fileID.ID());
	if(it==m_sndDataFiles.end())return 0;
	return it->second;
}

ISoundPtr FSLManager::loadSound(const core::string&filename,bool isStream,ESoundType type){



	if(!isStream){
		FSLSoundBuffer* dataFile=dynamic_cast<FSLSoundBuffer*>( getOrCreate(filename,isStream,type));
		if(dataFile){
			FSLsound fsnd=fslCreateEmptySound();
			FSLSound* snd=new FSLSound(this,filename,fsnd);
			snd->setSoundBuffer(dataFile->duplicate());
			addSound(snd);

			return snd;
		}
	}else
	{
		core::stringc fname;
		core::string str;
		gFileSystem.getCorrectFilePath(filename,str);
		core::string_to_char(str,fname);
		FSLsound fsnd=fslStreamSound(fname.c_str());
		if(!fsnd)return 0;
		FSLSound* snd=new FSLSound(this,filename,fsnd);
		addSound(snd);
		return snd;
	}
	return 0;
}
ISoundStreamFrame* FSLManager::getOrCreate(const core::string&filename,bool streaming,ESoundType type)
{
	core::string str;
	gFileSystem.getCorrectFilePath(filename,str);


	FSLSoundBuffer* dataFile=dynamic_cast<FSLSoundBuffer*>(getSound(filename.c_str()));
	if(!dataFile){
		core::stringc fname;
		core::string_to_char(str,fname);
		FSLbuffer buff;
		buff=fslLoadBuffer(fname.c_str());
		if(!buff){
			gSoundLoggerSystem.log(mT("Couldn't load sound file :")+filename,ELL_INFO,EVL_Heavy);
			return 0;
		}
		dataFile=new FSLSoundBuffer();
		dataFile->fileName=filename;
		dataFile->setBuffer(buff);
		SoundResourceManager::getInstance().addResource(dataFile,filename);
	}
	return dataFile;
}

ISoundListener* FSLManager::createListener()
{
	return new FSLListener();
}
void FSLManager::setActiveListener(ISoundListener* l)
{
	m_listener=l;
}
ISoundListener* FSLManager::getActiveListener()
{
	return m_listener;
}

void FSLManager::setVolume(float v){
	fslSetVolume(v);
}

int FSLManager::getCurrentSoundsStreaming(){
	return fslCountSoundsStreaming();
}

int FSLManager::getMaxSounds(){
	return fslCountSoundsTotal();
}

ulong FSLManager::getUsedMemory(){
	return fslGetSoundMemoryUsage();
}


void FSLManager::addSound(ISoundCRef snd){
	m_sounds.push_back(snd);
}
void FSLManager::removeSound(ISound*snd){
	SoundsList::iterator it=m_sounds.begin();
	for(;it!=m_sounds.end();++it)
	{
		if((*it).pointer()==snd)
		{
			m_sounds.erase(it);
			return;
		}
	}
}

void FSLManager::setDistanceFactor(float factor){
}
float FSLManager::getDistanceFactor(){
	return 1;
}

void FSLManager::runSounds(float dt){
	SoundsList::iterator p=m_sounds.begin();
	SoundsList::iterator end=m_sounds.end();

	for(;p!=end;++p)
	{
		(*p)->update(dt);
	}
	if(m_listener)
		m_listener->update(dt);
	
}


void FSLManager::setDopplerParameters(float factor,float velocity){
	fslSetDopplerParameters(factor,velocity);
}
void FSLManager::setSpeedOfSound(float v){
	fslSetSpeedOfSound(v);
}
void FSLManager::setMetersPerUnit(float meters){
	fslSetMetersPerUnit(meters);
}


void FSLManager::stopAllSounds()
{
	fslSoundStopAllSounds();
}
void FSLManager::playAllSounds()
{
}
void FSLManager::pauseAllSounds()
{
	fslSoundPauseAllSounds();
}
void FSLManager::resumeAllSounds()
{
	fslSoundUnPauseAllSounds();
}
void FSLManager::Clear()
{
	//SoundsList::iterator it=m_sounds.begin();
	//for(;it!=m_sounds.end();++it)
	//	delete *it;
	m_sounds.clear();
}




const core::string FSLManagerCreater::Type=mT("FreeSL");

const core::string& FSLManagerCreater::GetType()
{
	return Type;
}
ISoundManager* FSLManagerCreater::CreateObject()
{
	return new FSLManager();
}
core::string FSLManagerCreater::getDescription()
{
	return mT("Free Sound Libray");
}



}
}


