
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
	namespace AugTel
	{
		class IFingerDataProvider;
	}
namespace game
{
	class ArmComponent;
	class HeadCameraComponent;
	class CompositeBoneComponent;
class ArmsTrajectoryGenerator:public game::IGameComponent
{
	DECLARE_RTTI;
protected:

	struct ReferenceFrame
	{
		ReferenceFrame()
		{
			ref = 0;
			node = 0;
		}
		scene::IMovable* ref;
		scene::BoneNode* node;
		math::quaternion space;
	};

	ReferenceFrame m_fingers[2][5][4];

	CompositeBoneComponent* m_backBone;
	HeadCameraComponent* m_head;
	ArmComponent* m_arms[2];

	int m_headTID;
	int m_elbowTID[2];
	int m_wristTID[2];

	AugTel::IFingerDataProvider* m_gloves[2];

	void _CalculateArm(int i);

	bool GetPosOri(int id,math::vector3d& p, math::quaternion& ori);

public:
	ArmsTrajectoryGenerator(game::GameEntityManager*m);
	virtual~ArmsTrajectoryGenerator();

	virtual bool InitComponent();
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);

};
DECLARE_GAMECOMPONENT_FACTORY(ArmsTrajectoryGenerator);

}
}


#endif
