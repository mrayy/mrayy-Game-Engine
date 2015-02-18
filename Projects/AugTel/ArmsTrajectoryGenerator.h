
/********************************************************************
	created:	2014/01/16
	created:	16:1:2014   20:34
	filename: 	C:\Development\mrayEngine\Projects\AugTel\ArmsTrajectoryGenerator.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	ArmsTrajectoryGenerator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ArmsTrajectoryGenerator__
#define __ArmsTrajectoryGenerator__


#include "IGameComponent.h"
#include "IterativeIK.h"

namespace mray
{
	namespace game
	{
		class IMovableComponent;
	}
namespace AugTel
{
	class ArmComponent;
	class IFingerDataProvider;
	class HeadCameraComponent;
class ArmsTrajectoryGenerator:public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	enum EHand
	{
		ERight=0,
		ELeft
	};
	enum EBones
	{
		HeadBone,
		BackBone,
		LShoulderBone,
		LArmBone,
		LWristBaseBone,
		LWristBone,

		RShoulderBone,
		RArmBone,
		RWristBaseBone,
		RWristBone,

		EBonesCount
	};

	enum EStatus
	{
		EWaiting,
		EReady
	};

	struct BoneInfo
	{
		scene::IMovable* bone;
		math::quaternion initialRotation;
		math::quaternion rotationReset;


		int trackingID;

		void Init(scene::IMovable* b, int tid);
		void SetRotation(const math::quaternion& ori, bool worldSpace);
		void SetPosition(const math::vector3d& pos, bool worldSpace);
	};

	EStatus m_status;
	BoneInfo m_bones[EBonesCount];
	BoneInfo m_fingers[2][5*3];
	game::IMovableComponent* m_backBone;
	HeadCameraComponent* m_head;
	ArmComponent* m_arms[2];
	animation::IterativeIK* m_armsIK[2];
	animation::IterativeIK* m_wristIK[2];

	math::vector3d m_targetPos[2];
	math::quaternion m_targetOri[2];
	math::vector3d m_origPos[2];
// 	int m_headTID;
// 	int m_elbowTID[2];
// 	int m_wristTID[2];

	math::vector3d m_wristAngles;

	AugTel::IFingerDataProvider* m_gloves[2];

	void _CalculateArm(int i);

	bool GetPosOri(int id,math::vector3d& p, math::quaternion& ori);
	float _CalcArmLength(int arm,int wrist);
	math::quaternion _CalcHeadOrintation();
	void _ResetHeadOrintation();
	void _SetWristOrintation(ArmsTrajectoryGenerator::EHand hand, const math::quaternion& q);

	void _Calibrate();
public:
	ArmsTrajectoryGenerator(game::GameEntityManager*m);
	virtual~ArmsTrajectoryGenerator();

	virtual bool InitComponent();
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);

};
DECLARE_GAMECOMPONENT_FACTORY(ArmsTrajectoryGenerator);

}
}


#endif
