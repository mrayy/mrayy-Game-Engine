
#include "stdafx.h"
#include "LeapMotionController.h"
#include "LeapHand.h"



namespace mray
{


LeapMotionController::LeapMotionController()
{
}
LeapMotionController::~LeapMotionController()
{

}


void LeapMotionController::Init()
{

	bool m_LeapHMDModeWasOn = m_LeapController.config().getBool("head_mounted_display_mode");

	int flags = m_LeapController.policyFlags();
	flags |= Leap::Controller::POLICY_IMAGES;

	m_LeapHMDModeWasOn = (flags & Leap::Controller::POLICY_OPTIMIZE_HMD) != 0;
	//if (!m_LeapHMDModeWasOn) 
	{
		flags |= Leap::Controller::POLICY_OPTIMIZE_HMD;
	}
	m_LeapController.setPolicyFlags(static_cast<Leap::Controller::PolicyFlag>(flags));

}

}


