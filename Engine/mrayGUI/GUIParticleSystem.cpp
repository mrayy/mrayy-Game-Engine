

#include "GUIParticleSystem.h"

#include "GUIManager.h"
#include "Randomizer.h"


namespace mray{
namespace GUI{


GUIParticleSystem::GUIParticleSystem(GUIManager*manager):
GUIElement(manager)
{
#ifdef ___DEBUG___
	setDebugName("GUIParticleSystem");
#endif
	

	particlesGen=0;
	currRate=0;

	minEmitSize=1;
	maxEmitSize=1;
	
	minDieSize=1;
	maxDieSize=1;

	minEmitColor=255;
	maxEmitColor=255;

	minDieColor=255;
	maxDieColor=255;

	minRotSpeed=0;
	maxRotSpeed=100;

	m_particleShape=manager->addRotateImage(0,1);

	m_particleShape->removeFromParent();
}


GUIParticleSystem::~GUIParticleSystem()
{
	particles.clear();
	deadParticles.clear();
}

void GUIParticleSystem::setParticleAcceleration(math::vector2d acc){
	acceleration=acc;
}
void GUIParticleSystem::setParticleVelocity(math::vector2d vel){
	velocity=vel;
}


void GUIParticleSystem::setEmitColor(video::SColor minClr,video::SColor maxClr){
	minEmitColor=minClr;
	maxEmitColor=maxClr;
}
void GUIParticleSystem::setDieColor(video::SColor minClr,video::SColor maxClr){
	minDieColor=minClr;
	maxDieColor=maxClr;
}
void GUIParticleSystem::setEmitSize(math::vector2d minSize,math::vector2d maxSize){
	minEmitSize=minSize;
	maxEmitSize=maxSize;
}
void GUIParticleSystem::setDieSize(math::vector2d minSize,math::vector2d maxSize){
	minDieSize=minSize;
	maxDieSize=maxSize;
}
void GUIParticleSystem::setMinMaxRotSpeed(float min,float max){
	minRotSpeed=min;
	maxRotSpeed=max;
}


void GUIParticleSystem::setParticlesRate(float rate){particlesRate=rate;}
void GUIParticleSystem::setParticlesCount(int count){particlesCount=count;}
void GUIParticleSystem::setParticlesLifeSpawn(float minLife,float maxLife){
	minLifeSpawn=minLife;
	maxLifeSpawn=maxLife;
}

void GUIParticleSystem::addDeadParticle(GCPtr<GUIParticle>part){
	deadParticles.push_back(part);
};


void GUIParticleSystem::reset(){
	particlesGen=0;
}

//void GUIParticleSystem::setShape(GCPtr<GUIElment>shape);
GCPtr<GUIRotImage> GUIParticleSystem::getShapeNode(){
	return m_particleShape;
}
void GUIParticleSystem::reSpawn(GCPtr<GUIParticle>part)
{
	if(!part)return;
	part->lifeSpawn=minLifeSpawn+(maxLifeSpawn-minLifeSpawn)*RANDOM_FLOAT;
	if(part->lifeSpawn==0)return;

	float rnd=math::Randomizer::rand01();
	part->color=minEmitColor+(maxEmitColor-minEmitColor)*rnd;
	part->R=part->color.R;
	part->G=part->color.G;
	part->B=part->color.B;
	part->A=part->color.A;
	rnd=math::Randomizer::rand01();
	video::SColor dieClr=minDieColor+(maxDieColor-minDieColor)*rnd;
	part->colorDT.x=(dieClr.R-part->color.R)/part->lifeSpawn;
	part->colorDT.y=(dieClr.G-part->color.G)/part->lifeSpawn;
	part->colorDT.z=(dieClr.B-part->color.B)/part->lifeSpawn;
	part->AlphaDT=(dieClr.A-part->color.A)/part->lifeSpawn;

	part->rotSpeed=math::lerp(minRotSpeed,maxRotSpeed,math::Randomizer::rand01());

	rnd=math::Randomizer::rand01();
	part->MainSize=minEmitSize+(maxEmitSize-minEmitSize)*rnd;
	rnd=math::Randomizer::rand01();
	part->sizeDT=(minDieSize+(maxDieSize-minDieSize)*rnd-
		part->getSize())/part->lifeSpawn;

	part->velocity=velocity;
	part->acceleration=acceleration;

	part->MainPos=0;

	part->bAlive=1;
	setupParticlePosition(part);

	part->MainPos=part->getPos()+getAbsPos();
}

void GUIParticleSystem::draw(float dt)
{
	if((particlesGen<particlesCount||particlesCount==-1) && particlesRate>0 && Visible){
		currRate+=particlesRate*dt;
		GCPtr<GUIParticle>part;
		int i=0;
		//check deadParticles(graveyard)
		std::list<GCPtr<GUIParticle>>::iterator tmp,it=deadParticles.begin();
		for(;it!=deadParticles.end() && i<(int)currRate;++i)
		{
			reSpawn((*it));
			tmp=it;
			++it;
			deadParticles.remove(tmp);
			//dont push it back to particles list cuz they are already there
			particlesGen++;
			currRate--;
		}
		for(;i<(int)currRate;++i)
		{
			part=new  GUIParticle(this);
			reSpawn(part);
			particles.push_back(part);
			particlesGen++;
			currRate--;
		}
	}

	{
		std::list<GCPtr<GUIParticle>>::iterator it=particles.begin();
		for(;it!=particles.end();++it)
		{
			(*it)->draw(dt);
		}
	}

	GUIElement::draw(dt);
}




}
}

