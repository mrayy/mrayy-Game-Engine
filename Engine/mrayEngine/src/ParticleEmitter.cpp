#include "stdafx.h"

#include "ParticleEmitter.h"
#include "IParticle.h"

#include "SParticleSystem.h"
#include "IVideoDevice.h"
#include "IParticleAffector.h"
#include "MaterialResourceManager.h"

#include "IRenderPass.h"
#include "IViewNode.h"

#include "ISceneManager.h"
#include "IJobPiece.h"
#include "ThreadJobManager.h"
#include "IOSystem.h"
#include "Engine.h"
#include "IDebugDrawManager.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "IMeshBuffer.h"
#include "ICustomParticleEmitter.h"
#include "SParticleSystemFactory.h"

namespace mray
{
namespace scene
{



	class ParticleEmitterJob:public IJobPiece
	{
	public:

		ParticleEmitter::ParticleList::iterator firstParticle;
		ParticleEmitter::ParticleList::iterator lastParticle;

		ParticleEmitter* owner;
		math::box3d bbox;
		float dt;

		bool isUsed;

		ParticleEmitterJob(ParticleEmitter*o):owner(o),dt(0),isUsed(0)
		{
		}

		virtual bool Execute()
		{
			if(!isUsed)
				return true;
			ParticleEmitter::ParticleList::iterator it=firstParticle;
			IParticle* p=0;

			math::vector3d maxScale;

			if(it!=owner->getParticles().end())
				bbox.reset((*it)->position);

			for (;it!=lastParticle;++it)
			{
				p=*it;
				owner->_UpdateParticleMT(p,dt);
				bbox.AddPoint(p->position);

				if(p->scale.x>maxScale.x)maxScale.x=p->scale.x;
				if(p->scale.y>maxScale.y)maxScale.y=p->scale.y;
				if(p->scale.z>maxScale.z)maxScale.z=p->scale.z;
			}

			bbox.MinP-=maxScale*0.5;
			bbox.MaxP+=maxScale*0.5;

			return true;
			
		}
	};

ParticleEmitter::ParticleEmitter():m_stopEmitting(0),m_system(0)
{
	m_currRate=0;
	m_renderer=0;

	m_totalJobsRequested=0;

	m_time=0;
	m_cycleLength=0;
	m_generateMode=EPGM_Infinite;

	m_particlesGen=0;

	m_particleShapeType=EPS_Point;

	m_material=new video::RenderMaterial();

	m_emitter=0;

	m_visible=true;
}


ParticleEmitter::~ParticleEmitter()
{
	ClearParticles();
	ClearEffecters();
	particlesRate.clear();
	particlesCount.clear();
	particlesLifeSpawn.clear();
	delete m_renderer;
	if(m_emitter)
		delete m_emitter;
	{
		std::list<IJobPiece*>::iterator it=m_jobPieces.begin();
		for (;it!=m_jobPieces.end();++it)
		{
			delete *it;
		}
		m_jobPieces.clear();
	}
}


void ParticleEmitter::setupParticle(IParticle* particle)
{
	if(m_emitter)
		m_emitter->setupParticle(particle);
}

void ParticleEmitter::ClearParticles()
{
	ParticleList::iterator it= m_particles.begin();
	for (;it!=m_particles.end();++it)
	{
		delete *it;
	}
	it= m_deadParticles.begin();
	for (;it!=m_deadParticles.end();++it)
	{
		delete *it;
	}
	m_particles.clear();
	m_deadParticles.clear();
};
void ParticleEmitter::ClearEffecters()
{

	ParticleAffectorList::iterator it= m_particleAffectors.begin();
	for (;it!=m_particleAffectors.end();++it)
	{
		delete *it;
	}
	m_particleAffectors.clear();
}
void ParticleEmitter::SetRenderer(IParticleRenderer*r)
{
	m_renderer=r;
}
IParticleRenderer* ParticleEmitter::GetRenderer()
{
	return m_renderer;
}


void ParticleEmitter::setName(const core::string&name)
{
	m_name=name;
}
const core::string& ParticleEmitter::getName()
{
	return m_name;
}

bool ParticleEmitter::IsEmpty()const
{
	return m_particles.empty();
}

void ParticleEmitter::setGenerateMode(EParticleGenerateMode mode){
	m_generateMode=mode;
}
EParticleGenerateMode ParticleEmitter::getGenerateMode(){
	return m_generateMode;
}

void ParticleEmitter::setCycleLength(float length){
	m_cycleLength=length;
}
float ParticleEmitter::getCycleLength(){
	return m_cycleLength;
}

IParticle* ParticleEmitter::createParticle(){
	return new IParticle(this);
}

void ParticleEmitter::addAffector(IParticleAffector *affector){
	m_particleAffectors.push_back(affector);
}
void ParticleEmitter::removeAffector(IParticleAffector* affector){
}


void ParticleEmitter::setParticleShapeType(EParticleShape type){
	m_particleShapeType=type;
}
EParticleShape ParticleEmitter::getParticleShapeType(){
	return m_particleShapeType;
}

void ParticleEmitter::addDeadParticle(IParticle*part)
{
	m_deadParticles.push_back(part);
};


int ParticleEmitter::getTotalGeneratedParticles()
{
	return m_particlesGen;
}
int ParticleEmitter::getCurrentParticlesCount()
{
	return m_particles.size();
}

void ParticleEmitter::stopEmitting()
{
	m_stopEmitting=true;
}
void ParticleEmitter::resumeEmitting()
{
	m_stopEmitting=false;
}
bool ParticleEmitter::isStopEmitting()
{
	return m_stopEmitting;
}


void ParticleEmitter::reset(){
	m_particlesGen=0;
}
void ParticleEmitter::setParticleAcceleration(const math::vector3d& acc)
{
	m_acceleration=acc;
}
void ParticleEmitter::setParticleVelocity(const math::vector3d&  vel)
{
	m_velocity=vel;
}

void ParticleEmitter::setTexture(video::ITexturePtr  tex)
{
	m_material->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);
} 

void ParticleEmitter::reSpawn(IParticle*part)
{
	part->mainLifeSpawn=part->lifeSpawn=m_currLifeSpawn;
	if(part->lifeSpawn==0)return;


	float rnd=math::Randomizer::rand01();
	part->color.Set(255,255,255,255);
	part->scale=1;

	part->velocity=m_velocity;
	part->acceleration=m_acceleration;

	part->bAlive=1;

	part->position=0;
	part->randVal.x=math::Randomizer::rand01();
	part->randVal.y=math::Randomizer::rand01();
	part->randVal.z=math::Randomizer::rand01();
	part->randVal.w=math::Randomizer::rand01();
	setupParticle(part);

	part->position=m_system->getOwner()->getAbsoluteTransformation()*(part->position);
}

void ParticleEmitter::preRender(IRenderPass*pass)
{
}
void ParticleEmitter::render(IRenderPass*pass)
{
	if(!m_renderer)
		return;
	video::RenderTechnique*t= m_material->GetActiveTechnique();

	if(!t)
		return;
	video::IVideoDevice* device= Engine::getInstance().getDevice();
	IViewNode* vnode=pass->getViewNode();
	const math::Frustum* frust=vnode->getViewFrustrum();
	math::vector3d vPos=vnode->getViewPos();
	m_renderer->reset(vnode);
	ParticleList::iterator it=m_particles.begin();
	for(;it!=m_particles.end();++it)
	{
		IParticle*p=*it;
		float dist=(vPos-p->position).LengthSQ();
		float len=p->scale.x*p->scale.y;
		if(len<0.00001*dist)continue;
		if(frust->pointInFrustrum(p->position))
			m_renderer->addParticle(*it);
	}
	m_renderer->done();

	device->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	device->setStreamMap(m_renderer->getMeshBuffer()->getStreamsData());
	const video::RenderPassList& passes= t->GetPasses();
	for(int i=0;i<passes.size();++i)
	{
		device->useRenderPass(passes[i]);
		device->drawElements(m_renderer->getMeshBuffer()->getIndexData(),m_renderer->getMeshBuffer()->getRenderType());
	}
	device->endStreams();
	if(m_system->getOwner() && m_system->getOwner()->getDrawBoundingBox())
	{
		if(m_system->getOwner()->getSceneManager()->GetDebugDrawManager())
		{
			m_system->getOwner()->getSceneManager()->GetDebugDrawManager()->AddAABB(m_boundingBox,1);
		}
		
//		device->useMaterial(0);
//		device->drawBox(m_boundingBox,math::matrix4x4::Identity,video::MR_LINES);
	}
}

void ParticleEmitter::AddUpdateJobs(ThreadJobManager* mngr,float dt)
{
	std::list<IJobPiece*>::iterator it= m_jobPieces.begin();
	for (;it != m_jobPieces.end();++it)
	{
		ParticleEmitterJob*p=(ParticleEmitterJob*)*it;
		if(p->isUsed)
			mngr->AddPiece(p);
	}
}
void ParticleEmitter::_ApplyAffectors(float dt)
{

	int chunksCount = OS::IOSystem::getInstance().getProcessorsCount();
	int particlesCount = 1 + m_particles.size() / chunksCount;
	int currP = 0;
	ParticleList::iterator it = m_particles.begin();
	ParticleList::iterator firstIt = it;
	std::list<IJobPiece*>::iterator lastjIt, jIt = m_jobPieces.begin();
	lastjIt = jIt;
	m_totalJobsRequested = 0;
	if (it != m_particles.end())
	{
		std::advance(it, particlesCount - currP - 1);
		currP = particlesCount - 1;
	}
	for (; it != m_particles.end(); ++it, ++currP)
	{
		if (currP > particlesCount)
		{
			if (jIt == m_jobPieces.end())
			{
				m_jobPieces.push_back(new ParticleEmitterJob(this));
				jIt = m_jobPieces.end();
				--jIt;
			}
			ParticleEmitterJob*j = (ParticleEmitterJob*)(*jIt);
			j->isUsed = true;
			j->firstParticle = firstIt;
			j->lastParticle = it;
			j->dt = dt;
			j->listener = this;
			firstIt = it;
			lastjIt = jIt;
			++jIt;
			++m_totalJobsRequested;
			currP = 0;
		}
	}

	if (currP > 0)
	{
		if (jIt == m_jobPieces.end())
		{
			m_jobPieces.push_back(new ParticleEmitterJob(this));
			jIt = m_jobPieces.end();
			--jIt;
		}
		ParticleEmitterJob*j = (ParticleEmitterJob*)(*jIt);
		j->isUsed = true;
		j->firstParticle = firstIt;
		j->lastParticle = it;
		j->dt = dt;
		j->listener = this;
		++m_totalJobsRequested;
	}
}
void ParticleEmitter::_SpawnParticles(float dt)
{

	if (!m_stopEmitting){
		int currPCount;
		float currPRate;
		if (particlesCount.getKeysCount() > 0)
			currPCount = particlesCount.getInterpolatedKey(m_time);
		else
			currPCount = -1;
		if (particlesRate.getKeysCount() > 0)
			currPRate = particlesRate.getInterpolatedKey(m_time);
		else
			currPRate = 10;
		if ((m_particlesGen<currPCount || currPCount<0) && currPRate>0){


			m_currRate += currPRate*dt;
			IParticle*part;

			if (m_currRate>0){
				if (particlesLifeSpawn.getKeysCount() > 0)
					m_currLifeSpawn = particlesLifeSpawn.getInterpolatedKey(m_time);
				else
					m_currLifeSpawn = 1;
			}

			//check m_deadParticles(graveyard)
			int i = 0;
			int n = 0;
			ParticleList::iterator tmp, it = m_deadParticles.begin();
			for (i = 0; it != m_deadParticles.end() && i < (int)m_currRate; ++i)
			{
				reSpawn((*it));
				m_particles.push_back(*it);
				tmp = it;
				++it;
				m_deadParticles.erase(tmp);
				//dont push it back to particles list cuz they are already there
				m_particlesGen++;
				n++;
			}
			m_currRate -= n;
			n = 0;
			for (i = 0; i < (int)m_currRate; ++i)
			{
				part = createParticle();
				if (!part)
					break;
				reSpawn(part);
				m_particles.push_back(part);
				m_particlesGen++;
				n++;
			}
			m_currRate -= n;
		}
	}
}

void ParticleEmitter::update(float dt)
{
// 	if(!m_visible)
// 		return;
// 		
	_SpawnParticles(dt);
	_ApplyAffectors(dt);
	/*
	{
		ParticleList::iterator it=m_particleAffectors.begin();
		for(;it!=m_particleAffectors.end();++it)
		{
			(*it)->applyOnParticles(this,dt);
		}
	}

	{
		if(m_particles.getSize())
			m_boundingBox.reset((*m_particles.begin())->position);
		math::vector3d maxScale;
		ParticleList::iterator tmp,it=m_particles.begin();
		for(;it!=m_particles.end();)
		{
			IParticle* p=*it;
			p->update(dt);
			m_boundingBox.AddPoint(p->position);
			if(!p->bAlive){				
				tmp=it;
				++it;
				m_particles.remove(tmp);
			}else
				++it;

			if(p->scale.x>maxScale.x)maxScale.x=p->scale.x;
			if(p->scale.y>maxScale.y)maxScale.y=p->scale.y;
			if(p->scale.z>maxScale.z)maxScale.z=p->scale.z;
		}
	}
	*/


	m_time+=dt;

	if(m_generateMode==EPGM_Cycle && m_time>m_cycleLength){
		m_particlesGen=0;
		m_time=0;
	}
}

void ParticleEmitter::_UpdateParticleMT(IParticle* particle,float dt)
{

	ParticleAffectorList::iterator it=m_particleAffectors.begin();
	for(;it!=m_particleAffectors.end();++it)
	{
		if(!m_visible && !(*it)->affectsTransformations())
			continue;
		(*it)->applyOnParticle(particle,dt);
	}

	particle->update(dt);

}
void ParticleEmitter::SignalJobDone(IJobPiece*j)
{
	--m_totalJobsRequested;
	if(m_totalJobsRequested==0)
	{
		bool first=true;
		std::list<IJobPiece*>::iterator it= m_jobPieces.begin();
		for (;it != m_jobPieces.end();++it)
		{
			ParticleEmitterJob*p=(ParticleEmitterJob*)*it;
			if(p->isUsed)
			{
				if(first)
					m_boundingBox=p->bbox;
				else m_boundingBox.AddBox(p->bbox);

				p->isUsed=false;
				first=false;
			}
		}
		{
			ParticleList::iterator tmp,it=m_particles.begin();
			for(;it!=m_particles.end();)
			{
				IParticle* p=*it;
				if(!p->bAlive){				
					tmp=it;
					++it;
					m_particles.erase(tmp);
				}else
					++it;

			}
		}
	}
}

ParticleEmitter::ParticleList& ParticleEmitter::getParticles(){
	return m_particles;
}


void ParticleEmitter::setMaterial(const video::RenderMaterialPtr&mtrl)
{
			m_material=mtrl;
}

const video::RenderMaterialPtr& ParticleEmitter::getMaterial(){
	return m_material;
}


bool ParticleEmitter::LoadFromXML(xml::XMLElement*elem)
{
	xml::XMLElement* e;
	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Name"));
	if(attr)
		setName(attr->value);

	attr=elem->getAttribute(mT("Type"));
	if(attr)
		m_emitter= SParticleSystemFactory::getInstance().createParticleEmitter(attr->value);

	attr=elem->getAttribute(mT("MaterialName"));
	if(attr)
	{
		m_material=gMaterialResourceManager.getMaterial(attr->value);
	}else
	{
		m_material->loadFromXML(elem);
	}

	e=elem->getSubElement(mT("Rate"));
	if(e)
	{
		e=e->getSubElement(mT("Value"));
		for(;e;e=e->nextSiblingElement(mT("Value")))
		{
			float t=0,v=0;
			attr=e->getAttribute(mT("Time"));
			if(!attr)
				continue;
			t=core::StringConverter::toFloat(attr->value);
			attr=e->getAttribute(mT("V"));
			if(!attr)
				continue;
			v=core::StringConverter::toFloat(attr->value);
			particlesRate.addKey(t,v);
		}
	}

	e=elem->getSubElement(mT("Count"));
	if(e)
	{
		e=e->getSubElement(mT("Value"));
		for(;e;e=e->nextSiblingElement(mT("Value")))
		{
			float t=0,v=0;
			attr=e->getAttribute(mT("Time"));
			if(!attr)
				continue;
			t=core::StringConverter::toFloat(attr->value);
			attr=e->getAttribute(mT("V"));
			if(!attr)
				continue;
			v=core::StringConverter::toFloat(attr->value);
			particlesCount.addKey(t,v);
		}
	}
	e=elem->getSubElement(mT("LifeSpawn"));
	if(e)
	{
		e=e->getSubElement(mT("Value"));
		for(;e;e=e->nextSiblingElement(mT("Value")))
		{
			float t=0,v=0;
			attr=e->getAttribute(mT("Time"));
			if(!attr)
				continue;
			t=core::StringConverter::toFloat(attr->value);
			attr=e->getAttribute(mT("V"));
			if(!attr)
				continue;
			v=core::StringConverter::toFloat(attr->value);
			particlesLifeSpawn.addKey(t,v);
		}
	}
	return true;
}


}
}


