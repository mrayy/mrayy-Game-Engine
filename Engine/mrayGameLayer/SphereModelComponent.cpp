

#include "stdafx.h"
#include "SphereModelComponent.h"
#include "MeshGenerator.h"
#include "MeshBufferData.h"




namespace mray
{
namespace game
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(Radius,SphereModelComponent,float,mT("Radius"),EPBT_Basic,mT("Sphere's Radius"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Radius,SphereModelComponent,float,SetRadius,GetRadius,core::StringConverter::toString,core::StringConverter::toFloat,false);


	IMPLEMENT_PROPERTY_TYPE_HEADER(USegments,SphereModelComponent,int,mT("USegments"),EPBT_Basic,mT("Sphere's USegments"),8);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(USegments,SphereModelComponent,int,SetUSegments,GetUSegments,core::StringConverter::toString,core::StringConverter::toInt,false);


	IMPLEMENT_PROPERTY_TYPE_HEADER(VSegments,SphereModelComponent,int,mT("VSegments"),EPBT_Basic,mT("Sphere's VSegments"),8);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(VSegments,SphereModelComponent,int,SetVSegments,GetVSegments,core::StringConverter::toString,core::StringConverter::toInt,false);

SphereModelComponent::SphereModelComponent(GameEntityManager*mngr)
{
	m_radius=PropertyTypeRadius::instance.GetDefaultValue();
	m_uSegments=PropertyTypeUSegments::instance.GetDefaultValue();
	m_vSegments=PropertyTypeVSegments::instance.GetDefaultValue();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeRadius::instance);
		dic->addPropertie(&PropertyTypeUSegments::instance);
		dic->addPropertie(&PropertyTypeVSegments::instance);
	}
}
SphereModelComponent::~SphereModelComponent()
{

}

bool SphereModelComponent::InitComponent()
{
	if(!IPrimitiveModelComponent::InitComponent())
		return false;

	scene::MeshGenerator::getInstance().generateSphere(m_radius,m_uSegments,m_vSegments,m_model->getMesh()->addNewBuffer()->getMeshBuffer());
	
	IPrimitiveModelComponent::_postInit();

	return true;
}

bool SphereModelComponent::SetRadius(const float& v)
{
	m_radius=v;
	return true;
}
const float& SphereModelComponent::GetRadius()const
{
	return m_radius;
}

bool SphereModelComponent::SetUSegments(const int& v)
{
	m_uSegments=v;
	return true;
}
const int& SphereModelComponent::GetUSegments()const
{
	return m_uSegments;
}

bool SphereModelComponent::SetVSegments(const int& v)
{
	m_vSegments=v;
	return true;
}
const int& SphereModelComponent::GetVSegments()const
{
	return m_vSegments;
}



}
}


