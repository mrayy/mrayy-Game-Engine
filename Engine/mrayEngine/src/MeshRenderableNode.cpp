#include "stdafx.h"

#include "MeshRenderableNode.h"
#include "IVideoDevice.h"
#include "ISceneManager.h"
#include "Engine.h"

#include "RenderOperation.h"
#include "RenderManager.h"
#include "MeshBufferData.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{

	
MeshRenderableNode::MeshRenderableNode(const SMeshPtr& mesh)
{
	if(mesh)
		setMesh(mesh);
	else{
		m_mesh=new SMesh();
	}
	m_hasDrown=false;
	m_drawInCustomLayer=false;
	m_customLayer=0;
	m_customRender=false;
}
MeshRenderableNode::~MeshRenderableNode(){
	m_mesh=0;
}
bool MeshRenderableNode::isDrawInCustomLayer()
{
	return m_drawInCustomLayer;
}
void MeshRenderableNode::setDrawInCustomLayer(bool s)
{
	m_drawInCustomLayer=s;
}

void MeshRenderableNode::setCustomLayer(uint l)
{
	m_customLayer=l;
}
uint MeshRenderableNode::getCustomLayer()
{
	return m_customLayer;
}

void MeshRenderableNode::setMesh(const SMeshPtr& mesh){
	m_mesh=mesh;
}

bool MeshRenderableNode::preRender(IRenderPass*pass)
{
	m_hasDrown=false;
	if(m_owner)
		m_hasDrown=true;
	return false;
}
bool MeshRenderableNode::render(IRenderPass*pass)
{
	return false;
	/*
	if(!m_owner || !pass->canRenderObject(this) )
		return false;
	video::IVideoDevice*dev=Engine::getInstance().getDevice();

	if(!dev)
		return;

	m_hasDrown=true;
	dev->setTransformationState(video::TS_WORLD,m_owner->getAbsoluteTransformation());
	SMesh* mesh=getMesh();
	for (int i=0;i<mesh->getBuffersCount();++i)
	{
		MeshBufferData* buffD=mesh->getBufferData(i);
		IMeshBuffer* buff=buffD->getMeshBuffer();
		video::IShaderConstantsCallback* cb=buffD->getShaderCallback();


		video::RenderMaterial*mr=buffD->getMaterial();
		if(pass->useMaterial(mr))
			dev->useMaterial(mr);
		if(mr->getRenderShader() && cb)
			cb->setConstants(mr->getRenderShader(),video::IShaderConstantsCallback::ESUT_Both);

		dev->drawSingleMesh(buff);
	}*/
}

void MeshRenderableNode::update(float dt)
{
}
void MeshRenderableNode::AddUpdateJobs(ThreadJobManager* mngr,float dt)
{
	if(getMesh() && m_hasDrown){
		//getMesh()->update(dt);
		getMesh()->AddUpdateJobs(mngr,dt);
	}
	m_hasDrown=false;
}


void MeshRenderableNode::setMaterial(const video::RenderMaterialPtr& mtrl,int i){
	if(!mtrl)
		return;
	if(i<getMesh()->getBuffersCount())
		getMesh()->getBufferData(i)->setMaterial(mtrl);
}
const video::RenderMaterialPtr&  MeshRenderableNode::getMaterial(int i){
	if(i>=getMesh()->getBuffersCount())
		return video::RenderMaterialPtr::Null;
	return getMesh()->getBufferData(i)->getMaterial();
}
int MeshRenderableNode::getMaterialCount(){
	return getMesh()->getBuffersCount();
}

void MeshRenderableNode::setShader(int mesh,const video::GPUShaderPtr& shader)
{
	video::RenderMaterial* m= getMaterial(mesh);
	if(m)
		m->GetActiveTechnique()->GetPassAt(0)->setRenderShader(shader);
}
void MeshRenderableNode::setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback)
{
	MeshBufferData* bd= getMesh()->getBufferData(mesh);
	if(bd)
		bd->setShaderCallback(callback);
}
const video::GPUShaderPtr& MeshRenderableNode::getShader(int i)
{
	video::RenderMaterial* m= getMaterial(i);
	if(m)
		return m->GetActiveTechnique()->GetPassAt(0)->getRenderShader();
	return video::GPUShaderPtr::Null;
}

SMesh* MeshRenderableNode::getMesh(){
	return m_mesh;
}
math::box3d MeshRenderableNode::getBoundingBox()
{
	if(getMesh())
		return getMesh()->getBoundingBox();
	return math::box3d(0,0,0,0,0,0);
}


}
}