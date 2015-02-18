
#include "stdafx.h"

#include "AIComponent.h"
#include "PhysicsComponent.h"
#include "SceneComponent.h"
#include "LightComponent.h"
#include "BoneComponent.h"
#include "ScriptComponent.h"
#include "SoundComponent.h"
#include "ModelComponent.h"
#include "ModelCollisionComponent.h"
#include "PrimitiveCollisionComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleGameComponent.h"
#include "PhysicalJointDOF6Component.h"
#include "KinematicJointDOF6Component.h"
#include "PhysicalJointFixedComponent.h"
#include "SphereModelComponent.h"
#include "ClothComponent.h"
#include "HierarchyComponent.h"
#include "NullPhysicsComponent.h"
#include "SkyBoxComponent.h"

#include "GameComponentCreator.h"
#include "LineRendererComponent.h"


namespace mray
{
namespace game
{


IMPLEMENT_RTTI(IGameComponent,IObject)
IMPLEMENT_RTTI(IMovableComponent,IGameComponent)

IMPLEMENT_RTTI(NullPhysicsComponent,IPhysicsComponent);
IMPLEMENT_RTTI(AIComponent,IGameComponent)
IMPLEMENT_RTTI(IPhysicsComponent,IGameComponent)
IMPLEMENT_RTTI(SceneComponent, IMovableComponent)
IMPLEMENT_RTTI(LightComponent, IMovableComponent)
IMPLEMENT_RTTI(BoneComponent,IMovableComponent)
IMPLEMENT_RTTI(PhysicsComponent,IPhysicsComponent)
IMPLEMENT_RTTI(IPhysicalJointComponent,IGameComponent)
IMPLEMENT_RTTI(PrimitiveCollisionComponent,IGameComponent);
IMPLEMENT_RTTI(ScriptComponent,IGameComponent)
IMPLEMENT_RTTI(SoundComponent,IGameComponent)
IMPLEMENT_RTTI(IModelComponent,IGameComponent)
IMPLEMENT_RTTI(ModelComponent,IModelComponent)
IMPLEMENT_RTTI(IPrimitiveModelComponent,IModelComponent)
IMPLEMENT_RTTI(SphereModelComponent,IPrimitiveModelComponent)
IMPLEMENT_RTTI(ICollisionComponent,IGameComponent)
IMPLEMENT_RTTI(VehicleWheelComponent,IGameComponent);
IMPLEMENT_RTTI(VehicleGameComponent,IGameComponent);
IMPLEMENT_RTTI(HierarchyComponent,IGameComponent);
IMPLEMENT_RTTI(SkyBoxComponent,IGameComponent)

IMPLEMENT_RTTI(LineRendererComponent,IGameComponent);


IMPLEMENT_RTTI(PhysicalJointDOF6Component,IPhysicalJointComponent)
IMPLEMENT_RTTI(PhysicalJointFixedComponent,IPhysicalJointComponent)
IMPLEMENT_RTTI(ClothComponent,IGameComponent);

IMPLEMENT_RTTI(KinematicJointDOF6Component,IPhysical6DOFJointComponent)


IMPLEMENT_RTTI(ModelCollisionComponent,ICollisionComponent)

IMPLEMENT_GAMECOMPONENT_FACTORY(AIComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(SceneComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(LightComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(BoneComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(PhysicsComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(ScriptComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(SoundComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(ModelComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(ModelCollisionComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(VehicleWheelComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(VehicleGameComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(PhysicalJointDOF6Component);
IMPLEMENT_GAMECOMPONENT_FACTORY(KinematicJointDOF6Component);
IMPLEMENT_GAMECOMPONENT_FACTORY(PhysicalJointFixedComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(PrimitiveCollisionComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(SphereModelComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(ClothComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(HierarchyComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(NullPhysicsComponent);
IMPLEMENT_GAMECOMPONENT_FACTORY(SkyBoxComponent)

IMPLEMENT_GAMECOMPONENT_FACTORY(LineRendererComponent);



GameComponentCreator GameComponentCreator::s_instance;
}
}

