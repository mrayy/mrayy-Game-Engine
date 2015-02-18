

/********************************************************************
	created:	2014/01/16
	created:	16:1:2014   22:28
	filename: 	C:\Development\mrayEngine\Projects\AugTel\ArmComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	ArmComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ArmComponent__
#define __ArmComponent__

#include "IGameComponent.h"



namespace mray
{
	namespace VT
	{
		class CoupledJointComponent;
	}
namespace game
{
class ArmComponent :public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	core::string m_prefix;

	VT::CoupledJointComponent* m_shoulder[3];
	VT::CoupledJointComponent* m_foream[3];
	VT::CoupledJointComponent* m_wrist[3];
	VT::CoupledJointComponent* m_fingers[5][4];

	void _load(VT::CoupledJointComponent** m, const core::string&name);
public:
	DECLARE_PROPERTY_TYPE(Prefix, core::string, );
public:

	ArmComponent(game::GameEntityManager*m);
	virtual~ArmComponent();

	virtual bool InitComponent();

	virtual void Update(float dt);

	bool SetPrefix(const core::string &pref){ m_prefix = pref; return true; }
	const core::string& GetPrefix(){ return m_prefix; }


	VT::CoupledJointComponent** GetShoulder(){ return m_shoulder; }
	VT::CoupledJointComponent** GetFoream(){ return m_foream; }
	VT::CoupledJointComponent** GetWrist(){ return m_wrist; }
	VT::CoupledJointComponent* GetFinger(int finger, int part){ if (finger >= 5 || part >= 4)return 0; return m_fingers[finger][part]; }


	void SetForeamAngles(const math::vector3d& angles);
	void SetWristAngles(const math::vector3d& angles);
};
DECLARE_GAMECOMPONENT_FACTORY(ArmComponent);

}
}

#endif
