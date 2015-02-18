#include "stdafx.h"
#include "PhysicObject.h"


namespace mray{
namespace physics{

PhysicObject::PhysicObject(std::vector<math::vector3d> *points){
	setPoints(points);
}
PhysicObject::~PhysicObject(){
}

void PhysicObject::setPoints(std::vector<math::vector3d> *points){
	m_points=points;
	if(!m_points){
		m_vertices.clear();
	}else{
		m_vertices.resize(points->size());
		math::vector3d*vert=&(*m_points)[0];
		for (int i=0;i<m_points->size();++i)
		{
			m_vertices[i].pos=vert[i];
			m_vertices[i].oldPos=m_vertices[i].pos;
			m_vertices[i].force=0;
		}
	}
}
math::vector3d*PhysicObject::getPoints(){
	return &(*m_points)[0];
}


void PhysicObject::updateVertices(float dt){
	if(!m_points)
		return;
	update(dt);
	math::vector3d*vert= &(*m_points)[0];
	for (int i=0;i<m_points->size();++i)
	{
		vert[i]=m_vertices[i].pos;
		m_vertices[i].force=0;
	}
}

}
}