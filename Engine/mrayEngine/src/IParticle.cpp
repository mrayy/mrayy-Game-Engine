#include "stdafx.h"


#include "IParticle.h"
#include "ParticleEmitter.h"

namespace mray{
namespace scene{

IParticle::IParticle(ParticleEmitter*e)
:m_emitter(e),bAlive(0),lifeSpawn(0),AngleZ(0)
{
//	createRigidBody(1,0.1,EIK_SPHERE,ECT_NONE);
	position=0;
	velocity=0;
	acceleration=0;
	force=0;
	randVal=0;
}


IParticle::~IParticle()
{
}

ParticleEmitter* IParticle::GetEmitter(){
	return m_emitter;
}


void IParticle::setupParticleToPreRender(EParticleShape shape){
	m_lastShape=shape;
}

void IParticle::render()
{/*

	video::IVideoDevice*dev=pSystem->getDevice();
	if(!dev){
		return;
	}
	switch(m_lastShape){
		case EPS_Point:
		{
			dev->setTransformationState(video::TS_WORLD,math::matrix4x4());
			dev->useMaterial(pSystem->getMaterial());
			dev->drawVertex(position,color,scale.x);
		}break;
		case EPS_Line:
		{
			dev->setTransformationState(video::TS_WORLD,math::matrix4x4());
			dev->useMaterial(pSystem->getMaterial());
			dev->setLineWidth(scale.x);
			
			dev->draw3DLine(lastPosition,position,color);
		}break;
		case EPS_Billboard:
		{
			SBillboardNode*node=pSystem->getBillboardNode();
			
			node->setSize(math::vector2d(scale.x,scale.y));
			node->position=position ;
			node->AngleZ=(AngleZ);
			node->scale=scale;
			node->getMaterial(0)->Alpha=color.A;
			node->getMaterial(0)->diffuseColor=color;
			node->getMaterial(0)->ambientColor=color;
			node->getMaterial(0)->specularColor=color;
			node->updateAbsoluteTransformation();
			node->render(pass);;
			transformationMatrix=node->getAbsoluteTransformation();
		}break;
		case EPS_SceneNode:
		{
			ISceneNode*node=pSystem->getShapeNode();
			node->position=position ;
			//node->setOrintation(orintation);
			node->scale=scale;
			node->getMaterial()->Alpha=color.A;
			node->getMaterial()->diffuseColor=color;
			node->getMaterial()->ambientColor=color;
			node->getMaterial()->specularColor=color;
			node->updateAbsoluteTransformation();
			node->render(pass);
			transformationMatrix=node->getAbsoluteTransformation();
		}break;
	}*/
}

void IParticle::update(float dt)
{
	if(!bAlive)return;
	if (lifeSpawn != -1)
	{
		lifeSpawn -= dt;
		if (lifeSpawn <= 0)
		{
			m_emitter->addDeadParticle(this);
			lifeSpawn = 0;
			bAlive = 0;
			return;
		}
	}/*
	rigidBody->reset();
	std::list<SForces*>::iterator it=pSystem->getForces()->begin();
	for(;it!=pSystem->getForces()->end();++it)
	{
		(*it)->force(dt,rigidBody);
	}
	rigidBody->simulate(dt);
	position+=rigidBody->getPosition();
	rotate=rigidBody->getOrientation()*rotate;
*/
/*	if(pSystem->getRigidBody())
		position+=pSystem->getRigidBody()->getPosition();*/
	/*
	R+=colorDT.x*dt;
	G+=colorDT.y*dt;
	B+=colorDT.z*dt;
	A+=AlphaDT*dt;
	color=video::SColor(R,G,B,A);*/

	lastPosition=position;
	position+=velocity*dt;
	velocity+=(acceleration+force)*dt;


//	scale+=sizeDT*dt;
/*
	if((m_lastShape==EPS_Billboard || m_lastShape==EPS_SceneNode)){
		updateAbsoluteTransformation();
	}

	updateBoundingBox();*/


}

math::box3d IParticle::calculateBB()const
{
	return math::box3d(position-scale*0.5,position+scale*0.5);
}

/*
void IParticle::updateBoundingBox(){
	switch(m_lastShape){
		case EPS_Point:
			boundingBox.reset(position-scale.x);
			boundingBox.AddPoint(position+scale.x);
			break;
		case EPS_Line:
		{
			boundingBox.reset(lastPosition-scale.x);
			boundingBox.AddPoint(position+scale.x);
		}break;
		case EPS_Billboard:
		{
			SBillboardNode*node= pSystem->getBillboardNode();
			if(!node)
				boundingBox.reset(position);
			else{
				boundingBox=node->getBoundingBox();
		//		transformationMatrix.transformBox(boundingBox);
				boundingBox.MinP+=this->getAbsolutePosition();
				boundingBox.MaxP+=this->getAbsolutePosition();
			}
		}break;
		case EPS_SceneNode:
		{
			ISceneNode*node= pSystem->getShapeNode();
			if(!node)
				boundingBox.reset(position);
			else{
				boundingBox=node->getBoundingBox();
		//		transformationMatrix.transformBox(boundingBox);
				boundingBox.MinP+=this->getAbsolutePosition();
				boundingBox.MaxP+=this->getAbsolutePosition();
			}
		}break;
	}
}

math::box3d IParticle::getBoundingBox(){
	return boundingBox;
}
math::box3d IParticle::getTransformedBoundingBox(){
	return boundingBox;
}
*/

}
}
