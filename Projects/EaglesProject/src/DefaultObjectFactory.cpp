
#include <SAnimatedMesh.h>
#include <MeshResourceManager.h>
#include <MaterialResourceManager.h>
#include <SMeshManipulator.h>

#include "TextureManager.h"
#include "DefaultObjectFactory.h"
#include "ObjectFactory.h"
#include "StaticEntity.h"
#include "TreeEntity.h"
#include "BuildingEntity.h"



namespace mray{
namespace gameMod{

//////////////////////////////////////////////////////////////////////////

class BuildingBase_Creator:public IObjectCreator
{
	core::string m_type;
	int m_base;
public:
	BuildingBase_Creator(int base){
		m_type=mT("BuildingBase");
		m_type+=base;
		m_base=base;
		//precache destroyed model
		core::string name=mT("BuildingBaseDest")+m_type;
		name+=mT(".mdl");

		GCPtr<scene::SMesh> mesh=gMeshResourceManager.loadMesh(name);
	}
	virtual~BuildingBase_Creator(){}

	virtual const core::string& getObjectClass(){
		return m_type;
	}
	virtual GCPtr<GameEntity> createEntity(int type){
		core::string meshFileName=m_type+mT(".mdl");

		core::string meshName=m_type+mT("/");
		meshName+=type;

		GCPtr<scene::SMesh>m=gMeshResourceManager.getResource(meshName);
		GCPtr<scene::SMesh>m2;
		GCPtr<video::SMaterial> mtrl;

		bool alreadyExist=false;

		if(!m){
			m=gMeshResourceManager.loadMesh(meshFileName);

			if(!m)
				return 0;
			mtrl= gMaterialResourceManager.getMaterial(mT("Building_mtrl"));
			m2=m->duplicateContextOnly();
		}else{
			m2=m;
			alreadyExist=true;
		}

		if(!m)
			return 0;

		GCPtr<scene::SAnimatedMesh> node=new scene::SAnimatedMesh(0,0,1,math::quaternion::Identity,0);

		node->ShadowCaster=true;
		node->ShadowReciver=true;

		if(!alreadyExist){
			GCPtr<scene::IMeshBuffer>mb=m2->getBuffer(0);
			if(mtrl){
				mtrl=mtrl->duplicate();
			}else if(mb)
				mtrl=mb->getMaterial()->duplicate();
			if(mtrl){
				core::string tp=m_type+mT("\\b (");
				tp+=type;
				tp+=mT(").tga");

				mtrl->setTexture(TextureManager::getInstance().loadTexture2D(tp),0);

				if(mb){
					mb->setMaterial(mtrl);
				}
			}

			gMeshResourceManager.addResource(m2,meshName);
		}else{
			scene::SMeshManipulator::getInstance().recalcNormalsFlat(m2);
		}

		node->setMesh(m2);


		GCPtr<StaticEntity> ent=new BuildingEntity(node,mT("BuildingBase"),m_base);

		return ent;
	}
};

//////////////////////////////////////////////////////////////////////////

// 1->6 ~
class TreeBase_Creator:public IObjectCreator
{
	core::string m_type;
public:
	TreeBase_Creator(int base){
		m_type=mT("TreeBase");
		m_type+=base;
	}
	virtual~TreeBase_Creator(){}

	virtual const core::string& getObjectClass(){
		return m_type;
	}
	virtual GCPtr<GameEntity> createEntity(int type){
		core::string meshFileName=m_type+mT(".mdl");

		core::string meshName=m_type+mT("/");
		meshName+=type;

		GCPtr<scene::SMesh>m=gMeshResourceManager.getResource(meshName);
		GCPtr<scene::SMesh>m2;
		GCPtr<video::SMaterial> mtrl;

		bool alreadyExist=false;

		if(!m){
			m=gMeshResourceManager.loadMesh(meshFileName);

			if(!m)
				return 0;
			mtrl= gMaterialResourceManager.getMaterial(mT("Tree_mtrl"));
			m2=m->duplicateContextOnly();
		}else{
			m2=m;
			alreadyExist=true;
		}

		if(!m)
			return 0;

		GCPtr<scene::SAnimatedMesh> node=new scene::SAnimatedMesh(0,0,1,math::quaternion::Identity,0);

		if(!alreadyExist){
			GCPtr<scene::IMeshBuffer>mb=m2->getBuffer(0);
			if(mtrl){
				mtrl=mtrl->duplicate();
			}else if(mb)
				mtrl=mb->getMaterial()->duplicate();
			if(mtrl){
				core::string tp=mT("Trees\\T(");
				tp+=type;
				tp+=mT(").tga");

				mtrl->setTexture(TextureManager::getInstance().loadTexture2D(tp),0);

				if(mb){
					mb->setMaterial(mtrl);
				}
			}

			gMeshResourceManager.addResource(m2,meshName);
		}

		node->setMesh(m2);


		GCPtr<StaticEntity> ent=new TreeEntity(node);

		return ent;
	}
};


void DefaultObjectFactory::installObjects(){
	ObjectFactory::getInstance().addObjectCreator(new BuildingBase_Creator(0));
	ObjectFactory::getInstance().addObjectCreator(new BuildingBase_Creator(1));

	for(int i=0;i<6;i++)
		ObjectFactory::getInstance().addObjectCreator(new TreeBase_Creator(i));
}
void DefaultObjectFactory::uninstallObjects(){
}


}
}
