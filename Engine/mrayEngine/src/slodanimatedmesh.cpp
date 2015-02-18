
#include "stdafx.h"

#include "SLODAnimatedMesh.h"
#include "ISceneManager.h"
#include "IViewNode.h"

#include "IVideoDevice.h"
#include "MeshResourceManager.h"
#include "MaterialResourceManager.h"
#include "AnimationResourceManager.h"
#include "ShaderResourceManager.h"
#include "XMLTree.h"
#include "IFileSystem.h"
#include "MeshBufferData.h"
#include "CameraNode.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{

	
SLODAnimatedMesh::SLODAnimatedMesh():MeshRenderableNode(0),currLOD(-1)
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeModel::instance);
	}
}

SLODAnimatedMesh::~SLODAnimatedMesh(){
	m_meshs.clear();
}


bool SLODAnimatedMesh::SetModelFileName(const core::string& name)
{
	m_modelFile=name;
	return true;
}
const core::string& SLODAnimatedMesh::GetModelFileName()
{
	return m_modelFile;
}

void SLODAnimatedMesh::addLOD(const SLODMesh& lod){
	addLOD(lod.mesh,lod.distance);
}
void SLODAnimatedMesh::addLOD(const scene::SMeshPtr&mesh,float dist){
	if(!mesh)return;

	m_meshs.push_back(SLODMesh(mesh,dist));
	if(currLOD==-1)
		currLOD=0;
}
SMesh* SLODAnimatedMesh::getMesh(){
	if(currLOD<0 || m_meshs.size()==0)return 0;
	return m_meshs[currLOD].mesh;
}
bool SLODAnimatedMesh::preRender(IRenderPass*pass)
{
	if(!m_meshs.size())
		return false;

	IViewNode*cam=pass->getViewNode();
	if(cam){
		float dist=cam->getViewPos().getDistSQ(m_owner->getAbsolutePosition());
		std::sort(m_meshs.begin(),m_meshs.end());
		//m_meshs.sort();
		currLOD=0;
		for(int i=m_meshs.size()-1;i>=0;i--){
			if(dist>math::sqr<float>(m_meshs[i].distance)){
				currLOD=i;
				break;
			}
		}

	}

	m_mesh=m_meshs[currLOD].mesh;

	return MeshRenderableNode::preRender(pass);
}
void SLODAnimatedMesh::update(float dt){
	MeshRenderableNode::update(dt);
}

