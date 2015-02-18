

#include "Stdafx.h"
#include "BallCreator.h"
#include "GameEntityManager.h"
#include "SceneComponent.h"
#include "PhysicsComponent.h"
#include "MeshRenderableNode.h"
#include "MeshBufferData.h"
#include "MaterialResourceManager.h"

namespace mray
{
namespace VT
{

game::GameEntity* BallCreator::GenerateBall(game::GameEntityManager* mngr,float radius,const math::vector3d& pos)
{
	game::GameEntity* ent=mngr->CreateGameEntity("Ball");
	scene::ISceneNode* node= mngr->GetSceneManager()->createSceneNode();
	game::SceneComponent* scomp=new game::SceneComponent(mngr);
	scomp->SetSceneNode(node);
	game::PhysicsComponent* phcomp=new game::PhysicsComponent(mngr);
	physics::PhysicalNodeDesc phDesc;
	physics::PhysicalBodyDesc bDesc;
	physics::SphereShapeDesc sDesc;
	phDesc.globalPos.setTranslation(pos);
	bDesc.mass=0.001;
	phDesc.bodyDesc=&bDesc;
	sDesc.radius=radius;
	phDesc.shapes.push_back(&sDesc);
	phcomp->SetPhysicalNode(mngr->GetPhysicsManager()->createNode(&phDesc));


	game::PhysicalComponentAttachment *a=new game::PhysicalComponentAttachment();
	a->SetTargetNode(scomp);
	a->SetTargetObject(node);
	phcomp->AttachNode(a);

	scene::MeshRenderableNode* rend=new scene::MeshRenderableNode(0);
	scene::MeshGenerator::getInstance().generateSphere(radius,8,8,rend->getMesh()->addNewBuffer()->getMeshBuffer());
	node->AttachNode(rend);
	video::RenderMaterialPtr mtrl;
	if(math::Randomizer::rand01()>0.5)
		mtrl= gMaterialResourceManager.getMaterial("BlueBallMtrl");
	else
		mtrl= gMaterialResourceManager.getMaterial("RedBallMtrl");
	rend->setMaterial(mtrl,0);

	ent->AddComponent(scomp);
	ent->AddComponent(phcomp);

	return ent;
}

void BallCreator::AutoGenerate(float dt,game::GameEntityManager* mngr,float radius,const math::vector3d& pos)
{
	m_time+=dt;
	if(m_time>0.1f)
	{
		GenerateBall(mngr,radius,pos);
		m_time-=0.1f;
	}
}

}
}
