

#include "stdafx.h"
#include "LeapHand.h"
#include "LeapFinger.h"
#include "LeapHandController.h"
#include "LeapFunctions.h"
#include "IDebugDrawManager.h"



namespace mray
{


LeapHand::LeapHand(LeapHandController* c)
{
	m_handController = c;
	m_palm = 0;
	m_foream = 0;
	m_wristJoint = 0;

	for (int i = 0; i < (int)ELeapFinger::Count; ++i)
	{
		m_fingers[i]=new LeapFinger(this,(Leap::Finger::Type)i);
	}
}
LeapHand::~LeapHand()
{
	for (int i = 0; i < (int)ELeapFinger::Count;++i)
	{
		delete m_fingers[i];
	}
}

LeapFinger* LeapHand::GetFinger(ELeapFinger finger)
{
	return m_fingers[(int)finger];
}
void LeapHand::SetHand(Leap::Hand  h)
{
	m_hand = h;
	if (m_hand.isValid())
	{

		Leap::FingerList fingers = m_hand.fingers();
		for (int i = 0; i < fingers.count(); ++i)
		{
			m_fingers[i]->SetFinger(fingers[i]);
		}
	}
	else
	{
		for (int i = 0; i < (int)ELeapFinger::Count; ++i)
		{
			m_fingers[i]->SetFinger(Leap::Finger());
		}

	}
	/*for (int i = 0; i < (int)ELeapFinger::Count; ++i)
	{
		Leap::Finger f = h.finger(m_fingers[i]->GetType());
		m_fingers[i]->SetFinger(f);
	}*/
}


math::vector3d LeapHand::GetElbowPosition()
{
	math::vector3d pos = LeapToVector3d(m_hand.arm().elbowPosition());
	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		pos = trans->getAbsoluteOrintation()*pos + trans->getAbsolutePosition();
	return pos;
}



math::vector3d LeapHand::GetArmDirection()
{
	math::vector3d dir = LeapToVector3d(m_hand.arm().direction());

	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		dir = trans->getAbsoluteOrintation()*dir;
	return dir;

}

math::quaternion LeapHand::GetArmRotation()
{
	math::matrix4x4 m;
	math::quaternion q;
	LeapToMatrix(m_hand.arm().basis(), m);
	q.fromMatrix(m);

	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		q = trans->getAbsoluteOrintation()*q;
	return q;
}


math::vector3d LeapHand::GetWristPosition()
{
	math::vector3d pos = LeapToVector3d(m_hand.arm().wristPosition());
	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		pos = trans->getAbsoluteOrintation()*pos + trans->getAbsolutePosition();
	return pos;
}


math::vector3d LeapHand::GetPalmPosition()
{
	math::vector3d pos = LeapToVector3d(m_hand.palmPosition());
	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		pos = trans->getAbsoluteOrintation()*pos + trans->getAbsolutePosition();
	pos+=GetPalmOffset();
	return pos;
}

math::vector3d LeapHand::GetPalmOffset()
{
	math::vector3d additionalMovement = m_handController->GetHandMovementScale()-math::vector3d(1);
	math::vector3d scaledPosition = LeapToVector3d(m_hand.palmPosition())*additionalMovement;
	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		scaledPosition = trans->getAbsoluteOrintation()*scaledPosition;// trans->getAbsoluteTransformation()*scaledPosition - trans->getAbsolutePosition();

	return scaledPosition;
}

math::quaternion LeapHand::GetPalmRotation()
{
	math::matrix4x4 m;
	math::quaternion q;
	LeapToMatrix(m_hand.basis(), m);
	q.fromMatrix(m);

	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		q = trans->getAbsoluteOrintation()*q;
	return q;
}

math::vector3d LeapHand::GetPalmDirection()
{
	math::vector3d dir= LeapToVector3d(m_hand.direction());

	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		dir = trans->getAbsoluteOrintation()*dir;
	return dir;
}

math::vector3d LeapHand::GetPalmNormal()
{
	math::vector3d dir = LeapToVector3d(m_hand.palmNormal());

	scene::IMovable* trans = m_handController->GetTransform();
	if (trans)
		dir = trans->getAbsoluteOrintation()*dir;
	return dir;

}

 void LeapHand::UpdatePosition()
{
	 if (!m_hand.isValid())
		 return; 
	 math::quaternion palm = (GetPalmRotation());
	 math::quaternion arm = (GetArmRotation());
	 if (m_palm)
	 {
		 //	 m_palm->setPosition(GetPalmPosition());
		// m_palm->setOrintation(GetPalmRotation());
	 }
	 if (m_foream)
	 {
	//	 m_foream->setPosition(GetElbowPosition());
	//	 m_foream->setOrintation(arm);
	 }
	 else
		 arm = math::quaternion::Identity;
	 if (m_wristJoint)
	 {
	//	 m_wristJoint->setPosition(GetWristPosition());
	//	 m_wristJoint->setOrintation(palm*arm.inverse());
	 }
	 for (int i = 0; i < (int)ELeapFinger::Count; ++i)
		 m_fingers[i]->UpdatePosition();
}

 void LeapHand::DrawDebug(scene::IDebugDrawManager* dbg)
 {
	 if (!m_hand.isValid())
		 return;

	 math::vector3d elbow = GetElbowPosition();
	 math::vector3d wrist = GetWristPosition();
	 math::vector3d palm = GetPalmPosition();
	 dbg->AddLine(elbow, wrist, video::SColor(1, 1, 0, m_hand.confidence()), 1, 0);

	 for (int i = 0; i < (int)ELeapFinger::Count; ++i)
	 {
		 dbg->AddLine(wrist, m_fingers[i]->GetJointPosition(0), video::SColor(1, 1, 0, m_hand.confidence()), 1, 0);
		 m_fingers[i]->DrawDebug(dbg);

	 }
 }

}