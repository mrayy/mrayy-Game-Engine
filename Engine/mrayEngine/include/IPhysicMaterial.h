
/********************************************************************
	created:	2009/01/26
	created:	26:1:2009   1:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicMaterial.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicMaterial
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicMaterial___
#define ___IPhysicMaterial___



#include "mTypes.h"
#include "IPhysical3DDef.h"

namespace mray{
namespace physics{

class IPhysicMaterial
{
public:
	IPhysicMaterial(){}
	virtual~IPhysicMaterial(){}

	virtual MaterialID getMaterialIndex()=0;
	virtual const core::string& getMaterialName()=0;

	virtual void setRestitution(float v)=0;
	virtual float getRestitution()=0;

	virtual void setDynamicFriction (float v)=0;
	virtual float getDynamicFriction ()=0;

	virtual void setStaticFriction (float v)=0;
	virtual float getStaticFriction ()=0;
	
	virtual void setDynamicFrictionV (float v)=0;	//anisotropic dynamic friction coefficient for along the secondary (V) axis of anisotropy. 
	virtual float getDynamicFrictionV ()=0;
	virtual void setStaticFrictionV (float v)=0;
	virtual float getStaticFrictionV ()=0;

	virtual void setDirOfAnisotropy(const math::vector3d& v)=0;	//shape space direction (unit vector) of anisotropy.
	virtual math::vector3d getDirOfAnisotropy()=0;

	virtual void SaveToDesc(PhysicMaterialDesc* desc)=0;
	virtual void LoadFromDesc(PhysicMaterialDesc* desc)=0;
};

}
}


#endif //___IPhysicMaterial___

