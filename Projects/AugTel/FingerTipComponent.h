

/********************************************************************
	created:	2014/02/14
	created:	14:2:2014   10:40
	filename: 	C:\Development\mrayEngine\Projects\AugTel\FingerTipComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	FingerTipComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __FingerTipComponent__
#define __FingerTipComponent__


#include "MountableComponent.h"
#include "IFeedbackControl.h"

namespace mray
{
	namespace VT
	{
		class CameraComponent;
		class GGComponent;
	}
namespace AugTel
{

	//this component used to tell where the finger tip is, 
	// and it is linked with gravity grabber component
	// It measures the collision with depth map and drivers GG forces
class FingerTipComponent :public VT::MountableComponent, public VT::IFeedbackControl
{
	DECLARE_RTTI;
protected:
	DECLARE_PROPERTY_TYPE(NormalAxis, math::vector3d, );
	DECLARE_PROPERTY_TYPE(Channel, int, );
	DECLARE_PROPERTY_TYPE(GGDriverName, core::string, );
	DECLARE_PROPERTY_TYPE(IsLeftHand, bool, );

	VT::GGComponent* m_ggDriver;

	int m_index;

	int m_channel;
	math::vector3d m_normalAxis;
	core::string m_GGDriverName;
	bool m_isLeftHand;

	bool m_wasEnabled;

	math::vector3d m_projPos;
	math::vector2di m_depthSpace;
	
	math::vector3d m_lastPos;
	math::vector3d m_avgVelocity;

	math::vector3d m_force;
	float m_time;
	void _sendForceToGG();
	bool _calculateForce(float dt);
public:
	FingerTipComponent(game::GameEntityManager*m);
	virtual~FingerTipComponent();

	DECLARE_SETGET_PROP(NormalAxis, math::vector3d);
	DECLARE_SETGET_PROP(Channel, int);
	DECLARE_SETGET_PROP(GGDriverName, core::string);
	DECLARE_SETGET_PROP(IsLeftHand, bool);

	virtual bool InitComponent();

	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);

};
DECLARE_GAMECOMPONENT_FACTORY(FingerTipComponent);

}
}

#endif
