/********************************************************************
	created:	2012/07/13
	created:	13:7:2012   11:15
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\PhysicalMaterialComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	PhysicalMaterialComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PhysicalMaterialComponent___
#define ___PhysicalMaterialComponent___


#include "IPhysical3DDef.h"
#include "IGameComponent.h"
#include "IPhysicMaterial.h"

namespace mray
{
namespace game
{

class  MRAY_GAME_DLL PhysicalMaterialComponent:public IGameComponent
{
	DECLARE_RTTI;
private:
protected:

	physics::PhysicMaterialDesc m_desc;

	GCPtr<physics::IPhysicMaterial> m_material;
public:
	DECLARE_PROPERTY_TYPE(StaticFriction,float,);
	DECLARE_PROPERTY_TYPE(DynamicFriction,float,);
	DECLARE_PROPERTY_TYPE(Restitution,float,);
public:

	PhysicalMaterialComponent(GameEntityManager*mngr);
	virtual~PhysicalMaterialComponent();


	virtual bool InitComponent();

	bool SetDynamicFriction(float v);
	bool SetStaticFriction(float v);
	bool SetRestitution(float v);

	float  GetDynamicFriction(){return m_desc.dynamicFriction;}
	float  GetStaticFriction(){return m_desc.staticFriction;}
	float  GetRestitution(){return m_desc.restitution;}

	physics::IPhysicMaterial* GetMaterial(){return m_material;}
};
DECLARE_GAMECOMPONENT_FACTORY(PhysicalMaterialComponent);

}
}

#endif