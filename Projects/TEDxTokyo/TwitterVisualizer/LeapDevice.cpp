

#include "stdafx.h"
#include "LeapDevice.h"

using namespace Leap;

namespace mray
{
namespace nui
{


LeapDevice::LeapDevice()
{
	m_controller.addListener(*this);

}
LeapDevice::~LeapDevice()
{
}

bool LeapDevice::IsConnected()
{
	return m_controller.isConnected();
}

 void LeapDevice::onInit(const Controller&)
{
	printf("Leap controller Init\n");
}
 void LeapDevice::onConnect(const Controller&controller)
{
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);

	FIRE_LISTENR_METHOD(OnConnected, (this));
}
 void LeapDevice::onDisconnect(const Controller&)
{
	 FIRE_LISTENR_METHOD(OnDisconnected, (this));
 }
 void LeapDevice::onExit(const Controller&)
{

}
 void LeapDevice::onFrame(const Controller& c)
{
	 const Frame frame = m_controller.frame();
	 FIRE_LISTENR_METHOD(OnHands, (this, frame.hands()));

	 const GestureList gestures = frame.gestures();
	 for (int g = 0; g < gestures.count(); ++g) {
		 Gesture gesture = gestures[g];

		 switch (gesture.type()) {
		 case Gesture::TYPE_CIRCLE:
			 FIRE_LISTENR_METHOD(OnGesture, (this, ELeapGestureType::Circle, gesture));
			 break;
		 case Gesture::TYPE_SWIPE:
			 FIRE_LISTENR_METHOD(OnGesture, (this, ELeapGestureType::Swipe, gesture));
			 break;;
		 case Gesture::TYPE_SCREEN_TAP:
			 FIRE_LISTENR_METHOD(OnGesture, (this, ELeapGestureType::ScreenTap, gesture));
			 break;;
		 case Gesture::TYPE_KEY_TAP:
			 FIRE_LISTENR_METHOD(OnGesture, (this, ELeapGestureType::SurfaceTap, gesture));
			 break;;
		 }
	 }
 }
 void LeapDevice::onFocusGained(const Controller&)
{

}
 void LeapDevice::onFocusLost(const Controller&)
{

}

}
}