#include "stdafx.h"

#include "SpringSystem.h"


namespace mray{
namespace physics{


SpringSystem::SpringSystem(IPhysicObject*mesh){
	setPhysicObject(mesh);
	m_iterations=2;
	m_gravity.set(0,-9.8,0);
}
SpringSystem::~SpringSystem(){
	clear();
}

void SpringSystem::setSpringsCount(int cnt){
	m_springs.resize(cnt);
}
int SpringSystem::getSpringsCount(){
	return m_springs.size();
}

void SpringSystem::addSpring(int v1,int v2,float coeff,float minLength,float maxLength){
	m_springs.push_back(SpringLink(v1,v2,coeff,minLength,maxLength));
}
void SpringSystem::addSpring(SpringLink&spring){
	m_springs.push_back(spring);
}

void SpringSystem::removeSpring(int i){
	std::vector<SpringLink>::iterator it= m_springs.begin();
	std::advance(it,i);
	m_springs.erase(it);
}
void SpringSystem::clear(){
	m_springs.clear();
}



void SpringSystem::setForce(const math::vector3d &f){
	m_force=f;
}
void SpringSystem::setGravity(const math::vector3d &f){
	m_gravity=f;
}

math::vector3d SpringSystem::getForce(){
	return m_force;
}
math::vector3d SpringSystem::getGravity(){
	return m_gravity;
}

SpringLink*SpringSystem::getSpring(int i){
	if(i<0 || i>=m_springs.size())
		return 0;
	return &m_springs[i];
}
SpringLink*SpringSystem::getSprings(){
	return &m_springs[0];
}


void SpringSystem::affect(IPhysicObject*o,float dt){
	if(!m_mesh || o!=m_mesh)
		return;

	const float i50=1.0f/50.0f;
	if(dt>i50)
		dt=i50;

	int cnt=m_mesh->getVertexCount();
	PhysicalVertex*vertices=m_mesh->getVerticies();

	int scnt=m_springs.size();
	m_iterations=1;
	float maxError=10;
	for (int i=0;i<m_iterations;++i)
	{
		float ds=0;
		for (int s=0;s<scnt;s++)
		{
			SpringLink&spring=m_springs[s];

			int a=spring.vertConnection[0];
			int b=spring.vertConnection[1];
			
			if(vertices[a].getWeight()<=0 && vertices[b].getWeight()<=0){
				continue;
			}
			math::vector3d v=vertices[b].getNextPos(dt)-vertices[a].getNextPos(dt);
			float d=v.LengthSQ();
			if(d<spring.maxLength*spring.maxLength  || d==0)
				continue;
			d=sqrt(d);
			ds+=d;
			float dlen;
				dlen=spring.maxLength-d;
			math::vector3d dir=v/d;
			math::vector3d vel=vertices[b].vel-vertices[a].vel;

			math::vector3d factor[2];
			factor[0]=factor[1]=dir*(dlen*spring.springCoefficient*0.5f);

			if(vertices[a].getWeight()<=0){
				factor[0]=0;
			//	factor[1]*=2;
			}
			if(vertices[b].getWeight()<=0){
			//	factor[0]*=2;
				factor[1]=0;
			}
			vertices[a].force-=factor[0];
			vertices[b].force+=factor[1];
			/*
			math::vector3d force;
			force=dir*(spring.length-d)*spring.springCoefficient;
			vertices[a].force-=force;
			vertices[b].force+=force;*/
		}
		if(ds<maxError)
			break;
	}
}


}
}