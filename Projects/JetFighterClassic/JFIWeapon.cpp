
#include "stdafx.h"
#include "JFIWeapon.h"
#include "JFSpaceShip.h"

namespace mray
{
namespace game
{



JFIWeapon::JFIWeapon(const core::string& wtype)
:m_weaponType(wtype),m_attachedShip(0),m_cooldown(0),m_currentCooldown(0),
m_energyConsumption(0),m_ammo(0),m_infiniteAmmo(0)
{
}
JFIWeapon::~JFIWeapon()
{
}

void JFIWeapon::OnAttachedToShip(JFSpaceShip* s)
{
	m_attachedShip=s;
}

const core::string& JFIWeapon::GetWeaponType()
{
	return m_weaponType;
}

bool JFIWeapon::Shoot()
{
	if(!CanShoot())return false;
	m_currentCooldown=m_cooldown;
	float e= m_attachedShip->GetCurrentEnergy()-m_energyConsumption;
	m_attachedShip->SetEnergy(e);

	if(m_ammo>0)
		--m_ammo;
	m_currentCooldown=m_cooldown;

	_CreateShoot();

	return true;

}
bool JFIWeapon::CanShoot()
{
	if(m_currentCooldown>0 || m_ammo==0 || 
		!m_attachedShip ||  m_attachedShip->GetCurrentEnergy()<m_energyConsumption)
		return false;

	return true;
}
float JFIWeapon::GetCooldown()
{
	return m_cooldown;
}
float JFIWeapon::GetCurrentCooldown()
{
	return m_currentCooldown;
}
float JFIWeapon::GetEnergyConsumption()
{
	return m_energyConsumption;
}

void JFIWeapon::AddAmmo(int count)
{
	if(!m_infiniteAmmo)
		m_ammo+=count;
}
void JFIWeapon::SetCurrentAmmo(int ammo)
{
	if(!m_infiniteAmmo)
		m_ammo=ammo;
}
int JFIWeapon::GetCurrentAmmo()
{
	return m_ammo;
}

bool JFIWeapon::IsInfiniteAmmo()
{
	return m_infiniteAmmo;
}

void JFIWeapon::Update(float dt)
{
	m_currentCooldown=math::Max(0.0f,m_currentCooldown-dt);
}


}
}
