
#include "stdafx.h"
#include "JFSpaceShip.h"
#include "JFIWeapon.h"

namespace mray
{
namespace game
{


JFSpaceShip::JFSpaceShip()
{
	m_shooting=false;
	m_brakeOn=false;
}
JFSpaceShip::~JFSpaceShip()
{
}

void JFSpaceShip::ApplySpeed(const math::vector3d& speed)
{
	m_shipSpeed+=speed;
}
void JFSpaceShip::SetShipSpeed(const math::vector3d& speed)
{
	m_shipSpeed=speed;
}
const math::vector3d& JFSpaceShip::GetShipSpeed()
{
	return m_shipSpeed;
}
void JFSpaceShip::Brake(bool on)
{
	m_brakeOn=on;
}
void JFSpaceShip::Turbo(bool on)
{
	m_turboOn=on;
	if(m_turbo<=0)
		m_turbo=0;
}

void JFSpaceShip::Shoot(bool on)
{
	m_shooting=on;
}

float JFSpaceShip::GetMaxEnergy()const
{
	return m_maxEnergy;
}
float JFSpaceShip::GetCurrentEnergy()const
{
	return m_energy;
}
void JFSpaceShip::SetEnergy(float e)
{
	m_energy=e;
}
const JFSpaceShip::WeaponsList& JFSpaceShip::GetWeapons()const
{
	return m_weapons;
}
bool JFSpaceShip::AddWeapon(JFIWeapon* w)
{
	JFIWeapon*w2=GetWeapon(w->GetWeaponType());
	if(w2)
	{
		w2->AddAmmo(w2->GetCurrentAmmo());
	}else
	{
		m_weapons.push_back(w);
		m_weaponsMap[w->GetWeaponType()]=m_weapons.size();
		w->OnAttachedToShip(this);
	}
	return true;
}
void JFSpaceShip::RemoveWeapon(const core::string& wtype)
{
	WeaponMap::iterator it=m_weaponsMap.find(wtype);
	if(it==m_weaponsMap.end())
		return;
	WeaponsList::iterator it2=m_weapons.begin();
	std::advance( it2,it->second);
	delete *it2;
	m_weapons.erase(it2);
	m_weaponsMap.erase(it);
}
JFIWeapon* JFSpaceShip::GetWeapon(const core::string& wtype)
{
	WeaponMap::iterator it=m_weaponsMap.find(wtype);
	if(it==m_weaponsMap.end())
		return 0;
	return m_weapons[it->second];
}
int JFSpaceShip::GetActiveWeapon()
{
	return m_activeWeapon;
}
bool JFSpaceShip::SetActiveWeapon(int i)
{
	if(i>=m_weapons.size())
		return false;
	m_activeWeapon=i;
	return true;
}
void JFSpaceShip::SetNextWeapon()
{
	m_activeWeapon=(m_activeWeapon+1)%m_weapons.size();
}
void JFSpaceShip::SetPrevWeapon()
{
	--m_activeWeapon;
	if(m_activeWeapon<0)
		m_activeWeapon=m_weapons.size()-1;
}

void JFSpaceShip::Update(float dt)
{
	JFObject::Update(dt);

	if(m_brakeOn)
	{
		float len=m_shipSpeed.Length();
		if(len!=0)
		{
			math::vector3d dir=m_shipSpeed/len;
			len=math::Max(0.0f,100.0f*dt);
			m_shipSpeed=dir*len;
		}
		m_speed=m_shipSpeed;
	}else
	{
		if(m_accelState.x==0)
			m_shipSpeed.x-=math::sign(m_shipSpeed.x)*0.5*dt;
		else
			m_shipSpeed.x+=math::sign(m_accelState.x)*m_maxSpeed*dt;
		if(m_accelState.y==0)
			m_shipSpeed.y-=math::sign(m_shipSpeed.y)*0.5*dt;
		else
			m_shipSpeed.y+=math::sign(m_accelState.y)*m_maxSpeed*dt;
		if(m_accelState.z==0)
			m_shipSpeed.z-=math::sign(m_shipSpeed.z)*0.5*dt;
		else
			m_shipSpeed.z+=math::sign(m_accelState.z)*m_maxSpeed*dt;

		float len=m_shipSpeed.Length();
		if(len>m_maxSpeed)
			m_shipSpeed=m_shipSpeed*m_maxSpeed/len;

		m_speed=m_shipSpeed;
		if(m_turboOn)
		{
			m_speed+=100;
			m_turbo-=dt;
			if(m_turbo<=0)
			{
				m_turbo=0;
				m_turboOn=false;
			}
		}
	}

	for(int i=0;i<m_weapons.size();++i)
	{
		m_weapons[i]->Update(dt);
	}
	if(m_shooting && m_activeWeapon!=-1)
	{
		m_weapons[m_activeWeapon]->Shoot();
	}


	m_energy=math::Min(m_energy+m_energyRegeneration*dt,m_maxEnergy);
}



}
}
