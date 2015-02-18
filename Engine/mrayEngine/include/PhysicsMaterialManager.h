

/********************************************************************
	created:	2012/08/19
	created:	19:8:2012   23:10
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicsMaterialManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicsMaterialManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PhysicsMaterialManager___
#define ___PhysicsMaterialManager___

#include "IPhysical3DDef.h"

namespace mray
{
namespace physics
{
	class IPhysicMaterial;

class MRAY_DLL PhysicsMaterialManager
{
protected:
	std::vector<IPhysicMaterial*> m_materials;
	std::map<MaterialID,uint> m_idMap;
	std::map<core::string,uint> m_nameMap;


public:
	PhysicsMaterialManager();
	virtual~PhysicsMaterialManager();
	

	void RegisterMaterial(physics::IPhysicMaterial* mtrl);
	void UnRegisterMaterial(const core::string& name);

	physics::IPhysicMaterial* GetMaterialByName(const core::string& name);
	physics::IPhysicMaterial* GetMaterialByID(MaterialID id);

	void ClearMaterials();

	const std::vector<IPhysicMaterial*>& GetMaterials();


};

}
}

#endif
