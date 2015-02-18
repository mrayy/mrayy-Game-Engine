


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   14:39
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFSpaceShip.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFSpaceShip
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFSpaceShip__
#define __JFSpaceShip__

#include "JFObject.h"

namespace mray
{
namespace game
{
	class JFIWeapon;

class JFSpaceShip:public JFObject
{
	DECLARE_RTTI;
public:
	typedef std::vector<JFIWeapon*> WeaponsList;
private:
protected:
	typedef std::map<core::string,int> WeaponMap;

	WeaponsList m_weapons;
	WeaponMap m_weaponsMap;

	math::vector3d m_accelState;

	int m_activeWeapon;

	float m_maxEnergy;
	float m_energy;
	float m_energyRegeneration;

	float m_maxSpeed;

	float m_maxTurbo;
	float m_turbo;

	math::vector3d m_shipSpeed;
	bool m_turboOn;
	bool m_brakeOn;
	bool m_shooting;
public:
	JFSpaceShip();
	virtual~JFSpaceShip();

	virtual void SetAccelState(const math::vector3d& state){m_accelState=state;}
	virtual const math::vector3d& GetAccelState(){return m_accelState;}

	virtual void ApplySpeed(const math::vector3d& speed);
	virtual void SetShipSpeed(const math::vector3d& speed);
	virtual const math::vector3d& GetShipSpeed();
	virtual void Brake(bool on);
	virtual void Turbo(bool on);
	virtual void Shoot(bool on);

	float GetMaxEnergy()const;
	float GetCurrentEnergy()const;
	void SetEnergy(float e);

	const WeaponsList& GetWeapons()const;
	bool AddWeapon(JFIWeapon* w);
	void RemoveWeapon(const core::string& wtype);
	JFIWeapon* GetWeapon(const core::string& wtype);

	int GetActiveWeapon();
	bool SetActiveWeapon(int i);
	void SetNextWeapon();
	void SetPrevWeapon();

	virtual void Update(float dt);

};

}
}

#endif
