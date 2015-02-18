


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   14:44
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFObject.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFObject__
#define __JFObject__


namespace mray
{
namespace game
{
	class JFObject;

class IJFObjectListener
{
public:
	virtual void OnDamageReceived(JFObject*obj,float damage){};
	virtual void OnDestroyed(JFObject*obj){};

};

class JFObject:public IGameComponent,public ListenerContainer<IJFObjectListener>
{
	DECLARE_RTTI;
private:
protected:

	float m_maxHealth;
	float m_health;
	math::vector3d m_speed;

	float m_defense;

	uint m_mask;

	bool m_Kinematic;

	bool m_immune;

	DECLARE_FIRE_METHOD(OnDamageReceived,(JFObject*obj,float damage),(obj,damage));
	DECLARE_FIRE_METHOD(OnDestroyed,(JFObject*obj),(obj));
public:
	JFObject();
	virtual~JFObject();

	const math::vector3d& GetCurrentSpeed()const{return m_speed;}

	math::vector3d GetPosition();
	void SetPosition(const math::vector3d& pos);

	math::quaternion GetOrientation();
	void SetOrientation(const math::quaternion& ori);

	math::vector3d GetScale();
	void SetScale(const math::vector3d& s);

	void MakeKinematic(bool k){m_Kinematic=k;}

	void SetImmune(bool immune);
	bool IsImmune()const;


	void ReceivedDamage(float power);

	virtual void OnJFCreate();

	virtual void OnJFDestroy();

	virtual void Update(float dt);

	virtual void OnAttachedToComponent(IObjectComponent* owner);
};


}
}

#endif
