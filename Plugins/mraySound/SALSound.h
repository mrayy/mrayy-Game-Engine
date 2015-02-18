
#ifndef ___SALSound___
#define ___SALSound___


#include <mTypes.h>
#include <Point3d.h>
#include <mString.h>
#include <ISound.h>

#include <GCPtr.h>

#include "compileConfig.h"
namespace mray{
namespace sound{

class SALSoundManager;
enum ESoundType;

/*! 
   Sound Node Class
*/
class MRAY_SOUND_DLL SALSound:public ISound
{
	math::vector3d lastPos;
	math::vector3d position;
	math::vector3d velocity;
	math::vector3d direction;
	float m_pitch;
	float m_volume;
	bool m_looping;
	float max_distance;
	float min_distance;

	uint m_SourceID;
	uint m_format;
	int m_bufferCount;
	uint m_buffersID[2];
	SALSoundManager*soundSystem;

	GCPtr<ISoundStreamFrame >dataFormat;
	bool m_bAttachToListener;
	bool dieAfterEnd;

	void checkForError(const mchar*info);
public:
	SALSound(SALSoundManager*sndMngr,const core::string&name,math::vector3d pos=math::vector3d(0,0,0),math::vector3d vel=math::vector3d(0,0,0),math::vector3d dir=math::vector3d(0,0,0));
	virtual ~SALSound();

	virtual void setPosition(math::vector3d pos);
	virtual math::vector3d getPosition(){return position;}

	virtual void setVelocity(math::vector3d vel);
	virtual math::vector3d getVelocity(){return velocity;}

	virtual void setDirection(math::vector3d dir);
	virtual math::vector3d getDirection(){return direction;}

	virtual void setVolume(float vol);
	virtual float getVolume(){return m_volume;}

	virtual void setMaxDistance(float max_dist);
	virtual float getMaxDistance(){return max_distance;}

	virtual void setMinDistance(float min_dist);
	virtual float getMinDistance(){return min_distance;}

	virtual void setPitch(float pitch);
	virtual float getPitch(){return m_pitch;}

	virtual void setLooping(bool loop);
	virtual bool getLooping(){return m_looping;}

	virtual bool isAttachedToListener(){return m_bAttachToListener;}
	virtual void AttachedToListener(bool a){m_bAttachToListener=a;}

	virtual void setDieAfterEnd(bool s){dieAfterEnd=s;}
	virtual bool isDieAfterEnd(){return dieAfterEnd;}

	virtual void setMode(ESoundType mode);

	virtual bool isPlaying();
	virtual void play();
	virtual void pause();
	virtual void stop();

	virtual void update(float dt);
	


	virtual void setData(ISoundStreamFrame*data);
	virtual ISoundStreamFrame*getData(){return dataFormat;}

	int streamBuffer(uint bufferID,int size);
	void unqueueAll();

};

}
}


#endif


