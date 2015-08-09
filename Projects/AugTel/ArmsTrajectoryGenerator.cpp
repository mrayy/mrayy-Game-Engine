

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
#include "CoupledJointComponent.h"
#include "AppData.h"
#include "OculusDevice.h"
#include "FontResourceManager.h"

namespace mray
{
namespace AugTel
{
#define NECK_LENGTH 0.2f
#define SHOULDER_DISTANCE 0.1f
	void ArmsTrajectoryGenerator::BoneInfo::Init(scene::IMovable* b, int tid)
	{
		bone = b;
		if (bone)
		{
			initialRotation = b->getOrintation();
			math::vector3d angles;
			initialRotation.toEulerAngles(angles);

			printf("Joint[%s]:%s\n", b->getNodeName().c_str(), core::StringConverter::toString(angles).c_str());
		}
		trackingID = tid;
	}
	void ArmsTrajectoryGenerator::BoneInfo::SetRotation(const math::quaternion& ori,bool worldSpace)
	{
		if (bone)
		{
			math::quaternion q;
			q.Slerp(bone->getOrintation(), initialRotation*ori, 4 * gEngine.getFPS()->dt());
			q = ori*rotationReset;
			if (bone->getParent() && worldSpace)
			{
				q = bone->getAbsoluteOrintation().inverse()*q*bone->getAbsoluteOrintation();
			}
			else
			{
			}
			q = initialRotation *q;
			bone->setOrintation(q);
		}
	}
	void ArmsTrajectoryGenerator::BoneInfo::SetPosition(const math::vector3d& pos, bool worldSpace)
	{
		if (bone)
		{
			math::vector3d p;
			if (bone->getParent() && worldSpace)
			{
				p = bone->getParent()->getAbsoluteTransformation().inverseTransformVector(pos);
			}
			else
			{
				p = pos;
			}
			bone->setPosition(p);
		}
	}
ArmsTrajectoryGenerator::ArmsTrajectoryGenerator(game::GameEntityManager*m)
{
	m_gloves[ELeft] = new AugTel::GloveController5("L");
	m_gloves[ERight] = new AugTel::GloveController5("R");

	m_armsIK[ELeft] = new animation::IterativeIK();
	m_armsIK[ERight] = new animation::IterativeIK();
	m_wristIK[ELeft] = new animation::IterativeIK();
	m_wristIK[ERight] = new animation::IterativeIK();
}

ArmsTrajectoryGenerator::~ArmsTrajectoryGenerator()
{
	delete m_gloves[ELeft];
	delete m_gloves[ERight];

	delete m_armsIK[ELeft];
	delete m_armsIK[ERight];
	delete m_wristIK[ELeft];
	delete m_wristIK[ERight];
}


bool ArmsTrajectoryGenerator::InitComponent()
{
	if (!IGameComponent::InitComponent())
		return false;
	m_backBone = RetriveComponent<game::IMovableComponent>(m_ownerComponent, "J_Back");
	m_head = RetriveComponent<HeadCameraComponent>(m_ownerComponent,"Head");
	m_arms[ELeft] = RetriveComponent<ArmComponent>(m_ownerComponent, "LeftArm");
	m_arms[ERight] = RetriveComponent<ArmComponent>(m_ownerComponent, "RightArm");

	m_gloves[ELeft]->SetHand(0);
	m_gloves[ELeft]->Start();
	m_gloves[ERight]->SetHand(1);
	m_gloves[ERight]->Start();

	m_arms[ELeft]->InitComponent();
	m_arms[ERight]->InitComponent();

	m_bones[HeadBone].Init(m_head->GetNode(), 1);
	m_bones[BackBone].Init(m_backBone->GetMovableObject(), -1);
	m_bones[LShoulderBone].Init(m_arms[ELeft]->GetShoulder()->GetMovableObject(), -1);
	m_bones[LArmBone].Init(m_arms[ELeft]->GetFoream()->GetMovableObject(), -1);
	m_bones[LWristBone].Init(m_arms[ELeft]->GetWrist()->GetMovableObject(), -1);
	m_bones[LWristBaseBone].Init(m_arms[ELeft]->GetWristBase()->GetMovableObject(), -1);
	m_bones[RShoulderBone].Init(m_arms[ERight]->GetShoulder()->GetMovableObject(), -1);
	m_bones[RWristBaseBone].Init(m_arms[ERight]->GetWristBase()->GetMovableObject(), -1);
	m_bones[RWristBone].Init(m_arms[ERight]->GetWrist()->GetMovableObject(), 2);
	m_bones[RArmBone].Init(m_arms[ERight]->GetFoream()->GetMovableObject(), 3);

	//math::vector3d pos=m_bones[HeadBone].bone->getPosition();
	//pos.y = NECK_LENGTH;
	//m_bones[HeadBone].bone->setPosition(pos);
	((scene::BoneNode*)m_bones[HeadBone].bone)->SetInheritTransformation(false);

	m_status = EWaiting;
	
	for (int g = 0; g < 2; ++g)
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				game::IMovableComponent* f = m_arms[g]->GetFinger(i, j);
				if (f)
					m_fingers[g][i * 3 + j].Init(f->GetMovableObject(), -1);
			}
		}
	}
	animation::BonesJointQuery *q;
	for (int i = 0; i < 2; ++i)
	{
		q = new animation::BonesJointQuery();
		//q->addBone(m_arms[i]->GetShoulder()->GetMovableObject());
		q->addBone(m_arms[i]->GetShoulder()->GetMovableObject(), animation::EIKJointType::Prismatic, math::vector3d::YAxis)->SetLimits(-138,90);
		q->addBone(m_arms[i]->GetShoulder()->GetMovableObject(), animation::EIKJointType::Prismatic, math::vector3d::ZAxis)->SetLimits(-100, 30);
		q->SetEndEffector(m_arms[i]->GetFoream()->GetMovableObject());
		m_armsIK[i]->setJointsChain(q);
		m_armsIK[i]->calcOrintation(false);
		m_armsIK[i]->setBeta(0.1);
		m_armsIK[i]->setAllowedError(0.01);
		m_armsIK[i]->setMaxIterations(200);
		//m_ik->setTarget(math::vector3d(-42.624 ,137.684   ,24.88   ));
		m_armsIK[i]->setJacobian(new math::TJacobian());


		q = new animation::BonesJointQuery();
		//q->addBone(m_arms[i]->GetShoulder()->GetMovableObject());
		//q->addBone(m_arms[i]->GetShoulder()->GetMovableObject());
		q->addBone(m_arms[i]->GetShoulder()->GetMovableObject(), animation::EIKJointType::Prismatic, math::vector3d::YAxis)->SetLimits(-138, 90);
		q->addBone(m_arms[i]->GetShoulder()->GetMovableObject(), animation::EIKJointType::Prismatic, math::vector3d::XAxis)->SetLimits(-50, 100);
		q->addBone(m_arms[i]->GetShoulder()->GetMovableObject(), animation::EIKJointType::Prismatic, math::vector3d::ZAxis)->SetLimits(-100, 30);
		q->addBone(m_arms[i]->GetFoream()->GetMovableObject(), animation::EIKJointType::Prismatic, math::vector3d::ZAxis)->SetLimits(-120, -5);
		q->SetEndEffector(m_arms[i]->GetWristBase()->GetMovableObject());
		m_wristIK[i]->setJointsChain(q);
		m_wristIK[i]->calcOrintation(false);
		m_wristIK[i]->setBeta(0.1);
		m_wristIK[i]->setAllowedError(0.01);
		m_wristIK[i]->setMaxIterations(200);
		//m_ik->setTarget(math::vector3d(-42.624 ,137.684   ,24.88   ));
		m_wristIK[i]->setJacobian(new math::TJacobian());

		m_origPos[i] = m_arms[i]->GetWrist()->GetMovableObject()->getAbsolutePosition();
	}
	return true;
}
math::quaternion ArmsTrajectoryGenerator::_CalcHeadOrintation()
{
	math::quaternion ori;
	if (AppData::Instance()->oculusDevice)
	{
		ori = AppData::Instance()->oculusDevice->GetOrientation();
		math::vector3d angles;
		ori.toEulerAngles(angles);
		ori.fromEulerAngles(angles.x, -angles.y, -angles.z);
	}
	else
	{
		math::vector3d p;
		float lenghth = 0.4;
		GetPosOri(m_bones[HeadBone].trackingID, p, ori);

		math::vector3d angles;
		ori.toEulerAngles(angles);
		ori.fromEulerAngles(-angles.x, angles.y, -angles.z);
	}
	return ori;
}
void ArmsTrajectoryGenerator::_ResetHeadOrintation()
{

	if (AppData::Instance()->oculusDevice)
	{
		AppData::Instance()->oculusDevice->ResetOrientation();
		m_bones[HeadBone].rotationReset = math::quaternion::Identity;
	}
	else
	{
		math::vector3d p;
		math::quaternion ori;
		float lenghth = 0.3;
		if (GetPosOri(m_bones[HeadBone].trackingID, p, ori))
		{
			m_bones[HeadBone].rotationReset = ori.inverse();
		}
	}
}
bool ArmsTrajectoryGenerator::GetPosOri(int id, math::vector3d& p, math::quaternion& ori)
{
#if USE_OPTITRACK
	if (!AugTel::ATAppGlobal::Instance()->optiDataSource->GetPositionByID(id))
		return false;
	p= *AugTel::ATAppGlobal::Instance()->optiDataSource->GetPositionByID(id);
	ori= *AugTel::ATAppGlobal::Instance()->optiDataSource->GetOrientationByID(id);
//  	p.x = -p.x;
//  	p.z = -p.z;
	return true;
#else
	return false;
#endif
}
float ArmsTrajectoryGenerator::_CalcArmLength(int arm, int wrist)
{
	math::vector3d p1,p2;
	math::quaternion ori;
	float lenghth = 0.4;
	if (GetPosOri(m_bones[arm].trackingID, p1, ori))
	{
		if (GetPosOri(m_bones[wrist].trackingID, p2, ori))
		{
			lenghth = p1.getDist(p2);
		}
	}
	return lenghth;

}
void ArmsTrajectoryGenerator::_SetWristOrintation(ArmsTrajectoryGenerator::EHand hand, const math::quaternion& q)
{
	int wrist, wristBase,elbow;
	if (hand == ELeft)
	{
		wristBase = LWristBaseBone;
		wrist = LWristBone;
		elbow = LArmBone;
	}
	else
	{
		wristBase = RWristBaseBone;
		wrist = RWristBone;
		elbow = RArmBone;
	}
	math::vector3d angles,eang;
	q.toEulerAngles(angles);
	angles.z = math::clamp<float>(angles.z, -90, 90);
	math::quaternion wbOri(angles.z, 0, 0);
	m_bones[elbow].bone->getOrintation().toEulerAngles(eang);
	math::quaternion eOri=m_bones[elbow].bone->getOrintation()*math::quaternion(angles.z / 4, 0, 0);;
	//eang.x = angles.z / 2;
	//m_bones[elbow].bone->setOrintation(eang);
	
	m_bones[wristBase].SetRotation(wbOri, false);

	m_wristAngles = angles;

//	m_bones[elbow].SetRotation(eang, false);
	//m_bones[wrist].SetRotation(math::quaternion(0, -angles.y, angles.x), false);
}

