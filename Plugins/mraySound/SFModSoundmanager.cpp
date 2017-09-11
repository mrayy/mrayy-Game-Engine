#include "stdafx.h"

#include "SFmodSoundManager.h"
#include "SFModSound.h"
#include "SFModSoundStream.h"
#include "SFModListener.h"

#include <LogManager.h>
#include <math.h>
#include <IFileSystem.h>
#include <macros.h>
#include <SoundResourceManager.h>
#include <TraceManager.h>
#include <IOSSystem>
#include "SFModSound.h"

namespace mray{
namespace sound{

SFModSoundManager::SFModSoundManager():m_listener(0)
{

	traceFunction(eSound);
	//initilize OpenAL

	
	gLogManager.startSection(mT("FMOD"));
	gLogManager.flush();

	FMOD_RESULT result;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS        caps;
	char name[256];

	result=FMOD::System_Create(&m_system);
	if(reportError(result)) return;


	uint version;
	result=m_system->getVersion(&version);
	if(reportError(result)) return;

    if (version < FMOD_VERSION)
    {
		core::string msg;
		msg=mT("SFModSoundManager-FMod: using the old version!  You should be using FMOD ");
		msg+=FMOD_VERSION;
		gLogManager.log(msg.c_str(), ELL_WARNING);
        return;
    }

	result = m_system->getDriverCaps(0, &caps, 0, 0, &speakermode);
    if(reportError(result)) return;

	result = m_system->getSpeakerMode(&speakermode);       /* Set the user selected speaker mode. */
    if(reportError(result)) return;

    if (caps & FMOD_CAPS_HARDWARE_EMULATED)               
    {                                                   
		gLogManager.log(mT("SFModSoundManager-FMod: No hardware support! This is really bad for latency!"),ELL_INFO);
		result = m_system->setDSPBufferSize(1024, 10);
        if(reportError(result)) return;
    }

	
	result = m_system->getDriverInfo(0, name, 256, 0);
    if(reportError(result)) return;

    if (strstr(name, "SigmaTemT("))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
    {
		result = m_system->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
        if(reportError(result)) return;
    }

	
	result = m_system->init(100, FMOD_INIT_3D_RIGHTHANDED | FMOD_INIT_SOFTWARE_OCCLUSION | FMOD_INIT_SOFTWARE_HRTF, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
		result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        if(reportError(result)) return;
                
		result = m_system->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
        if(reportError(result)) return;
    }

	m_listener=0;//new SFModListener(m_system);

	result = m_system->set3DSettings(1,100,1);
	if(reportError(result)) return;

	result = m_system->createChannelGroup( "music",   &m_music );
	if(reportError(result)) return;
	result = m_system->createChannelGroup( "playing", &m_statePlaying );
	if(reportError(result)) return;
	result = m_system->createChannelGroup( "stopped", &m_stateStopped );
	if(reportError(result)) return;
	result = m_system->createChannelGroup( "startup", &m_stateStartup );
	if(reportError(result)) return;

    {
		core::string buffer;

        int num3d = 0, num2d = 0;

        result = m_system->getHardwareChannels(&num2d, &num3d, 0);
        reportError(result);

		buffer=mT("Hardware 2D channels :  ");
		buffer+=num2d;
        gLogManager.log(buffer, ELL_INFO);
		buffer=mT("Hardware 3D channels : ");
		buffer+=num3d;
        gLogManager.log(buffer, ELL_INFO);
    }

    /*
        Set the distance units. (meters/feet etc).
	*/

	m_dfactor=1;
	m_dvelocity=1;
	setDistanceFactor(1.0f);


	gLogManager.endSection(true);
	gLogManager.flush();


	

}

SFModSoundManager::~SFModSoundManager(){

	

	soundsList.clear();
	
	for (int i = 0; i < sndDataFiles.size(); ++i)
		sndDataFiles[i]->unload();
	sndDataFiles.clear();

	FMOD_RESULT result;
	result = m_system->close();
    if(reportError(result)) return;
	result = m_system->release();
    if(reportError(result)) return;

	//delete m_listener;


	
}
ISoundPtr SFModSoundManager::CreateManualSound(const core::string&name)
{
	return 0;
}

ISoundStreamFrame* SFModSoundManager::CreateManualStream()
{
	return 0;
}
void SFModSoundManager::setVolume(float v){
	
}

int SFModSoundManager::getCurrentSoundsStreaming(){
	return 0;
}

int SFModSoundManager::getMaxSounds(){
	return 0;
}

ulong SFModSoundManager::getUsedMemory(){
	return 0;
}

bool SFModSoundManager::reportError(FMOD_RESULT error){
	
	if(error!=FMOD_OK){
		core::string errMsg;
		core::char_to_string(FMOD_ErrorString(error),errMsg);
		gLogManager.log(mT("SFModSoundManager::reportError() ")+errMsg,ELL_WARNING);
		return 1;
	}
	return 0;

}


ISoundListener* SFModSoundManager::createListener()
{
	return new SFModListener(m_system);
}
void SFModSoundManager::setActiveListener(ISoundListener* l)
{
	m_listener=l;
}
ISoundListener* SFModSoundManager::getActiveListener()
{
	return m_listener;
}

void SFModSoundManager::setDistanceFactor(float factor){
	if(factor<=0)return;
	distance_factor=factor;
	FMOD_RESULT res=m_system->set3DSettings(m_dfactor, distance_factor, m_dvelocity);
	reportError(res);
}


void SFModSoundManager::setDopplerParameters(float factor,float velocity){
	m_dfactor=factor;
	m_dvelocity=velocity;
	FMOD_RESULT res=m_system->set3DSettings(m_dfactor, distance_factor, m_dvelocity);
	reportError(res);
}
void SFModSoundManager::setSpeedOfSound(float v){
}
void SFModSoundManager::setMetersPerUnit(float meters){
}

FMOD::System*SFModSoundManager::getFModSystem(){
	return m_system;
}

void SFModSoundManager::runSounds(float dt){
	
	traceFunction(eSound);

	FMOD_RESULT result;

	int channelsPlaying;
//	static int lastPlaying=0;
	m_system->getChannelsPlaying(&channelsPlaying);
	//FATAL_ERROR(channelsPlaying>20,"Max Channels Reach!");

	if(m_listener)
		m_listener->update(dt);
	//reportError(result);

	//update sounds
	SoundsList::iterator it=soundsList.begin();
	SoundsList::iterator it2;
	for(;it!=soundsList.end();){
		bool remove=0;
		if((*it)->isAttachedToListener() && m_listener)
			(*it)->setPosition(m_listener->getPosition());
		if((*it)->isPlaying())
			(*it)->update(dt);

		if(!(*it)->isPlaying() && (*it)->isDieAfterEnd()){
			remove=1;
		}
		if(remove){
			it2=it;
			it2++;
			soundsList.erase(it);
			it=it2;
		}else
			++it;
	}

	try{
		result=m_system->update();
		FATAL_ERROR(reportError(result),mT("Error in sound update!"));
		
	}catch(...){
	}
	
	
}


ISoundStreamFrame*SFModSoundManager::getSound(const core::string&filename){
	for(int i=0;i<sndDataFiles.size();++i){
		if(sndDataFiles[i]->getResourceName().equals_ignore_case(filename))
			return sndDataFiles[i];
	}
	return 0;
}

ISoundPtr SFModSoundManager::playSound(const core::string&sndName,const math::vector3d& pos,bool isStream,float vol,bool loop,ESoundType type){
	ISoundPtr snd=loadSound(sndName,isStream,type);
	if(snd.isNull())
		return ISoundPtr::Null;
	snd->setPosition(pos);
	snd->setVolume(vol);
	snd->setLooping(loop);
	snd->play();
	snd->setDieAfterEnd(1);
	return snd;
}
 

SFModSoundStream* SFModSoundManager::createDataFile(const core::string&filename,bool isStream,ESoundType type){
	if(filename==mT(""))return 0;

	traceFunction(eSound);

	core::string str;
	gFileSystem.getCorrectFilePath(filename,str);
	GCPtr<OS::IStream> stream=gFileSystem.openFile(str,OS::BIN_READ);
	if(!stream){
		return 0;
	}

	SFModSoundStream*dataFile=0;

	FMOD_RESULT result;

	dataFile=new  SFModSoundStream(filename);

	stream->seek(0,OS::ESeek_Set);
	int sz=stream->length();
	char*data=new char[sz];
	stream->read(data,sz);

	uint mode=0 ;
	if(type==ESNDT_2D)
		mode |= FMOD_2D;
	else mode |= FMOD_3D;
	dataFile->type=type;
	dataFile->filename=filename;
	dataFile->manager=this;
 
	FMOD_CREATESOUNDEXINFO exinfo;
	IOSSystem::memSet(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = sz;
	mode|=FMOD_OPENMEMORY_POINT;
	dataFile->m_streamData=data;
	if(isStream){
		mode|=FMOD_HARDWARE;
		result=m_system->createStream(data,mode,&exinfo,&dataFile->m_sound);
		dataFile->m_isStreaming=true;
	}else{
		mode|=FMOD_SOFTWARE ;
		result=m_system->createSound(data,mode,&exinfo,&dataFile->m_sound);
		dataFile->m_isStreaming=false;
	}

	if(reportError(result)){
		return 0;
	}
	sndDataFiles.push_back(dataFile);
	
	
	return dataFile;
}
ISoundPtr SFModSoundManager::loadSound(const core::string&filename,bool isStream,ESoundType type){

	traceFunction(eSound);

	core::string str;
	gFileSystem.getCorrectFilePath(filename,str);

	ISoundPtr snd;

	SFModSoundStream*dataFile=getOrCreate(filename,isStream,type);
	if(!dataFile)
		return ISoundPtr::Null;
	
	snd=new  SFModSound(this,filename);
	snd->setSoundBuffer(dataFile);
	addSound(snd);
		
	return snd;
}

SFModSoundStream* SFModSoundManager::getOrCreate(const core::string&filename,bool isStream,ESoundType type)
{


	ISound* snd=0;

	SFModSoundStream*dataFile=dynamic_cast<SFModSoundStream*>(getSound(filename));
	if(!dataFile){
		dataFile=createDataFile(filename.c_str(),isStream,type);
		if(!dataFile || !dataFile->m_sound)
			return 0;
	//	SoundResourceManager::getInstance().addResource(dataFile,filename);
	}
	return dataFile;
}

void SFModSoundManager::addSound(ISoundCRef snd){
	soundsList.push_back(snd);
}
void SFModSoundManager::removeSound(ISound*snd){
	SoundsList::iterator it=soundsList.begin();
	for(;it!=soundsList.end();++it){
		if((*it).pointer()==snd){
			snd->stop();
			soundsList.erase(it);
			return;
		}
	}
}

void SFModSoundManager::stopAllSounds(){
	SoundsList::iterator it=soundsList.begin();
	for(;it!=soundsList.end();++it){
		(*it)->stop();
	}
}
void SFModSoundManager:: playAllSounds(){

}
void SFModSoundManager:: pauseAllSounds(){

}
void SFModSoundManager:: resumeAllSounds(){

}

void SFModSoundManager::Clear()
{
}

const core::string FModManagerCreater::Type=mT("FMod");

const core::string& FModManagerCreater::GetType()
{
	return Type;
}
ISoundManager* FModManagerCreater::CreateObject()
{
	return new SFModSoundManager();
}
core::string FModManagerCreater::getDescription()
{
	return mT("FMod Sound Manager");
}


}
}