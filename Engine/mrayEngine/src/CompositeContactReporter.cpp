#include "stdafx.h"

#include "CompositeContactReporter.h"


namespace mray
{
namespace physics
{

CompositeContactReporter::CompositeContactReporter(){}
CompositeContactReporter::~CompositeContactReporter(){}

void CompositeContactReporter::OnPairStartTouch(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairStartTouch,(pair));
}
void CompositeContactReporter::OnPairTouch(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairTouch,(pair));
}
void CompositeContactReporter::OnPairEndTouch(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairEndTouch,(pair));
}
void CompositeContactReporter::OnPairImpact(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairImpact,(pair));
}
void CompositeContactReporter::OnPairRoll(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairRoll,(pair));
}
void CompositeContactReporter::OnPairSlide(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairSlide,(pair));
}

void CompositeContactReporter::OnPairStartTouchForceThreshold(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairStartTouchForceThreshold,(pair));
}
void CompositeContactReporter::OnPairTouchForceThreshold(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairTouchForceThreshold,(pair));
}
void CompositeContactReporter::OnPairEndTouchForceThreshold(PhysicContactPair* pair)
{
	FIRE_LISTENR_METHOD(OnPairEndTouchForceThreshold,(pair));
}

}
}