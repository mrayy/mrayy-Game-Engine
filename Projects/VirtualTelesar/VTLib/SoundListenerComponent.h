

/********************************************************************
	created:	2012/09/24
	created:	24:9:2012   14:38
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\SoundListenerComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	SoundListenerComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___SoundListenerComponent___
#define ___SoundListenerComponent___

#include "MountableComponent.h"
#include "ISoundListener.h"

namespace mray
{
namespace VT
{

class SoundListenerComponent:public MountableComponent
{
	DECLARE_RTTI;
protected:
	float m_gain;
public:
	GCPtr<sound::ISoundListener> m_listener;
public:
	DECLARE_PROPERTY_TYPE(Gain,float,);

	SoundListenerComponent(game::GameEntityManager*mngr);
	virtual~SoundListenerComponent();

	virtual bool InitComponent();

	const GCPtr<sound::ISoundListener>& GetListener(){return m_listener;}

	DECLARE_SETGET_PROP(Gain,float);

	void SetActive();
	virtual void Update(float dt);
};
DECLARE_GAMECOMPONENT_FACTORY(SoundListenerComponent);

}
}

#endif
