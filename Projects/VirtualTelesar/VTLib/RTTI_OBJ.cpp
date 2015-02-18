
#include "stdafx.h"

#include "MountableComponent.h"
#include "RobotCommunicatorComponent.h"
#include "GUIGeneratorComponent.h"
#include "DistanceCalculatorComponent.h"
#include "CoupledJointComponent.h"
#include "CommunicationComponent.h"
#include "CameraComponent.h"
#include "PhysicsTransformComponent.h"
#include "TouchSensorComponent.h"
#include "ErrorCalculatorComponent.h"
#include "SoundListenerComponent.h"
#include "SpringTouchSensorComponent.h"
#include "LinkDebugComponent.h"
#include "KinBodyControlComponent.h"
#include "GGComponent.h"
#include "ContactDisablerComponent.h"
#include "VTCalibrationComponent.h"

namespace mray
{
namespace VT
{

	IMPLEMENT_RTTI(IVTComponent,game::IGameComponent);
	IMPLEMENT_RTTI(ICommunicatorComponent,IVTComponent);
	IMPLEMENT_RTTI(MountableComponent,IVTComponent);
	IMPLEMENT_RTTI(GUIGeneratorComponent,IVTComponent);
	IMPLEMENT_RTTI(DistanceCalculatorComponent,IVTComponent);
	IMPLEMENT_RTTI(PhysicsTransformComponent,IVTComponent);
	IMPLEMENT_RTTI(TouchSensorComponent,IVTComponent);
	IMPLEMENT_RTTI(ErrorCalculatorComponent,IVTComponent);
	IMPLEMENT_RTTI(LinkDebugComponent,IVTComponent);
	IMPLEMENT_RTTI(GGComponent,IVTComponent);
	IMPLEMENT_RTTI(ContactDisablerComponent,IVTComponent);

	IMPLEMENT_RTTI(CameraComponent,MountableComponent);
	IMPLEMENT_RTTI(SoundListenerComponent,MountableComponent);

	IMPLEMENT_RTTI(CommunicationComponent,ICommunicatorComponent);
	IMPLEMENT_RTTI(RobotCommunicatorComponent,ICommunicatorComponent);

	IMPLEMENT_RTTI(CoupledJointComponent,IVTComponent);
	IMPLEMENT_RTTI(SpringTouchSensorComponent,IVTComponent);
	IMPLEMENT_RTTI(KinBodyControlComponent, IVTComponent);
	IMPLEMENT_RTTI(VTCalibrationComponent, IVTComponent);

	IMPLEMENT_GAMECOMPONENT_FACTORY(SoundListenerComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(MountableComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(ErrorCalculatorComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(TouchSensorComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(DistanceCalculatorComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(PhysicsTransformComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(CommunicationComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(RobotCommunicatorComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(CoupledJointComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(CameraComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(SpringTouchSensorComponent)
	IMPLEMENT_GAMECOMPONENT_FACTORY(LinkDebugComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(KinBodyControlComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(GGComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(ContactDisablerComponent);
	IMPLEMENT_GAMECOMPONENT_FACTORY(VTCalibrationComponent);

}
}