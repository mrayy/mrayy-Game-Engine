#include "stdafx.h"
#include "LeapHandController.h"
#include "LeapMotionController.h"
#include "LeapHand.h"

#include <leap/Leap.h>

namespace mray
{


LeapHandController::LeapHandController(LeapMotionController* c)
{
	m_controller = c;
	m_leftHand = new LeapHand(this);
	m_rightHand = new LeapHand(this);
	m_handMovementScale = 1;
}
LeapHandController:: ~LeapHandController()
{

}

void LeapHandController::Update(float dt)
{

	if (!m_controller->GetController()->isConnected())
		return;
	Leap::Frame frame = m_controller->GetController()->frame();

	std::set<int> toRemove;
	HandsMap::iterator it = m_hands.begin();
	for (; it != m_hands.end();++it)
	{
		toRemove.insert(it->first);
	}

	m_leftHand->SetHand(Leap::Hand());
	m_rightHand->SetHand(Leap::Hand());
	
	Leap::HandList leapHands = frame.hands();
	for (int i = 0; i < leapHands.count(); ++i)
	{
		Leap::Hand hand = leapHands[i];
// 		if (hand.confidence() < 0.4)
// 			continue;
		toRemove.erase(hand.id());
		HandsMap::iterator it = m_hands.find(hand.id());
		LeapHand* h = 0;
		if (it != m_hands.end())
		{
			h = it->second;
			h->SetHand(hand);
		}
		else
		{
			h = new LeapHand(this);
			m_hands[hand.id()] = h;
			h->SetHand(hand);

			FIRE_LISTENR_METHOD(OnHandCreated, (this, h));
		}
		h->UpdatePosition();
		if (hand.isLeft())
			m_leftHand->SetHand(hand);
		else
			m_rightHand->SetHand(hand);
	}
	m_leftHand->UpdatePosition();
	m_rightHand->UpdatePosition();
	{
		std::set<int>::iterator it2= toRemove.begin();
		for (; it2 != toRemove.end();++it2)
		{
			it = m_hands.find(*it2);
			FIRE_LISTENR_METHOD(OnHandRemoved, (this,it->second));
			delete it->second;
			m_hands.erase(it);
		}
	}
}

}
