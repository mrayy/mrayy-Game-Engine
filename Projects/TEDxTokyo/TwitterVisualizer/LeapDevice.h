

/********************************************************************
	created:	2014/04/11
	created:	11:4:2014   23:37
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\LeapDevice.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	LeapDevice
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LeapDevice__
#define __LeapDevice__


#include <leap/Leap.h>

namespace mray
{
namespace nui
{


	enum class ELeapGestureType
	{
		Circle,
		Swipe,
		ScreenTap,
		SurfaceTap
	};

	class LeapDevice;
	class ILeapListener
	{
	public:
		virtual void OnConnected(LeapDevice* d){}
		virtual void OnDisconnected(LeapDevice* d){}
		virtual void OnGesture(LeapDevice* d, ELeapGestureType type, Leap::Gesture g){}
		virtual void OnHands(LeapDevice* d, Leap::HandList hands){}
	};
class LeapDevice:public Leap::Listener,public ListenerContainer<ILeapListener*>
{
protected:
	Leap::Controller m_controller;

	DECLARE_FIRE_METHOD(OnConnected, (LeapDevice* d), (d));
	DECLARE_FIRE_METHOD(OnDisconnected, (LeapDevice* d), (d));
	DECLARE_FIRE_METHOD(OnGesture, (LeapDevice* d, ELeapGestureType type, Leap::Gesture g), (d,type,g));
	DECLARE_FIRE_METHOD(OnHands, (LeapDevice* d, Leap::HandList hands), (d,hands));
protected:
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
public:
	LeapDevice();
	virtual~LeapDevice();

	bool IsConnected();

};

}
}

#endif
