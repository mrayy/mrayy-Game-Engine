

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
	namespace game
	{
		class IMovableComponent;
	}
namespace AugTel
{
class ArmComponent :public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	core::string m_prefix;

	core::string m_shoulderName;
	core::string m_armBaseName;
	core::string m_armName;
	core::string m_wristName;
	core::string m_wristBaseName;

	game::IMovableComponent* m_shoulder;
	game::IMovableComponent* m_foreamBase;
	game::IMovableComponent* m_foream;
	game::IMovableComponent* m_wrist;
	game::IMovableComponent* m_wristBase;
	game::IMovableComponent* m_fingers[5][4];

	float m_armLength;
	float m_shoulderLength;

	void _load(game::IMovableComponent** m, const core::string&name);
public:
	DECLARE_PROPERTY_TYPE(ShoulderName, core::string, );
	DECLARE_PROPERTY_TYPE(ArmBaseName, core::string, );
	DECLARE_PROPERTY_TYPE(ArmName, core::string, );
	DECLARE_PROPERTY_TYPE(WristName, core::string, );
	DECLARE_PROPERTY_TYPE(WristBaseName, core::string, );
	DECLARE_PROPERTY_TYPE(Prefix, core::string, );
	
	DECLARE_PROPERTY_TYPE(ArmLength, float, );
	DECLARE_PROPERTY_TYPE(ShoulderLength, float, );

public:

	ArmComponent(game::GameEntityManager*m);
	virtual~ArmComponent();

	virtual bool InitComponent();

	virtual void Update(float dt);
	DECLARE_SETGET_PROP(ShoulderName, core::string);
	DECLARE_SETGET_PROP(ArmBaseName, core::string);
	DECLARE_SETGET_PROP(ArmName, core::string);
	DECLARE_SETGET_PROP(WristName, core::string);
	DECLARE_SETGET_PROP(WristBaseName, core::string);
	DECLARE_SETGET_PROP(Prefix, core::string);
	
	bool SetArmLength(float l);
	float GetArmLength(){ return m_armLength; }

	bool SetShoulderDistance(float l);
	float GetShoulderDistance(){ return m_shoulderLength; }

	game::IMovableComponent* GetShoulder(){ return m_shoulder; }
	game::IMovableComponent* GetFoream(){ return m_foream; }
	game::IMovableComponent* GetWrist(){ return m_wrist; }
	game::IMovableComponent* GetWristBase(){ return m_wristBase; }
	game::IMovableComponent* GetFinger(int finger, int part){ if (finger >= 5 || part >= 4)return 0; return m_fingers[finger][part]; }


	void SetForeamAngles(const math::vector3d& angles);
	void SetWristAngles(const math::vector3d& angles);
};
DECLARE_GAMECOMPONENT_FACTORY(ArmComponent);

}
}

#endif
