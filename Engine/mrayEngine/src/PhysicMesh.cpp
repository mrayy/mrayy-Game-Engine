#include "stdafx.h"

#include "PhysicMesh.h"


namespace mray{
namespace physics{

PhysicMesh::PhysicMesh(scene::IMeshBuffer*mesh){
	setMesh(mesh);
}
PhysicMesh::~PhysicMesh(){
}

void PhysicMesh::setMesh(scene::IMeshBuffer*mesh){
	m_mesh=mesh;
	if(!m_mesh){
		m_vertices.clear();
	}else{
		GCPtr<video::IHardwareStreamBuffer>vStream= mesh->getStream(0,video::EMST_Position);
		if(!vStream)
			return;
		m_vertices.resize(vStream->length());
		math::vector3d*vert=(math::vector3d*) vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
		for (int i=0;i<m_vertices.size();++i)
		{
			m_vertices[i].pos=vert[i];
			m_vertices[i].oldPos=m_vertices[i].pos;
			m_vertices[i].force=0;
		}

		vStream->unlock();
	}
}
scene::IMeshBuffer*PhysicMesh::getMesh(){
	return m_mesh;
}


void PhysicMesh::updateVertices(float dt){
	if(!m_mesh)
		return;
	update(dt);
	GCPtr<video::IHardwareStreamBuffer>vStream= m_mesh->getStream(0,video::EMST_Position);
	if(!vStream)
		return;
	m_vertices.resize(vStream->length());
	math::vector3d*vert=(math::vector3d*) vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::box3d bb;
	bb.reset(vert[0]);
	for (int i=0;i<m_vertices.size();++i)
	{
		vert[i]=m_vertices[i].pos;
		m_vertices[i].force=0;
		bb.AddPoint(vert[i]);
	}
	m_mesh->setBoundingBox(bb);

	vStream->unlock();
}

}
}