

#include "stdafx.h"
#include "VehicleGameComponent.h"
#include "VehicleWheelComponent.h"
#include "ModelComponent.h"
#include "GameEntity.h"
#include "PhysicsComponent.h"
#include "IFileSystem.h"
#include "BoneNode.h"

#include "PhysicalVehicle.h"
#include "VehicleDesc.h"
#include "VehicleGearDesc.h"
#include "VehicleMotorDesc.h"
#include "VehicleWheelDesc.h"
#include "VehicleManager.h"
#include "VehicleMotor.h"
#include "VehicleGear.h"
#include "VehicleXMLParser.h"
#include "IVehicleWheel.h"

#include "GameEntityManager.h"


namespace mray
{
namespace game
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(VehiclePath,VehicleGameComponent,core::string,mT("VehiclePath"),EPBT_Basic,mT("Model's path"),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(VehiclePath,VehicleGameComponent,core::string,SetVehiclePath,GetVehiclePath,,,false);


VehicleGameComponent::VehicleGameComponent(GameEntityManager*m)
{
	m_scale=1;
	m_vehManager=m->GetVehicleManager();
	m_vehicle=0;

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeVehiclePath::instance);
	}
}
VehicleGameComponent::VehicleGameComponent(physics::VehicleManager* cm)
{
	m_scale=1;
	m_vehManager=cm;
	m_vehicle=0;

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeVehiclePath::instance);
	}
}
VehicleGameComponent::~VehicleGameComponent()
{
}

bool VehicleGameComponent::SetScale(float scale)
{
	m_scale=scale;
	return true;
}
float VehicleGameComponent::GetScale()
{
	return m_scale;
}


physics::PhysicalVehicle* VehicleGameComponent::GetModel()
{
	return m_vehicle;
}
IObject* VehicleGameComponent::GetAttachedObject()
{
	return m_vehicle;
}
bool VehicleGameComponent::SetVehiclePath(const core::string& path)
{
	/*
	for(int i=0;i<m_wheels.size();++i)
	{
		this->RemoveComponent(m_wheels[i]);
		delete m_wheels[i];
	}
	m_wheels.clear();*/
	

	m_vehiclePath=path;
// 	game::ModelComponent* modelComp=(game::ModelComponent*)m_ownerEntity->GetFirstComponent(game::ModelComponent::getClassRTTI());
// 	if(!modelComp)
// 		return false;
	game::PhysicsComponent* phNode=(game::PhysicsComponent*)m_ownerEntity->GetFirstComponent(game::PhysicsComponent::getClassType());
	if(!phNode)
		return false;


	physics::VehicleXMLParser parser;
	physics::VehicleDesc *vehicleDesc=parser.LoadVehicleDesc(gFileSystem.openFile(m_vehiclePath));
	if(!vehicleDesc)
		return false;
/*
	core::string bones[]=
	{
		mT("Wheel_FR"),
		mT("Wheel_FL"),
		mT("Wheel_BR"),
		mT("Wheel_BL")
	};
	core::string wheelNames[]=
	{
		mT("FR"),
		mT("FL"),
		mT("BR"),
		mT("BL")
	};

	
	bool hasSkeleton=false;

	if(modelComp->GetModel() && modelComp->GetModel()->getMesh() &&
		modelComp->GetModel()->getMesh()->getSkeleton())
		hasSkeleton=true;
	
	if(hasSkeleton)
	{
		for(int i=0;i<4;++i)
		{
			physics::VehicleWheelDesc* wdesc= vehicleDesc->GetWheel(wheelNames[i]);
			scene::BoneNode*b= modelComp->GetModel()->getMesh()->getSkeleton()->getBone(bones[i]);
			if(wdesc && b)
			{
				wdesc->position=b->getAbsoluteBasePosition()*m_scale;
			}
		}
	}*/
	physics::IPhysicalNode* phnode=phNode->GetPhysicalNode();
	m_vehicle=m_vehManager->CreateVehicle(*vehicleDesc,phnode,m_scale);
	delete vehicleDesc;

	/*

	if(hasSkeleton)
	{
		for(int i=0;i<m_vehicle->GetWheels().size();++i)
		{
			physics::IVehicleWheel*wheel=m_vehicle->GetWheels()[i];
			scene::BoneNode*b= modelComp->GetModel()->getMesh()->getSkeleton()->getBone(bones[i]);
			VehicleWheelComponent* comp=new VehicleWheelComponent();//b,wheel,m_vehicle);

			m_wheels.push_back(comp);

			AddComponent(comp);

		}
	}
*/
	return true;

}

const core::string& VehicleGameComponent::GetVehiclePath()
{
	return m_vehiclePath;
}


}
}

