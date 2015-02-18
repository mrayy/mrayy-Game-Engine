

#ifndef ___SALSoundManager___
#define ___SALSoundManager___


#include <ISoundManager.h>
#include <mArray.h>
#include <C_List.h>

#include <SoundManagerFactory.h>

#include <GCPtr.h>

#	include <OpenAL\AL.h>
#	include <OpenAL\ALC.h>
#	include <OpenAL\ALUT.h>
#	include <OpenAL\efx.h>
#	include <OpenAL\xram.h>
#	include <OpenAL\efx-creative.h>

#include "compileConfig.h"
namespace mray{
namespace scene{
	
	class ISceneManager;
}

namespace sound{


/*! 
  add/remove sounds
*/
class MRAY_SOUND_DLL SALSoundManager:public ISoundManager
{

	ALCdevice*  m_Device;
	ALCcontext* m_Context;

	
	void checkForError(const mchar*info);

	scene::IMovable* m_listener;

	math::vector3d lastPos;
	math::vector3d m_listenerPos;
	math::vector3d m_listenerDir;
	bool m_attachToNode;

	float distance_factor;

	std::list<GCPtr<ISound>> soundsList;

//	std::vector<GCPtr<loaders::ISoundLoader>>soundLoaders;
//	std::vector<GCPtr<ISoundStreamFrame>>sndDataFiles;
//	GCPtr<ISoundStreamFrame>getSound(const  mchar *filename);
public:
	SALSoundManager();
	virtual ~SALSoundManager();



	const mchar*getALVersion()const;
	const mchar*getALVendor()const;
	const mchar*getALRenderer()const;
	const mchar*getALExtensions()const;

	virtual GCPtr<ISound>playSound(const  mchar*sndName,math::vector3d pos,bool isStream,float vol,bool loop=0,ESoundType type=ESoundType::ESNDT_3D);

	virtual GCPtr<ISound>loadSound(const  mchar *filename,bool isStream,ESoundType type=ESoundType::ESNDT_3D);

	virtual std::list<GCPtr<ISound>>*getSoundsList(){return &soundsList;}

	virtual void addSound(GCPtr<ISound>snd);
	virtual void removeSound(GCPtr<ISound>snd);

	virtual void enableListener(bool a){m_attachToNode=a;}
	virtual bool isAttachedToListener(){return m_attachToNode;}

	virtual void setListenerPos(math::vector3d p){m_listenerPos=p;}
	virtual math::vector3d getListenerPos(){return m_listenerPos;}
	
	virtual void setListenerDir(math::vector3d d){m_listenerDir=d;}
	virtual math::vector3d getListenerDir(){return m_listenerDir;}

	virtual void setDistanceFactor(float factor);
	virtual float getDistanceFactor(){return distance_factor;}

	virtual void attachToListener(scene::IMovable*node);
	virtual scene::IMovable* getListenerNode();

	virtual void runSounds(float dt);


	virtual void stopAllSounds();
	virtual void playAllSounds();
	virtual void pauseAllSounds();
	virtual void resumeAllSounds();
};



class MRAY_SOUND_DLL OpenALManagerCreater:public ISoundManagerCreater
{
public:
	virtual GCPtr<ISoundManager> create();

	virtual bool canCreate(const mchar*type);
	virtual const mchar* getType();
	virtual const mchar* getDescription();
};

}
}


#endif




