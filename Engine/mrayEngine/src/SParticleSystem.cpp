#include "stdafx.h"


#include "SParticleSystem.h"
#include "ISceneManager.h"


#include "ParticleEmitter.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

#include "RenderOperation.h"
#include "RenderManager.h"

#include "ILogManager.h"
#include "XMLTree.h"
#include "IFileSystem.h"
#include "IViewNode.h"

namespace mray{
namespace scene{

SParticleSystem::SParticleSystem()
:IRenderable()
{
	SetTargetRenderGroup(RGH_Transparent);
	SetHasCustomRenderGroup(true);
}


SParticleSystem::~SParticleSystem()
{
	ClearEmitters();

}



ParticleEmitter* SParticleSystem::CreateEmitter()
{
	ParticleEmitter* e=new ParticleEmitter();
	AddEmitter(e);
	return e;
}
void SParticleSystem::AddEmitter(ParticleEmitter*e)
{
	e->SetSystem(this);
	m_emitters.push_back(e);
}


int SParticleSystem::GetEmittersCount()
{
	return m_emitters.size();
}


ParticleEmitter* SParticleSystem::GetEmitter(int i)
{
	if(i>=m_emitters.size())return 0;
	return m_emitters[i];
}


void SParticleSystem::RemoveEmitter(int i)
{
	if(i>=m_emitters.size())return;
	delete m_emitters[i];
	m_emitters.erase(m_emitters.begin()+ i);
}



void SParticleSystem::ClearEmitters()
{
	for(int i=0;i<m_emitters.size();++i)
		delete m_emitters[i];
	m_emitters.clear();
}


bool SParticleSystem::preRender(IRenderPass*pass)
{
	for(int i=0;i<m_emitters.size();++i)
	{
		m_emitters[i]->preRender(pass);
	}

	return false;
}
bool SParticleSystem::render(IRenderPass*pass)
{
	const math::Frustum* f=pass->getViewNode()->getViewFrustrum();
	for(int i=0;i<m_emitters.size();++i)
	{
		if(f->boxInFrustrum(m_emitters[i]->GetBoundingBox()))
			m_emitters[i]->render(pass);
	}
	return true;
}


void SParticleSystem::update(float dt)
{
	for(int i=0;i<m_emitters.size();++i)
	{
		m_emitters[i]->update(dt);
	}
}

void SParticleSystem::AddUpdateJobs(ThreadJobManager* mngr,float dt)
{
	for(int i=0;i<m_emitters.size();++i)
	{
		m_emitters[i]->AddUpdateJobs(mngr,dt);
	}
}

void SParticleSystem::setMaterial(const video::RenderMaterialPtr& mtrl,int i)
{
	return m_emitters[i]->setMaterial(mtrl);
}
const video::RenderMaterialPtr& SParticleSystem::getMaterial(int i)
{
	return m_emitters[i]->getMaterial();
}
int SParticleSystem::getMaterialCount()
{
	return m_emitters.size();
}

void SParticleSystem::setShader(int mesh,const video::GPUShaderPtr& shader)
{
	getMaterial(mesh)->GetActiveTechnique()->GetPassAt(0)->setRenderShader(shader);
}
void SParticleSystem::setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback)
{
}

const video::GPUShaderPtr& SParticleSystem::getShader(int i)
{
	return video::GPUShaderPtr::Null;
}
math::box3d SParticleSystem::getBoundingBox()
{
	math::box3d box;
	if(m_emitters.size())
		box=m_emitters[0]->GetBoundingBox();
	for (int i=1;i<m_emitters.size();++i)
	{
		box.AddBox(m_emitters[i]->GetBoundingBox());
	}
	return box;
}
ParticleEmitter* SParticleSystem::LoadEmitter(xml::XMLElement*elem)
{
//	xml::XMLAttribute*attr;
	/*
	attr=elem->getAttribute(mT("Type"));
	if(!attr)
	{
		return 0;
	}*/
	ParticleEmitter* emitter= new ParticleEmitter();// 
	emitter->LoadFromXML(elem);
	AddEmitter(emitter);

	return emitter;
}

ParticleEmitter* SParticleSystem::LoadEmitter(const core::string&path)
{
	xml::XMLTree tree;
	GCPtr<OS::IStream> stream=gFileSystem.openFile(path);
	bool res=tree.load(stream);
	stream->close();
	if(res)
	{
		return 0;
	}
	xml::XMLElement* elem= tree.getSubElement(mT("Emitter"));
	if(!elem)
	{
		gLogManager.log(mT("Couldn't find emitter tag in file:") + path,ELL_WARNING);
		return 0;
	}
	return  LoadEmitter(elem);
}


void SParticleSystem::LoadSystem(const core::string&path)
{
}


}
}
