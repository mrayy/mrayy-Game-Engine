


#include "stdafx.h"
#include "PhysicsMaterialManager.h"
#include "IPhysicMaterial.h"

namespace mray
{
namespace physics
{
	PhysicsMaterialManager::PhysicsMaterialManager()
	{
	}
	PhysicsMaterialManager::~PhysicsMaterialManager()
	{
		ClearMaterials();
	}


	void PhysicsMaterialManager::RegisterMaterial(physics::IPhysicMaterial* mtrl)
	{
		if(!mtrl)
			return;
		m_materials.push_back(mtrl);
		m_idMap[mtrl->getMaterialIndex()]=m_materials.size()-1;
		m_nameMap[mtrl->getMaterialName()]=m_materials.size()-1;
	}
	void PhysicsMaterialManager::UnRegisterMaterial(const core::string& name)
	{
		std::map<core::string,uint>::iterator it=m_nameMap.find(name);
		if(it==m_nameMap.end())
			return;
		MaterialID id=m_materials[it->second]->getMaterialIndex();
		m_materials[it->second]=0;
		m_nameMap.erase(it);
		m_idMap.erase(id);
		
	}

	physics::IPhysicMaterial* PhysicsMaterialManager::GetMaterialByName(const core::string& name)
	{
		std::map<core::string,uint>::iterator it=m_nameMap.find(name);
		if(it==m_nameMap.end())
			return 0;
		return m_materials[it->second];
	}
	physics::IPhysicMaterial* PhysicsMaterialManager::GetMaterialByID(MaterialID id)
	{
		std::map<MaterialID,uint>::iterator it=m_idMap.find(id);
		if(it==m_idMap.end())
			return 0;
		return m_materials[it->second];
	}

	void PhysicsMaterialManager::ClearMaterials()
	{
		m_materials.clear();
		m_idMap.clear();
		m_nameMap.clear();
	}

	const std::vector<IPhysicMaterial*>& PhysicsMaterialManager::GetMaterials()
	{
		return m_materials;
	}



}
}
