
#include "stdafx.h"
#include "PrimitiveCollisionComponent.h"


namespace mray
{
namespace game
{


void PrimitiveCollisionComponent::_OnTargetNodeChanged()
{
}
PrimitiveCollisionComponent::PrimitiveCollisionComponent(GameEntityManager*mngr)
{
	m_shapeDesc=0;
}
PrimitiveCollisionComponent::PrimitiveCollisionComponent()
{
	m_shapeDesc=0;
}
PrimitiveCollisionComponent::~PrimitiveCollisionComponent()
{
	delete m_shapeDesc;
}

bool PrimitiveCollisionComponent::InitComponent()
{
	return IGameComponent::InitComponent();
}

bool PrimitiveCollisionComponent::CopyShapeDesc(physics::IPhysicalShapeDesc*d)
{
	return false;
}


void PrimitiveCollisionComponent::SetShapeDesc(physics::IPhysicalShapeDesc*d)
{
	if(m_shapeDesc)
		delete m_shapeDesc;
	m_shapeDesc=d;
}
physics::IPhysicalShapeDesc* PrimitiveCollisionComponent::GetShapeDesc()
{
	return m_shapeDesc;
}

}
}

