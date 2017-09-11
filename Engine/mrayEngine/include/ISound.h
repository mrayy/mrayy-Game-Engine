


#ifndef ___ISound___
#define ___ISound___


#include "compileconfig.h"
#include "mString.h"
#include "GCPtr.h"
#include "SoundEnums.h"
#include "IObject.h"
#include "RTTI.h"
#include "TypedProperty.h"

namespace mray{
	namespace xml
	{
		class XMLElement;
	}
namespace sound{


	class ISoundStreamFrame;


class MRAY_DLL ISound:public IObject
{
	DECLARE_RTTI
protected:
	core::string m_name;
public:
	DECLARE_PROPERTY_TYPE(Position,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Direction,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Volume,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Pitch,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Looping,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AttachedToListener,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(MaxDistance,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(MinDistance,float,MRAY_DLL);

public:

	ISound(const core::string&name);
	virtual~ISound();

	void setName(const core::string&name);
	const  core::string& getName();
	
	virtual bool setPosition(const math::vector3d& pos)=0;
	virtual math::vector3d getPosition()=0;

	virtual bool setVelocity(const math::vector3d& vel)=0;
	virtual math::vector3d getVelocity()=0;

	virtual bool setDirection(const math::vector3d& dir)=0;
	virtual math::vector3d getDirection()=0;

	virtual bool setVolume(float vol)=0;
	virtual float getVolume()=0;

	virtual bool setMaxDistance(float max_dist)=0;
	virtual float getMaxDistance()=0;

	virtual bool setMinDistance(float min_dist)=0;
	virtual float getMinDistance()=0;

	virtual bool setPitch(float pitch)=0;
	virtual float getPitch()=0;

	virtual bool setLooping(bool loop)=0;
	virtual bool getLooping()=0;


	virtual bool isAttachedToListener()=0;
	virtual bool AttachedToListener(bool a)=0;

	virtual bool setDieAfterEnd(bool s)=0;
	virtual bool isDieAfterEnd()=0;

	virtual bool setMode(ESoundType mode)=0;
	virtual ESoundType getMode()=0;

	virtual bool isPlaying()=0;
	virtual void play()=0;
	virtual void pause()=0;
	virtual void stop()=0;
	virtual void rewind()=0;

	virtual void update(float dt)=0;

	virtual void seek(float time)=0;
	virtual uint getTimeLength()=0;
	virtual uint getTimePosition()=0;

	virtual void setSoundBuffer(GCPtr<ISoundStreamFrame> data) = 0;
	virtual GCPtr<ISoundStreamFrame> getSoundBuffer() = 0;

	virtual void QueueSoundBuffer(ISoundStreamFrame*data)=0;
	virtual void UnqueueSoundBuffer(ISoundStreamFrame*data)=0;

	virtual void GetSamples(float* samples, float count, int channel)=0;
	virtual void GetSpectrum(float* samples, float count, int channel, EFFT_WindowType window) = 0;

// 	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement* e);
// 	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement*e);
};

MakeSharedPtrType(ISound)

}
}



#endif