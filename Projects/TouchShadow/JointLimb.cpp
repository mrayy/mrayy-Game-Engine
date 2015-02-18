
#include "stdafx.h"
#include "JointLimb.h"


namespace mray
{


std::vector<JointLimb> JointLimb::Limbs;
JointLimb JointLimb::instance;

void JointLimb::Init()
{
	using namespace nite;

	Limbs.clear();
	Limbs.push_back(JointLimb(JOINT_NECK,JOINT_HEAD));

	Limbs.push_back(JointLimb(JOINT_LEFT_SHOULDER,JOINT_LEFT_ELBOW));
	Limbs.push_back(JointLimb(JOINT_LEFT_ELBOW,JOINT_LEFT_HAND));

	Limbs.push_back(JointLimb(JOINT_RIGHT_SHOULDER,JOINT_RIGHT_ELBOW));
	Limbs.push_back(JointLimb(JOINT_RIGHT_ELBOW,JOINT_RIGHT_HAND));

	Limbs.push_back(JointLimb(JOINT_LEFT_SHOULDER,JOINT_RIGHT_SHOULDER));

	Limbs.push_back(JointLimb(JOINT_TORSO,JOINT_LEFT_SHOULDER));
	Limbs.push_back(JointLimb(JOINT_TORSO,JOINT_RIGHT_SHOULDER));
	Limbs.push_back(JointLimb(JOINT_TORSO,JOINT_LEFT_HIP));
	Limbs.push_back(JointLimb(JOINT_TORSO,JOINT_RIGHT_HIP));

	Limbs.push_back(JointLimb(JOINT_LEFT_HIP,JOINT_RIGHT_HIP));

	Limbs.push_back(JointLimb(JOINT_LEFT_HIP,JOINT_LEFT_SHOULDER));
	Limbs.push_back(JointLimb(JOINT_RIGHT_HIP,JOINT_RIGHT_SHOULDER));

	Limbs.push_back(JointLimb(JOINT_LEFT_HIP,JOINT_LEFT_KNEE));
	Limbs.push_back(JointLimb(JOINT_LEFT_KNEE,JOINT_LEFT_FOOT));

	Limbs.push_back(JointLimb(JOINT_RIGHT_HIP,JOINT_RIGHT_KNEE));
	Limbs.push_back(JointLimb(JOINT_RIGHT_KNEE,JOINT_RIGHT_FOOT));

}

}
