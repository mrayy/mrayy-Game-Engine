
/********************************************************************
	created:	2009/08/04
	created:	4:8:2009   11:29
	filename: 	i:\Programing\GameEngine\mrayEngine\mraySound\SFModListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mraySound
	file base:	SFModListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SFModListener___
#define ___SFModListener___

#include <IMovable.h>
#include <ISoundListener.h>

#include "compileConfig.h"

#include <fmod\fmod.hpp>

namespace mray{
namespace sound{

class MRAY_SOUND_DLL SFModListener:public ISoundListener
{
private:
protected:

	scene::IMovable* m_node;

	FMOD::System *m_system;

	math::vector3d m_position,m_lastPos;
	math::vector3d m_velocity;
	math::quaternion m_orient;

public:
	SFModListener(FMOD::System*sys);

	virtual~SFModListener();

	virtual void setPosition(const math::vector3d& p);
	virtual const math::vector3d& getPosition(){return m_position;}

	virtual void setVelocity(const math::vector3d& p);
	virtual const math::vector3d& getVelocity(){return m_velocity;}

	virtual void setOreintation(const math::quaternion& d);
	virtual const math::quaternion& getOreintation(){return m_orient;}

	virtual void attachToListener(scene::IMovable*node);

	virtual scene::IMovable* getListenerNode();

	void setDistanceModel(EListenerDistanceModel m);
	EListenerDistanceModel getDistanceModel();
	
	void GetSamples(float* samples, float count, int channel);
	void GetSpectrum(float* samples, float count, int channel, EFFT_WindowType window) ;

	virtual void update(float dt);
};



}
}


#endif //___SFModListener___
