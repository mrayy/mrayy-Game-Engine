

#ifndef LeapMotionHandsController_h__
#define LeapMotionHandsController_h__

#include "IHandsController.h"

namespace mray
{
	class LeapMotionImageRetrival;
namespace AugTel
{
	class LeapMotionHandsControllerImpl;
	
class LeapMotionHandsController:public IHandsController
{
protected:
	bool m_enabled;
	LeapMotionHandsControllerImpl* m_data;
public:
	LeapMotionHandsController();
	virtual ~LeapMotionHandsController();


	virtual void Init(AugTelSceneContext* context);
	virtual void Start(AugTelSceneContext* context);
	virtual void End(AugTelSceneContext* context);
	virtual void Update(float dt);
	virtual void RenderStart(const math::rectf& rc, TBee::ETargetEye eye);
	virtual void DebugRender(scene::IDebugDrawManager* dbg, const math::rectf& rc, TBee::ETargetEye eye);

	virtual math::vector3d GetHandPosition(EHandType hand);
	virtual math::quaternion GetHandRotation(EHandType hand);
	virtual math::vector3d GetFingerPosition(EHandType hand, EFingerType finger);

	virtual void LoadFromXML(xml::XMLElement* e);


	virtual core::string GetHandsType(){ return "Leap"; }


	virtual void SetEnabled(bool e);
	virtual bool IsEnabled() { return m_enabled; }


	LeapMotionImageRetrival* GetLeapImage(int i);
};

}
}

#endif // LeapMotionHandsController_h__
