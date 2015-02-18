#include "stdafx.h"

#include "CPhysXMaterial.h"
#include "PhysXObjectConverter.h"
#include <NxMaterial.h>


namespace mray{
namespace physics{

#define ToVec3(v)math::vector3d(v.x,v.y,v.z)
#define ToNxVec3(v)NxVec3(v.x,v.y,v.z)

CPhysXMaterial::CPhysXMaterial(const core::string& name){
	m_material=0;
	m_name=name;
}
CPhysXMaterial::~CPhysXMaterial(){
	m_material=0;
}
MaterialID CPhysXMaterial::getMaterialIndex(){
	if(!m_material)
		return -1;
	return m_material->getMaterialIndex();
}

void CPhysXMaterial::setRestitution(float v){
	if(!m_material)return;
	m_material->setRestitution(v);
}
float CPhysXMaterial::getRestitution(){
	if(!m_material)return 0;
	return m_material->getRestitution();
}

void CPhysXMaterial::setDynamicFriction (float v){
	if(!m_material)return;
	m_material->setDynamicFriction(v);
}
float CPhysXMaterial::getDynamicFriction (){
	if(!m_material)return 0;
	return m_material->getDynamicFriction();
}

void CPhysXMaterial::setStaticFriction (float v){
	if(!m_material)return;
	m_material->setStaticFriction(v);
}
float CPhysXMaterial::getStaticFriction (){
	if(!m_material)return 0;
	return m_material->getStaticFriction();
}

void CPhysXMaterial::setDynamicFrictionV (float v){
	if(!m_material)return;
	m_material->setDynamicFrictionV(v);
}
float CPhysXMaterial::getDynamicFrictionV (){
	if(!m_material)return 0;
	return m_material->getDynamicFrictionV();
}
void CPhysXMaterial::setStaticFrictionV (float v){
	if(!m_material)return;
	m_material->setStaticFrictionV(v);
}
float CPhysXMaterial::getStaticFrictionV (){
	if(!m_material)return 0;
	return m_material->getStaticFrictionV();
}

void CPhysXMaterial::setDirOfAnisotropy(const math::vector3d& v){
	if(!m_material)return;
	NxVec3 vx=ToNxVec3(v);
	m_material->setDirOfAnisotropy(vx);
}
math::vector3d CPhysXMaterial::getDirOfAnisotropy(){
	NxVec3 v=m_material->getDirOfAnisotropy();
	return ToVec3(v);
}
void CPhysXMaterial::setMaterial(NxMaterial*m){
	m_material=m;
	if(m_material)
		m_material->userData=this;
}
NxMaterial*CPhysXMaterial::getMaterial(){
	return m_material;
}

void CPhysXMaterial::SaveToDesc(PhysicMaterialDesc* desc)
{
	if(!m_material)
		return;
	NxMaterialDesc d;
	desc->name=m_name;
	m_material->saveToDesc(d);
	PhysXObjectConverter::ConvertMaterialDesc(&d,desc);
}
void CPhysXMaterial::LoadFromDesc(PhysicMaterialDesc* desc)
{
	if(!m_material)
		return;
	NxMaterialDesc d;
	PhysXObjectConverter::ConvertMaterialDesc(desc,&d);

	m_material->loadFromDesc(d);

}

}

}
