

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
#include "OptiTrackDataSource.h"
#include "CompositeBoneComponent.h"

namespace mray
{
namespace game
{

ArmsTrajectoryGenerator::ArmsTrajectoryGenerator(game::GameEntityManager*m)
{
	m_gloves[0] = new AugTel::GloveController5("L");
	m_gloves[1] = new AugTel::GloveController5("R");

}

ArmsTrajectoryGenerator::~ArmsTrajectoryGenerator()
{
	delete m_gloves[0];
	delete m_gloves[1];
}


bool ArmsTrajectoryGenerator::InitComponent()
{
	if (!IGameComponent::InitComponent())
		return false;
	m_backBone = RetriveComponent<CompositeBoneComponent>(m_ownerComponent, "Back");
	m_head = RetriveComponent<HeadCameraComponent>(m_ownerComponent,"Head");
	m_arms[0] = RetriveComponent<ArmComponent>(m_ownerComponent, "LeftArm");
	m_arms[1] = RetriveComponent<ArmComponent>(m_ownerComponent, "RightArm");

 	m_gloves[0]->SetHand(0);
 	m_gloves[1]->SetHand(1);
 	m_gloves[0]->Start();
 	m_gloves[1]->Start();

	m_arms[0]->InitComponent();
	m_arms[1]->InitComponent();

	const core::string pref[2] = { "L_Ref", "R_Ref" };
	for (int h = 0; h < 2; ++h)
	{
		core::string prefix = pref[h] + "F";
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				IMovableComponent* node=RetriveComponent<IMovableComponent>(m_ownerComponent, prefix + core::StringConverter::toString(i+1) + core::StringConverter::toString(j+1));
				if (!node)
					continue;
				node->InitComponent();
				m_fingers[h][i][j].ref = node->GetMovableObject();
				m_fingers[h][i][j].node = (scene::BoneNode*)m_arms[h]->GetFinger(i, j);

				m_fingers[h][i][j].ref->updateAbsoluteTransformation();
				m_fingers[h][i][j].node->updateAbsoluteTransformation();
				m_fingers[h][i][j].space = m_fingers[h][i][j].ref->getAbsoluteOrintation().inverse()*m_fingers[h][i][j].node->getAbsoluteOrintation();
			}
		}
	}


	m_headTID = 1;
	m_wristTID[0] = 2;
	m_elbowTID[0] = 3;
	m_wristTID[1] = 2;
	m_elbowTID[1] = 3;

	return true;
}

bool ArmsTrajectoryGenerator::GetPosOri(int id, math::vector3d& p, math::quaternion& ori)
{
	if (!AugTel::ATAppGlobal::Instance()->optiDataSource->GetPositionByID(id))
		return false;
	p= *AugTel::ATAppGlobal::Instance()->optiDataSource->GetPositionByID(id);
	ori= *AugTel::ATAppGlobal::Instance()->optiDataSource->GetOrientationByID(id);


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
	
	
	math::vector3d pos;
	math::quaternion ori;
	scene::IMovable* parent;
	if ( GetPosOri(m_headTID,pos,ori))
	{
		m_backBone->SetPosition(pos-math::vector3d(0,0.3,0),true);

		parent = m_head->GetNode()->getParent();
		parent->updateAbsoluteTransformation();
		m_head->GetNode()->setPosition(parent->getAbsoluteTransformation().inverseTransformVector(pos));
		m_head->GetNode()->setOrintation(ori*parent->getAbsoluteOrintation().inverse());
	}
	
	for (int i = 0; i < 2; ++i)
	{
		if (GetPosOri(m_elbowTID[i], pos, ori))
		{
			parent = m_head->GetNode()->getParent();
			parent = m_arms[i]->GetFoream()->getParent();
			pos = parent->getAbsoluteTransformation().inverseTransformVector(pos);
			//m_arms[0]->GetFoream()->setPosition(pos);
			m_arms[i]->GetFoream()->setOrintation(ori*parent->getAbsoluteOrintation().inverse());
		}
		if (GetPosOri(m_wristTID[i], pos, ori))
		{
			parent = m_arms[i]->GetWrist()->getParent();
			pos = parent->getAbsoluteTransformation().inverseTransformVector(pos);
			//m_arms[0]->GetWrist()->setPosition(pos);
			m_arms[i]->GetWrist()->setOrintation(ori*parent->getAbsoluteOrintation().inverse());
		}
	}


	return;
}

void renderFrame(scene::IDebugDrawManager* renderer, const math::vector3d &pos, const math::quaternion &ori)
{
	renderer->AddLine(pos, pos + ori*math::vector3d::XAxis*0.2, video::DefaultColors::Red, 1, 0);
	renderer->AddLine(pos, pos + ori*math::vector3d::YAxis*0.2, video::DefaultColors::Green, 1, 0);
	renderer->AddLine(pos, pos + ori*math::vector3d::ZAxis*0.2, video::DefaultColors::Blue, 1, 0);

}
void ArmsTrajectoryGenerator::DebugRender(scene::IDebugDrawManager* renderer)
{
	math::vector3d pos;
	math::quaternion ori;
	renderFrame(renderer, math::vector3d::Zero, math::quaternion::Identity);

	if (GetPosOri(m_headTID, pos, ori))
	{
		renderFrame(renderer, pos, ori);
		renderer->AddArrow(pos, pos + ori*math::vector3d::YAxis, video::DefaultColors::YellowGreen, 1, 0);
	}

	for (int i = 0; i < 2; ++i)
	{
		if (GetPosOri(m_elbowTID[i], pos, ori))
		{
			renderFrame(renderer, pos, ori);

			renderer->AddLine(pos, pos + ori*math::vector3d::ZAxis, video::DefaultColors::Red, 1, 0);
		}
		if (GetPosOri(m_wristTID[i], pos, ori))
		{
			renderFrame(renderer, pos, ori);

			renderer->AddLine(pos, pos + ori*math::vector3d::ZAxis, video::DefaultColors::Blue, 1, 0);
		}
	}
}


IMPLEMENT_RTTI(ArmsTrajectoryGenerator, IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(ArmsTrajectoryGenerator);


}
}

