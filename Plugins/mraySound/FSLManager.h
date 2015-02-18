
/********************************************************************
	created:	2009/08/04
	created:	4:8:2009   12:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mraySound\FSLManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mraySound
	file base:	FSLManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FSLManager___
#define ___FSLManager___

#include <ISoundManager.h>
#include <ObjectFactory.h>
#include <GCPtr.h>
#include "compileconfig.h"


#include <map>

namespace mray{
namespace OS{
	class IThread;
	class IThreadFunction;
}
namespace sound{

class MRAY_SOUND_DLL FSLManager:public ISoundManager
{
private:
protected:
	SoundsList m_sounds;
	ISoundListener* m_listener;

	typedef std::map<uint,ISoundStreamFrame*> SoundStreamMap;
	SoundStreamMap m_sndDataFiles;
	ISoundStreamFrame *getSound(const core::string&filename);

	OS::IThreadFunction* m_streamingFunc;
	OS::IThread* m_streamingThread;

public:
	FSLManager();
	virtual~FSLManager();

	ISoundPtr CreateManualSound(const core::string&name);

	virtual ISoundStreamFrame* CreateManualStream();

	ISoundStreamFrame* getOrCreate(const core::string&filename,bool streaming,ESoundType type);

	ISoundPtr playSound(const core::string&sndName,const math::vector3d& pos,bool isStream,float vol,bool loop=0,ESoundType type=ESNDT_3D);

	ISoundPtr loadSound(const core::string&filename,bool isStream,ESoundType type=ESNDT_3D);


	ISoundListener* createListener();
	void setActiveListener(ISoundListener* l);
	ISoundListener* getActiveListener();

	void setVolume(float v);

	int getCurrentSoundsStreaming();

	int getMaxSounds();

	ulong getUsedMemory();

	const SoundsList&getSoundsList(){return m_sounds;}

	void addSound(ISoundCRef snd);
	void removeSound(ISound*snd);

	void setDistanceFactor(float factor);
	float getDistanceFactor();

	void runSounds(float dt);


	void setDopplerParameters(float factor,float velocity);
	void setSpeedOfSound(float v);
	void setMetersPerUnit(float meters);


	void stopAllSounds();
	void playAllSounds();
	void pauseAllSounds();
	void resumeAllSounds();


	void Clear();
};

class MRAY_SOUND_DLL FSLManagerCreater:public ObjectFactory<ISoundManager>
{
public:
	static const core::string Type;

	virtual const core::string& GetType();
	virtual ISoundManager* CreateObject();
	virtual core::string getDescription();
};


}
}

#endif //___FSLManager___
