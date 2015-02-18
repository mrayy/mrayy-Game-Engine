
/********************************************************************
	created:	2009/01/26
	created:	26:1:2009   1:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\CPhysXMaterial.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	CPhysXMaterial
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CPhysXMaterial___
#define ___CPhysXMaterial___

#include <IPhysicMaterial.h>
#include "compileConfig.h"

class NxMaterial;

namespace mray{
namespace physics{

class MRAY_PHYSX_DLL CPhysXMaterial:public IPhysicMaterial
{
protected:
	NxMaterial  *m_material;
	core::string m_name;
public:
	CPhysXMaterial(const core::string& name);
	virtual~CPhysXMaterial();

	virtual MaterialID getMaterialIndex();
	virtual const core::string& getMaterialName(){return m_name;}

	virtual void setRestitution(float v);
	virtual float getRestitution();

	virtual void setDynamicFriction (float v);
	virtual float getDynamicFriction ();

	virtual void setStaticFriction (float v);
	virtual float getStaticFriction ();

	virtual void setDynamicFrictionV (float v);	//anisotropic dynamic friction coefficient for along the secondary (V) axis of anisotropy. 
	virtual float getDynamicFrictionV ();
	virtual void setStaticFrictionV (float v);
	virtual float getStaticFrictionV ();

	virtual void setDirOfAnisotropy(const math::vector3d& v);	//shape space direction (unit vector) of anisotropy.
	virtual math::vector3d getDirOfAnisotropy();

	virtual void SaveToDesc(PhysicMaterialDesc* desc);
	virtual void LoadFromDesc(PhysicMaterialDesc* desc);

	void setMaterial(NxMaterial*m);
	NxMaterial*getMaterial();
};

}

}


#endif //___CPhysXMaterial___
