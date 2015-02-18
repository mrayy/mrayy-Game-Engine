#include "stdafx.h"

#include "IPhysicObject.h"

namespace mray{
namespace physics{

PhysicalVertex::PhysicalVertex(){
	pos=oldPos=0;
	force=0;
	vel=0;
	weight=0;
}
math::vector3d PhysicalVertex::getNextPos(float dt){
	if(weight<=0)
		return pos;
	return pos+(vel*0.998)*dt;

}
void PhysicalVertex::update(float dt){
	if(weight>0){
		vel+=force*iweight*dt;
		math::vector3d p=pos;
		pos+=vel*dt;
		vel*=0.998;
		oldPos=p;
		force=0;
	}
}
void PhysicalVertex::setWeight(float w){
	weight=w;
	if(weight)
		iweight=1.0f/weight;
	else
		iweight=0;
}
float PhysicalVertex::getWeight(){
	return weight;
}

//////////////////////////////////////////////////////////////////////////
IPhysicObject::IPhysicObject(){

}
IPhysicObject::~IPhysicObject(){
	m_vertices.clear();
	clearAffectors();
}


void IPhysicObject::update(float dt){
	int pcnt=getVertexCount();
	PhysicalVertex*pvert=getVerticies();

	AffectorList::iterator it=m_affectors.begin();
	for (;it!=m_affectors.end();++it)
	{
		(*it)->affect(this,dt);
	}

	for (int v=0;v<pcnt;v++)
	{
		pvert[v].update(dt);
	}

}
PhysicalVertex*IPhysicObject::getVertex(int i){
	return &m_vertices[i];
}
PhysicalVertex*IPhysicObject::getVerticies(){
	return &m_vertices[0];
}
int IPhysicObject::getVertexCount(){
	return m_vertices.size();
}

void IPhysicObject::translate(math::vector3d p){
	int pcnt=getVertexCount();
	PhysicalVertex*pvert=getVerticies();
	for (int v=0;v<pcnt;v++)
	{
		pvert[v].pos+=p;
	}
}

void IPhysicObject::addAffector(GCPtr<IPhysicObjectAffector>a){
	m_affectors.push_back(a);
}
void IPhysicObject::clearAffectors(){
	m_affectors.clear();
}

}
}
