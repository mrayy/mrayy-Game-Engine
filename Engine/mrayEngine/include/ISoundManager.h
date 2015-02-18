

#ifndef ___ISoundManager___
#define ___ISoundManager___


#include "mstring.h"

#include "SoundEnums.h"
#include "ISound.h"

namespace mray{
namespace sound{

	class ISoundListener;
	class ISoundStreamFrame;


class ISoundManager
{
public:
	typedef std::list<ISoundPtr> SoundsList;

	virtual ~ISoundManager(){}

	virtual ISoundStreamFrame* CreateManualStream()=0;

	virtual ISoundPtr CreateManualSound(const core::string&name)=0;

	virtual ISoundPtr playSound(const core::string&sndName,const math::vector3d& pos,bool isStream,float vol,
		bool loop=0,ESoundType type=ESNDT_3D)=0;

	virtual ISoundPtr loadSound(const core::string&filename,bool isStream,
		ESoundType type=ESNDT_3D)=0;

	virtual ISoundListener* createListener()=0;
	virtual void setActiveListener(ISoundListener* l)=0;
	virtual ISoundListener* getActiveListener()=0;

	virtual const SoundsList& getSoundsList()=0;

	virtual void addSound(ISoundCRef snd)=0;
	virtual void removeSound(ISound*snd)=0;

	virtual void setVolume(float v)=0;

	virtual int getCurrentSoundsStreaming()=0;

	virtual int getMaxSounds()=0;

	virtual ulong getUsedMemory()=0;

	virtual void setDistanceFactor(float factor)=0;
	virtual float getDistanceFactor()=0;

	virtual void setDopplerParameters(float factor,float velocity)=0;
	virtual void setSpeedOfSound(float v)=0;
	virtual void setMetersPerUnit(float meters)=0;

	virtual void runSounds(float dt)=0;

	virtual void stopAllSounds()=0;
	virtual void playAllSounds()=0;
	virtual void pauseAllSounds()=0;
	virtual void resumeAllSounds()=0;

	virtual void Clear()=0;
};


}
}




#endif


