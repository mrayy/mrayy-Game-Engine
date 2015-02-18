

#include "BuildingEntity.h"
#include "GameLevelManager.h"
#include "IEaglesManager.h"

#include <MeshResourceManager.h>
#include <MeshSceneNode.h>
#include <TextureBank.h>
#include <BankAnimatedSprite.h>
#include <TextureResourceManager.h>



namespace mray{
namespace gameMod{

BuildingEntity::BuildingEntity(GCPtr<scene::ISceneNode> shape,const core::string&base,int type)
:StaticEntity(shape)
{
	m_exploded=false;
	m_base=base;
	m_type=type;
}
BuildingEntity::~BuildingEntity(){
}

void BuildingEntity::hasCollide(){
	if(m_exploded)
		return;
	m_exploded=true;

	core::string name=m_base+mT("Dest")+m_type;
	name+=mT(".mdl");

	GCPtr<scene::SMesh> mesh=gMeshResourceManager.loadMesh(name);
	if(mesh){
		((scene::MeshSceneNode*)m_shape.pointer())->setMesh(mesh);
	}

	GCPtr<video::TextureBank> bank=new video::TextureBank();
	for(int i=0;i<14;i++){
		core::string name=mT("fire/(");
		name+=core::StringConverter::toString(i);
		name+=mT(").png");
		bank->addTexture(gTextureResourceManager.loadTexture2D(name));
	}
	//for(int i=0;i<4;i++)
	{
		GCPtr<scene::BankAnimatedSprite>m_explosion=new scene::BankAnimatedSprite(bank,0,0,math::quaternion::Identity,20,0);
		// 			m_explosion->position().x=(math::Randomizer::rand01()*2-1)*10;
		// 			m_explosion->position().y=(math::Randomizer::rand01()*2-1)*10;
		// 			m_explosion->position().z=(math::Randomizer::rand01()*2-1)*10;
		// 			m_explosion->scale=(math::Randomizer::rand01()*2-1)*30+20;

		GCPtr<GameLevel>m_level=GameLevelManager::getInstance().getCurrentLevel();

		m_explosion->SetSceneManager(m_level->getSceneManager());
		m_explosion->setDevice(eagles::IEaglesManager::getInstance().getVideoDevice());
		m_explosion->setAutoAnimate(1);
		m_explosion->setAutoReplay(true);
		m_explosion->setSpeed(100);
		m_explosion->getMaterial(0)->setMaterialRenderer(video::MRT_ADD);
		m_explosion->getMaterial(0)->setRenderState(video::RS_Lighting,video::ES_DontUse);
		m_explosion->getMaterial(0)->setRenderState(video::RS_ZWrite,video::ES_DontUse);
		m_shape->addChild(m_explosion);
		m_explosion->position().y=10;

		//m_shape->addChild(m_explosion);
	}
}

}
}