void ArmsTrajectoryGenerator::_Calibrate()
{
	float arm1 = _CalcArmLength(LArmBone, LWristBone);
	float arm2 = _CalcArmLength(RArmBone, RWristBone);
	m_arms[ELeft]->SetArmLength(arm1);
	m_arms[ERight]->SetArmLength(arm2);

	m_arms[ELeft]->SetShoulderDistance(SHOULDER_DISTANCE);
	m_arms[ERight]->SetShoulderDistance(SHOULDER_DISTANCE);
}

void ArmsTrajectoryGenerator::Update(float dt)
{
	IGameComponent::Update(dt);
	math::vector3d pos;
	scene::IMovable* parent = 0;
	math::quaternion ori;
	if (AppData::Instance()->inputMngr->getKeyboard()->getKeyState(KEY_L))
	{
		_Calibrate();

		m_status = EReady;
	}
	if (m_status != EReady)
		return;
	if (AppData::Instance()->inputMngr->getKeyboard()->getKeyState(KEY_C))
	{
		for (int i = 0; i < EBonesCount; ++i)
		{
			if (GetPosOri(m_bones[i].trackingID, pos, ori))
			{
				m_bones[i].rotationReset = ori.inverse();
			}
		}
		_ResetHeadOrintation();
	}
	static float fangle = 0;
	fangle += 10*dt;
	float fingerVal = sin(fangle) * 45;
	for (int g = 0; g < 2; ++g)
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				m_gloves[g]->Update(dt);
				fingerVal = m_gloves[g]->GetFingerValue(i, j);
				m_fingers[g][i * 3 + j].SetRotation(math::quaternion(0, 0, fingerVal), false);
				/*
				VT::CoupledJointComponent* part = m_arms[g]->GetFinger(i, j);
				if (part)
				{
					//	part->setOrintation(math::quaternion(0, 0, m_gloves[g]->GetFingerValue(i, j)));
					math::vector3d angles =  math::vector3d(m_gloves[g]->GetFingerValue(i, j), 0, 0);
					//part->setOrintation(math::quaternion(angles));
				}*/
			}
		}
	}
	
	
	if ( GetPosOri(m_bones[HeadBone].trackingID,pos,ori))
	{
		//m_bones[HeadBone].bone->setPosition(pos - math::vector3d(0, 0.3, 0));

// 		parent = m_head->GetNode()->getParent();
// 		parent->updateAbsoluteTransformation();
// 		m_head->GetNode()->setPosition(parent->getAbsoluteTransformation().inverseTransformVector(pos));
// 		m_head->GetNode()->setOrintation(ori*parent->getAbsoluteOrintation().inverse());

// 		math::vector3d angles;
// 		ori.toEulerAngles(angles); 

		math::vector3d p=pos ;// = m_backBone->GetMovableObject()->getPosition();
		p.y -= NECK_LENGTH;
		//pos.z -= 0.2f;
		m_head->GetNode()->setPosition(pos);
		//p=p-m_backBone->GetMovableObject()->getAbsolutePosition();
		m_backBone->GetMovableObject()->setPosition(p);// , scene::TS_World);
		m_backBone->GetMovableObject()->updateAbsoluteTransformation();
		//m_head->GetNode()->setPosition(pos - m_backBone->GetMovableObject()->getAbsolutePosition());
	}
	m_bones[HeadBone].SetRotation(_CalcHeadOrintation(),false);
	math::vector3d angles;

	static float angle = 0;
	angle += dt*40;
	angles.z = 45 * math::sind(angle);
	for (int i = LShoulderBone; i < EBonesCount; ++i)
	{
		if (GetPosOri(m_bones[i].trackingID, pos, ori))
		{
			//ori.toEulerAngles(angles);
			//m_bones[i].SetRotation(ori);
		}
		else
		{
			//m_bones[i].SetRotation(angles);
		}
	}

	if (true)
	{
		if (GetPosOri(m_bones[LArmBone].trackingID, pos, ori))
		{
			m_armsIK[ELeft]->setTarget(pos, math::quaternion::Identity);
		}
		if (GetPosOri(m_bones[RArmBone].trackingID, pos, ori))
		{
			m_armsIK[ERight]->setTarget(pos, math::quaternion::Identity);
		}
		if (GetPosOri(m_bones[LWristBone].trackingID, pos, ori))
		{
			m_wristIK[ELeft]->setTarget(pos, math::quaternion::Identity);
			m_targetPos[ELeft] = pos;
			m_targetOri[ELeft] = ori;
		}
		if (GetPosOri(m_bones[RWristBone].trackingID, pos, ori))
		{
			m_wristIK[ERight]->setTarget(pos, math::quaternion::Identity);
			m_targetPos[ERight] = pos;
			m_targetOri[ERight] = ori;

		}

	}
	{
		if (false)
		{
			if (GetPosOri(m_bones[RArmBone].trackingID, pos, ori))
			{
				parent = m_bones[RArmBone].bone->getParent();
				parent->updateAbsoluteTransformation();
				//	m_bones[RArmBone].bone->setPosition(parent->getAbsoluteTransformation().inverseTransformVector(pos));
				m_bones[RArmBone].SetRotation(ori, true);
				{

					math::vector3d w = pos;
					math::vector3d s = m_bones[RShoulderBone].bone->getAbsolutePosition();
					math::vector3d angles;
					w -= s;
					w.Normalize();
					math::quaternion q;
					q.fromMatrix(math::MathUtil::CreateLookAtMatrix(0, w, math::vector3d::YAxis));
					//q = q*m_bones[LShoulderBone].bone->getAbsoluteOrintation().inverse();
					m_bones[RShoulderBone].SetRotation(q, true);
				}
			}
			if (GetPosOri(m_bones[LArmBone].trackingID, pos, ori))
			{
				parent = m_bones[LArmBone].bone->getParent();
				parent->updateAbsoluteTransformation();
				//	m_bones[LArmBone].bone->setPosition(parent->getAbsoluteTransformation().inverseTransformVector(pos));
				m_bones[LArmBone].SetRotation(ori, true);
				//calculate shoulder orintation
				{

					math::vector3d w = pos;
					math::vector3d s = m_bones[LShoulderBone].bone->getAbsolutePosition();
					math::vector3d angles;
					w -= s;
					w.Normalize();
					math::quaternion q;
					q.fromMatrix(math::MathUtil::CreateLookAtMatrix(0, w, math::vector3d::YAxis));
					//q = q*m_bones[LShoulderBone].bone->getAbsoluteOrintation().inverse();
					m_bones[LShoulderBone].SetRotation(q, true);
				}
			}
			if (GetPosOri(m_bones[RWristBone].trackingID, pos, ori))
			{
				parent = m_bones[RWristBone].bone->getParent();
				parent->updateAbsoluteTransformation();
			//	m_bones[RWristBone].bone->setPosition(parent->getAbsoluteTransformation().inverseTransformVector(pos));
				m_bones[RWristBone].SetRotation(ori, true);
			}
			if (GetPosOri(m_bones[LWristBone].trackingID, pos, ori))
			{
				parent = m_bones[LWristBone].bone->getParent();
				parent->updateAbsoluteTransformation();
				//m_bones[LWristBone].bone->setPosition(parent->getAbsoluteTransformation().inverseTransformVector(pos));
				m_bones[LWristBone].SetRotation(ori, true);
			}
		}

	}
	if (true)
	{
		math::vector3d angles;
		//for (int i = 0; i < 2; ++i)
		int i = ERight;
		{
			m_armsIK[i]->update();
			
			//limit the foream angles , the dirty way...
			math::quaternion q= m_arms[i]->GetFoream()->GetMovableObject()->getOrintation();
			q.toEulerAngles(angles);
			angles.x = 0;
			//angles.y = 0;
			//m_arms[i]->GetFoream()->GetMovableObject()->setOrintation(angles);
			m_wristIK[i]->update();
		}
		if (false)
		{
			for (int i = 0; i < 2; ++i)
			{
				m_targetPos[i] = m_origPos[i];
				m_targetPos[i].x += 0.2*cos(angle * 0.02);
				m_targetPos[i].y += 0.2*sin(angle * 0.02)+1.4;
				m_targetPos[i].z += 0.2*sin(angle * 0.04) + 0.3;
				m_wristIK[i]->setTarget(m_targetPos[i], m_targetOri[i]);
				m_wristIK[i]->update();
			}
		}
		m_targetOri[ELeft].toEulerAngles(angles);
		m_targetOri[ELeft].fromEulerAngles(-angles.x, angles.y, -angles.z);
		_SetWristOrintation(ELeft, m_targetOri[ELeft]);
		m_targetOri[ERight].toEulerAngles(angles);
		m_targetOri[ERight].fromEulerAngles(-angles.x, angles.y, -angles.z);
		_SetWristOrintation(ERight, m_targetOri[ERight]);
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
	IGameComponent::DebugRender(renderer);
	if (!AppData::Instance()->IsDebugging)
		return;
	math::vector3d pos;
	math::quaternion ori;
	renderFrame(renderer, math::vector3d(0,0.1,1), math::quaternion::Identity);
	video::SColor clr[2] = { video::DefaultColors::Red, video::DefaultColors::Green };

	for (int i = 0; i < EBonesCount; ++i)
	{
		if (GetPosOri(m_bones[i].trackingID, pos, ori))
		{
			renderFrame(renderer, pos, ori);
			renderer->AddArrow(pos, pos + ori*math::vector3d::YAxis, video::DefaultColors::YellowGreen, 1, 0);
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		renderer->AddCross(m_targetPos[i], 0.5, clr[i], 0.1);
		renderer->AddLine(m_targetPos[i], m_targetPos[i] + m_targetOri[i] * math::vector3d(0, 0, 1), clr[i], 2, 0);
	}
}

void ArmsTrajectoryGenerator::OnGUIRender(GUI::IGUIRenderer* renderer,const math::rectf& vp)
{
	IGameComponent::OnGUIRender(renderer,vp);
	if (!AppData::Instance()->IsDebugging)
		return;

	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	AppData* app = AppData::Instance();
	if (font)
	{
		attr.fontColor.Set(1, 0, 0, 1);
		attr.fontAligment = GUI::EFA_TopLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = vp;
		r.ULPoint.y += 30;

		core::string msg = mT("Wrist: ") + core::StringConverter::toString(math::vector3di(m_wristAngles));
		font->print(r, &attr, 0, msg, renderer);
		r.ULPoint.y += attr.fontSize + 5;
	}

}

IMPLEMENT_RTTI(ArmsTrajectoryGenerator, IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(ArmsTrajectoryGenerator);


}
}

