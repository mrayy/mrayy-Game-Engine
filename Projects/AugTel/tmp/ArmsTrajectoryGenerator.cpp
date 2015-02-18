

#include "stdafx.h"
#include "ArmsTrajectoryGenerator.h"
#include "ArmComponent.h"
#include "HeadCameraComponent.h"
#include "GloveController5.h"
#include "BonesJointQuery.h"
#include "TJacobian.h"
#include "IDebugDrawManager.h"
#include "IMovableComponent.h"
#include "ATAppGlobal.h"
#include "IMovableComponent.h"
#include "OptiTrackObject.h"

namespace mray
{
namespace game
{
#define RIGHT_HAND_ID 1
#define HEAD_ID 2
#define LEFT_HAND_ID 3

ArmsTrajectoryGenerator::ArmsTrajectoryGenerator(game::GameEntityManager*m)
{
	m_gloves[0] = new AugTel::GloveController5("L");
	m_gloves[1] = new AugTel::GloveController5("R");

	m_armsIK[0] = new animation::IterativeIK();
	m_armsIK[1] = new animation::IterativeIK();
}

ArmsTrajectoryGenerator::~ArmsTrajectoryGenerator()
{
	delete m_gloves[0];
	delete m_gloves[1];

	delete m_armsIK[0];
	delete m_armsIK[1];
}


bool ArmsTrajectoryGenerator::InitComponent()
{
	if (!IGameComponent::InitComponent())
		return false;
	m_head = RetriveComponent<HeadCameraComponent>(m_ownerComponent,"Head");
	m_arms[0] = RetriveComponent<ArmComponent>(m_ownerComponent, "LeftArm");
	m_arms[1] = RetriveComponent<ArmComponent>(m_ownerComponent, "RightArm");

 	m_gloves[0]->SetHand(0);
 	m_gloves[1]->SetHand(1);
 	m_gloves[0]->Start();
 	m_gloves[1]->Start();

	m_arms[0]->InitComponent();
	m_arms[1]->InitComponent();

	animation::BonesJointQuery *q;
	for (int i = 0; i < 2; ++i)
	{
		q = new animation::BonesJointQuery();
		q->addBone(m_arms[i]->GetShoulder());
		q->addBone(m_arms[i]->GetFoream());
		q->addBone(m_arms[i]->GetWrist(), true);
		m_armsIK[i]->setJointsChain(q);
		m_armsIK[i]->calcOrintation(false);
		m_armsIK[i]->setBeta(0.01);
		m_armsIK[i]->setAllowedError(0.001);
		m_armsIK[i]->setMaxIterations(200);
		//m_ik->setTarget(math::vector3d(-42.624 ,137.684   ,24.88   ));
		m_armsIK[i]->setJacobian(new math::TJacobian());

		std::vector<animation::BonesJointQuery::JointDOF>& bones = q->getJointsDOF();
		bones[0].hasAngularConst.set(0,0,0);
		bones[0].lowerAngularConst.set(-50, -90, -100);
		bones[0].upperAngularConst.set(50, 90, 90);

		bones[1].hasAngularConst.set(0,0,0);
		bones[1].lowerAngularConst.set(-50, 0, 0);
		bones[1].upperAngularConst.set(50, 140, 90);

		m_arms[i]->GetWrist()->updateAbsoluteTransformation();
		m_origPos[i] = m_arms[i]->GetWrist()->getAbsolutePosition();
	}

	const core::string pref[2] = { "L_Ref", "R_Ref" };
	for (int h = 0; h < 2; ++h)
	{
		core::string prefix = pref[h] + "F";
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				IMovableComponent* node=RetriveComponent<IMovableComponent>(m_ownerComponent, prefix + core::StringConverter::toString(i+1) + core::StringConverter::toString(j+1));
				node->InitComponent();
				m_fingers[h][i][j].ref = node->GetMovableObject();
				m_fingers[h][i][j].node = (scene::BoneNode*)m_arms[h]->GetFinger(i, j);

				m_fingers[h][i][j].ref->updateAbsoluteTransformation();
				m_fingers[h][i][j].node->updateAbsoluteTransformation();
				m_fingers[h][i][j].space = m_fingers[h][i][j].ref->getAbsoluteOrintation().inverse()*m_fingers[h][i][j].node->getAbsoluteOrintation();
			}
		}
	}


