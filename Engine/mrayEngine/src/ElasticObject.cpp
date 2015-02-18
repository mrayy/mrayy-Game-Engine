#include "stdafx.h"

#include "ElasticObject.h"



namespace mray{
namespace physics{

ElasticObject::ElasticObject(IPhysicObject*mesh)
:SpringSystem(mesh){
	setPhysicObject(mesh);
	m_stiffness=20;

}
ElasticObject::~ElasticObject(){
	m_Lengths.clear();
}

void ElasticObject::setPhysicObject(IPhysicObject*mesh){
	SpringSystem::setPhysicObject(mesh);
	if(!m_mesh){
		m_Lengths.clear();
	}else{
		m_center=0;
		PhysicalVertex*vert=m_mesh->getVerticies();
		int cnt=mesh->getVertexCount();
		m_Lengths.resize(cnt);
		if(!cnt)
			return;
		for (int i=0;i<cnt;++i)
		{
			m_center+=vert[i].pos;
		}
		m_center/=cnt;
		for (int i=0;i<cnt;++i)
		{
			m_Lengths[i].vec=vert[i].pos-m_center;
			m_Lengths[i].lengthSQ=m_Lengths[i].vec.LengthSQ();
			m_Lengths[i].length=sqrt(m_Lengths[i].lengthSQ);
		//	m_Lengths[i].vec/=m_Lengths[i].length;
		}
	}
}
void ElasticObject::affect(IPhysicObject*o,float dt){
	if(!m_mesh || o!=m_mesh)
		return;
	PhysicalVertex*vert=m_mesh->getVerticies();
	int cnt=m_Lengths.size();
	math::vector3d center;
	for (int i=0;i<cnt;++i)
	{
		center+=vert[i].pos;
	}
	center/=cnt;
	for (int i=0;i<m_Lengths.size();++i)
	{
		math::vector3d v=vert[i].pos-center;
		float d=v.LengthSQ();
		if(d!=m_Lengths[i].lengthSQ){
//   			d=sqrt(d);
//   			v/=d;
			math::vector3d force;
			force=(m_Lengths[i].vec-v)*m_stiffness;
		//	force=v*(m_Lengths[i].length-d)*m_stiffness;
			vert[i].force+=force;
		}
	}
//	SpringSystem::update(dt);
}

}
}
