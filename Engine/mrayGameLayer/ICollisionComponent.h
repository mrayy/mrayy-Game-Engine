

/********************************************************************
	created:	2012/02/23
	created:	23:2:2012   15:01
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\ICollisionComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	ICollisionComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ICollisionComponent__
#define __ICollisionComponent__

//#include "PhysicalShapes.h"

#include "IGameComponent.h"

namespace mray
{
namespace physics
{
	class IPhysicMaterial;
	class IPhysicalShapeDesc;
	class CollisionMask;
	typedef ushort GroupID;
}
namespace game
{

class MRAY_GAME_DLL ICollisionComponent:public IGameComponent
{
	DECLARE_RTTI
private:
protected:
public:
	core::string m_physicalMaterial;
	DECLARE_PROPERTY_TYPE(Name,core::string,);
	DECLARE_PROPERTY_TYPE(PhysicalMaterial,core::string,);
	DECLARE_PROPERTY_TYPE(Density,float,);
	DECLARE_PROPERTY_TYPE(LocalSpace,math::matrix4x4,);
	DECLARE_PROPERTY_TYPE(Mass,float,);
	DECLARE_PROPERTY_TYPE(GroupID,physics::GroupID,);
	DECLARE_PROPERTY_TYPE(Flags,uint,);
public:

	ICollisionComponent();
	virtual~ICollisionComponent();


	virtual bool SetName(const core::string& n);
	virtual const core::string& GetName();

	virtual bool SetPhysicalMaterial(const core::string& n);
	virtual const core::string& GetPhysicalMaterial();

	virtual bool SetDensity(float d);
	virtual float GetDensity();

	virtual bool SetLocalSpace(const math::matrix4x4&m);
	virtual const math::matrix4x4& GetLocalSpace();

	virtual bool SetMaterial(physics::IPhysicMaterial* m);
	virtual physics::IPhysicMaterial* GetMaterial();

	virtual bool SetMass(float m);
	virtual float GetMass();

	virtual bool SetCollisionGroup(physics::GroupID g);
	virtual physics::GroupID GetCollisionGroup();

	virtual bool SetCollisionMask(const physics::CollisionMask& c);
	virtual const physics::CollisionMask& GetCollisionMask();

	virtual bool SetUserData(void* u);
	virtual void* GetUserData();

	//EShapeFlag
	virtual bool SetFlags(uint f);
	virtual uint GetFlags();

	virtual bool CopyShapeDesc(physics::IPhysicalShapeDesc*d)=0;
	virtual physics::IPhysicalShapeDesc* GetShapeDesc()=0;

	xml::XMLElement*  loadXMLSettings(xml::XMLElement* elem);
	xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};


}
}

#endif

