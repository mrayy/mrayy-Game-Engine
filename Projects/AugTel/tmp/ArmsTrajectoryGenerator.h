
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
class ArmsTrajectoryGenerator:public game::IGameComponent
{
	DECLARE_RTTI;
protected:

	struct ReferenceFrame
	{
		scene::IMovable* ref;
		scene::BoneNode* node;
		math::quaternion space;
	};

	ReferenceFrame m_fingers[2][5][4];

	HeadCameraComponent* m_head;
	ArmComponent* m_arms[2];

	animation::IterativeIK* m_armsIK[2];

	math::vector3d m_targetPos[2];
	math::quaternion m_targetOri[2];
	math::vector3d m_origPos[2];

	AugTel::IFingerDataProvider* m_gloves[2];


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
