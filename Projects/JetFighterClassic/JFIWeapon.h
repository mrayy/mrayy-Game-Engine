


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   14:53
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFIWeapon.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFIWeapon
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFIWeapon__
#define __JFIWeapon__


namespace mray
{
namespace game
{
	class JFSpaceShip;

class JFIWeapon:public IObject
{
	DECLARE_RTTI;
private:
protected:

	core::string m_weaponType;

	JFSpaceShip* m_attachedShip;

	float m_cooldown;				// time to shoot
	float m_currentCooldown;		// time to shoot next missile

	float m_energyConsumption;
	int m_ammo;						// -1 for infinite ammo
	bool m_infiniteAmmo;			// if true, then ammo is -1


	virtual void _CreateShoot()=0; // weapon specific implementation
public:

	JFIWeapon(const core::string& wtype);
	virtual~JFIWeapon();

	void OnAttachedToShip(JFSpaceShip* s);

	virtual const core::string& GetWeaponType();

	virtual bool Shoot();
	virtual bool CanShoot();

	// get in milliseconds when we can shoot 
	float GetCooldown();
	float GetCurrentCooldown();

	// how much energy does this weapon consumes
	float GetEnergyConsumption();

	void AddAmmo(int count);
	void SetCurrentAmmo(int ammo);
	int GetCurrentAmmo();

	bool IsInfiniteAmmo();

	virtual void Update(float dt);
};

}
}

#endif