	return true;
}

void ArmsTrajectoryGenerator::Update(float dt)
{
	IGameComponent::Update(dt);
	for (int g = 0; g < 2; ++g)
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m_gloves[g]->Update(dt);
				scene::BoneNode* part = (scene::BoneNode*)m_arms[g]->GetFinger(i, j);
				if (part)
				{
					//	part->setOrintation(math::quaternion(0, 0, m_gloves[g]->GetFingerValue(i, j)));
					math::vector3d angles = m_fingers[g][i][j].space* math::vector3d(m_gloves[g]->GetFingerValue(i, j), 0, 0);
					part->setOrintation(part->getBaseOrintation()* math::quaternion(angles));
				}
			}
		}
	}
	
	
	math::vector3d *pos= AugTel::ATAppGlobal::Instance()->optiTrack->GetPositionByID(HEAD_ID);
	math::quaternion *ori;
	if (pos)
	{
	//	m_head->GetNode()->setPosition(*pos);
	}

	int handsIDs[2] = { LEFT_HAND_ID, RIGHT_HAND_ID };

	static float time = 0;
	time += dt;
	for (int i = 0; i < 2; ++i)
	{
		pos = AugTel::ATAppGlobal::Instance()->optiTrack->GetPositionByID(handsIDs[i]);
		ori = AugTel::ATAppGlobal::Instance()->optiTrack->GetOrientationByID(handsIDs[i]);
		if (pos)
		{
			m_targetPos[i] = *pos;
			m_targetOri[i] = *ori;
		}


		m_targetPos[i] = m_origPos[i];
		m_targetPos[i].x += 0.1*cos(time * 2);
		m_targetPos[i].y += 0.1*sin(time * 2);
		m_targetPos[i].z += 0.1*sin(time * 5) - 0.05;
		//m_armsIK[i]->setTarget(m_targetPos[0], m_arms[0]->GetWrist()->getAbsoluteOrintation());


		m_armsIK[i]->setTarget(m_targetPos[i], ((scene::BoneNode*)m_arms[i]->GetWrist())->getBaseOrintation()*m_targetOri[i]);
		m_armsIK[i]->update();
		m_arms[i]->GetWrist()->setOrintation(((scene::BoneNode*)m_arms[i]->GetWrist())->getBaseOrintation()*m_targetOri[i]);
	}
	return;

	m_targetPos[1] = m_origPos[1];
	m_targetPos[1].x += 0.2*cos(time * 2);
	m_targetPos[1].y += 0.2*sin(time * 2);
	m_targetPos[1].z += 0.2*sin(time * 5) - 0.1;
	m_armsIK[1]->setTarget(m_targetPos[1], m_arms[1]->GetWrist()->getAbsoluteOrintation());

}
void ArmsTrajectoryGenerator::DebugRender(scene::IDebugDrawManager* renderer)
{
	video::SColor clr[2] = { video::DefaultColors::Red, video::DefaultColors::Green };
	for (int i = 0; i < 2; ++i)
	{
		renderer->AddCross(m_targetPos[i], 0.5, clr[i], 0.1);
		renderer->AddLine(m_targetPos[i], m_targetPos[i] + ((scene::BoneNode*)m_arms[i]->GetWrist())->getBaseOrintation()*m_targetOri[i] * math::vector3d(0, 0, 1), clr[i], 2, 0.1);

		scene::IMovable* elems[3];

		elems[0] = m_arms[i]->GetShoulder();
		elems[1] = m_arms[i]->GetFoream();
		elems[2] = m_arms[i]->GetWrist();

		for (int i = 0; i < 3; ++i)
		{
			math::quaternion ori = elems[i]->getAbsoluteOrintation();
			math::vector3d pos = elems[i]->getAbsolutePosition();

			renderer->AddLine(pos, pos + ori*math::vector3d::XAxis*0.1, video::DefaultColors::Red, 2, 0);
			renderer->AddLine(pos, pos + ori*math::vector3d::YAxis*0.1, video::DefaultColors::Green, 2, 0);
			renderer->AddLine(pos, pos + ori*math::vector3d::ZAxis*0.1, video::DefaultColors::Blue, 2, 0);

		}
	}

}


IMPLEMENT_RTTI(ArmsTrajectoryGenerator, IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(ArmsTrajectoryGenerator);


}
}

