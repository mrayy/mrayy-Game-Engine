

/********************************************************************
	created:	2013/05/08
	created:	8:5:2013   20:01
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\LinkDebugComponent.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	LinkDebugComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LinkDebugComponent__
#define __LinkDebugComponent__


#include "IVTComponent.h"

namespace mray
{
	namespace game
	{
		class IPhysical6DOFJointComponent;
		class IPhysicsComponent;
	}
namespace VT
{

	class LinkDebugComponentData;
	enum ELinkDebugTargetInput
	{
		ELink_Kinematic,
		ELink_Realtime
	};
class LinkDebugComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:
	LinkDebugComponentData* m_data;
public:
	DECLARE_PROPERTY_TYPE(Cascade,bool,);
	DECLARE_PROPERTY_TYPE(UseKinematic,bool,);
	DECLARE_PROPERTY_TYPE(ThresholdAngle,float,);
	DECLARE_PROPERTY_TYPE(TargetAlpha,float,);
	DECLARE_PROPERTY_TYPE(Color,video::SColor,);
	DECLARE_PROPERTY_TYPE(TargetInput,ELinkDebugTargetInput,);

public:
	LinkDebugComponent(game::GameEntityManager*);
	virtual~LinkDebugComponent();

	bool SetCascade(bool v);
	bool GetCascade();

	bool SetUseKinematic(bool v);
	bool GetUseKinematic();

	bool SetThresholdAngle(float v);
	float GetThresholdAngle();

	bool SetTargetAlpha(float v);
	float GetTargetAlpha();

	bool SetColor(const video::SColor& v);
	const video::SColor& GetColor();

	bool SetTargetInput(ELinkDebugTargetInput e);
	ELinkDebugTargetInput GetTargetInput();

	virtual bool InitComponent();

	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);

	virtual void OnDestroy();
};


DECLARE_GAMECOMPONENT_FACTORY(LinkDebugComponent);


}
}


#endif
