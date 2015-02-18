

/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   15:57
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFIBullet.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFIBullet
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFIBullet__
#define __JFIBullet__


namespace mray
{
namespace game
{
	/*
class JFIBullet;

class JFIBulletInformation
{
	float m_damage;
	bool m_aoe;
public:
	virtual float GetDamage();
	virtual bool IsAOE();

	virtual JFIBullet* CreateBullet(uint m_mask)=0;
};
*/
class JFIBullet
{
private:
protected:

	uint m_mask;
public:
	JFIBullet(uint m_mask);
	virtual~JFIBullet();

	virtual void OnCreate(const math::vector3d& pos,const math::vector3d& dir);

	virtual void Update(float dt);

};

}
}

#endif