void SLODAnimatedMesh::loadAnimation(const core::string&fileName)
{
	GCPtr<animation::IAnimationMixer>mixer=gAnimationResourceManager.getAnimation(fileName);
	if(!mixer)
	{
		return;
	}
	m_animationMixers.push_back(mixer);
	GCPtr<scene::Skeleton> sk= getMesh()->getSkeleton();
	if(sk)
	{
		animation::IAnimationMixer::ATGroupList::const_iterator p=mixer->getTrackGroups().begin();
		animation::IAnimationMixer::ATGroupList::const_iterator end=mixer->getTrackGroups().end();
		for(;p!=end;++p)
		{
			sk->addAnimationTrackGroup( p->second);
		}
	}
}
xml::XMLElement* SLODAnimatedMesh::loadXMLSettings(xml::XMLElement* rootElem)
{
	//xml::XMLElement*sceneNode=elem->getSubElement(mT("IAnimatedMesh"));
	xml::XMLElement* elem= MeshRenderableNode::loadXMLSettings(rootElem);
	if(!elem)return 0;


	xml::XMLAttribute*attr=elem->getAttribute(mT("Model"));
	if(attr && attr->value!=mT("")){
		xml::XMLTree t;
		OS::IStreamPtr stream=gFileSystem.openFile(attr->value);
		if(stream){
			if(t.load(stream)){
				stream->close();
				xml::XMLElement*e=t.getSubElement(mT("ISceneNode"));
				if(e){
					loadXMLSettings(e);
					return elem;
				}
			}
		}
	}


	std::map<core::string,video::RenderMaterialPtr > materialsMap;


	xml::XMLElement* shaders=elem->getSubElement(mT("Shaders"));
	if(shaders)
	{
		gShaderResourceManager.parseShaderXML(shaders);
	}

	xml::XMLElement* mtrls=elem->getSubElement(mT("Materials"));
	if(mtrls)
	{
		xml::XMLElement* m= mtrls->getSubElement(mT("Material"));
		for(;m;m=m->nextSiblingElement(mT("Material")))
		{
			xml::XMLAttribute*attr;
			video::RenderMaterialPtr  mt;
			attr=m->getAttribute(mT("Name"));
			if(attr)
			{
				if(materialsMap.find(attr->value)!=materialsMap.end())
					continue;
				mt=gMaterialResourceManager.getMaterial(attr->value);
			//	if(mt)
			//		mt=mt->Duplicate();
			}
			else
				continue;
			if(!mt){
				mt=new video::RenderMaterial();
				mt->loadXMLSettings(m);
				gMaterialResourceManager.addResource(mt,mt->GetName());
			}
			materialsMap[mt->getResourceName()]=mt;
		}
	}
	xml::XMLElement* lods=elem->getSubElement(mT("LODS"));
	if(lods)
	{
		xml::xmlSubElementsMapIT it= lods->getElementsBegin();
		xml::xmlSubElementsMapIT end= lods->getElementsEnd();
		for(;it!=end;++it)
		{
			SLODMesh lodMesh;
			if((*it)->GetType()!=xml::ENT_Element)continue;
			xml::XMLElement* lod=dynamic_cast<xml::XMLElement*>(*it);
			xml::XMLAttribute*attr;
			attr=lod->getAttribute(mT("Mesh"));
			if(attr)
			{
				lodMesh.mesh=gMeshResourceManager.loadMesh(attr->value,1);
			}
			attr=lod->getAttribute(mT("Distance"));
			if(attr)
			{
				lodMesh.distance=core::StringConverter::toFloat(attr->value);
			}
			if(!lodMesh.mesh)
				continue;
			addLOD(lodMesh);
			xml::xmlSubElementsMapIT eit=lod->getElementsBegin();
			xml::xmlSubElementsMapIT eend=lod->getElementsEnd();

			for(;eit!=eend;++eit){
				if((*eit)->GetType()!=xml::ENT_Element)continue;
				xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
				if(e->getName().equals_ignore_case(mT("Material"))){
					attr=e->getAttribute(mT("Submodel"));
					if(!attr)
						continue;
					int i=lodMesh.mesh->getBufferIndex(attr->value.c_str());
					if(i==-1)
						continue;
					attr=e->getAttribute(mT("Name"));
					if(!attr)
						continue;
					scene::MeshBufferData* bd=lodMesh.mesh->getBufferData(i);
					if(materialsMap.find(attr->value)!=materialsMap.end())
						bd->setMaterial(materialsMap[attr->value]);
					else {
						video::RenderMaterial*m= gMaterialResourceManager.getMaterial(attr->value);
						if(m)
							m=m->Duplicate();

						bd->setMaterial(m);
					}
				}
			}
		}
	}

	xml::XMLElement* anims=elem->getSubElement(mT("Animations"));
	if(anims)
	{
		xml::XMLElement* anim=anims->getSubElement(mT("AnimationTag"));
		for(;anim;anim=anim->nextSiblingElement(mT("AnimationTag")))
		{
			xml::XMLAttribute*attr=anim->getAttribute(mT("File"));
			if(attr)
			{
				loadAnimation(attr->value);
			}
		}
	}
/*

	for(int i=0;i<m_meshs.size();++i)
	{
		xml::XMLElement* lod=new xml::XMLElement(mT("LOD"));
		lod->addAttribute(mT("Mesh"),m_meshs[i].mesh->getResourceName());
		lod->addAttribute(mT("Distance"),core::StringConverter::toString(m_meshs[i].distance));
		xml::XMLElement* mtrl=new xml::XMLElement(mT("Material"));

		for(int j=0;j<m_meshs[i].mesh->getBuffersCount();++j)
		{
			scene::MeshBufferData*bd=m_meshs[i].mesh->getBufferData(i);
			if(!bd)
				continue;
			mtrl->addAttribute(mT("submodel"),bd->getMeshBuffer()->getMeshName());
			if(bd->getMaterial())
				mtrl->addAttribute(mT("name"),bd->getMaterial()->getResourceName());
		}

		lod->addSubElement(mtrl);
		lods->addSubElement(lod);
	}*/

	return elem;

}
xml::XMLElement* SLODAnimatedMesh::exportXMLSettings(xml::XMLElement*elem)
{
	xml::XMLElement* e=MeshRenderableNode::exportXMLSettings(elem);

	if(m_modelFile!=mT(""))
		return e;

	std::map<core::string,video::GPUShader*> usedShaders;

	for(int i=0;i<getMaterialCount();++i)
	{
		if(getMaterial(i)){
			video::RenderTechnique* t= getMaterial(i)->GetActiveTechnique();
			const video::RenderPassList& lst=t->GetPasses();
			for(int i=0;i<lst.size();++i)
			{
				video::GPUShader*s=lst[i]->getRenderShader();
				if(s)
					usedShaders[s->GetName()]=s;
			}
		}
	}

	if(usedShaders.size()){
		std::map<core::string,video::GPUShader*>::iterator it=usedShaders.begin();

		xml::XMLElement* shaders=new xml::XMLElement(mT("Shaders"));
		e->addSubElement(shaders);
		for(;it!=usedShaders.end();++it)
		{
			video::GPUShader* s=it->second;
			s->exportXMLSettings(shaders);
		}
	}

	xml::XMLElement* mtrls=new xml::XMLElement(mT("Materials"));
	e->addSubElement(mtrls);
	for(int i=0;i<getMaterialCount();++i)
	{
		if(getMaterial(i))
		{
			//xml::XMLElement* m=new xml::XMLElement(mT("MaterialTag"));
			//mtrls->addSubElement(m);
			//m->addAttribute("Name",getMaterial(i)->getResourceName());

			getMaterial(i)->exportXMLSettings(mtrls);
		}
	}

	xml::XMLElement* lods=new xml::XMLElement(mT("LODS"));
	e->addSubElement(lods);
	for(int i=0;i<m_meshs.size();++i)
	{
		xml::XMLElement* lod=new xml::XMLElement(mT("LOD"));
		lod->addAttribute(mT("Mesh"),m_meshs[i].mesh->getResourceName());
		lod->addAttribute(mT("Distance"),core::StringConverter::toString(m_meshs[i].distance));

		for(int j=0;j<m_meshs[i].mesh->getBuffersCount();++j)
		{
			xml::XMLElement* mtrl=new xml::XMLElement(mT("Material"));
			scene::MeshBufferData*bd=m_meshs[i].mesh->getBufferData(j);
			if(!bd)
				continue;
			mtrl->addAttribute(mT("Submodel"),bd->getMeshBuffer()->getMeshName());
			if(bd->getMaterial())
				mtrl->addAttribute(mT("Name"),bd->getMaterial()->getResourceName());
			lod->addSubElement(mtrl);
		}

		lods->addSubElement(lod);
	}
	if(m_animationMixers.size())
	{
		xml::XMLElement*anims=new xml::XMLElement(mT("Animations"));
		for(int i=0;i<m_animationMixers.size();++i)
		{
			xml::XMLElement*e2=new xml::XMLElement(mT("AnimationTag"));
			e2->addAttribute(mT("File"),m_animationMixers[i]->getResourceName());
			anims->addSubElement(e2);
		}
		e->addSubElement(anims);
	}


	return e;
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Model,SLODAnimatedMesh,string,mT("Model"),EPBT_Basic,mT("Model name"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Model,SLODAnimatedMesh,core::string,SetModelFileName,GetModelFileName,,,false)



}
}
