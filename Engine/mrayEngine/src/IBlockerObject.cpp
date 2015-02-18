#include "stdafx.h"

#include "IBlockerObject.h"


namespace mray{
namespace physics{

IBlockerObject::IBlockerObject(){
	m_bounce=0.5;
}
IBlockerObject::~IBlockerObject(){
}


void IBlockerObject::setBounce(float b){
	m_bounce=b;
}
float IBlockerObject::getBounce(){
	return m_bounce;
}


void IBlockerObject::affect(IPhysicObject *o,float dt){
	int cnt=o->getVertexCount();
	PhysicalVertex*vert=o->getVerticies();
	for (int i=0;i<cnt;++i)
	{
		test(vert[i],dt);
	}
}

BOPlaneBlocker::BOPlaneBlocker(){

}
BOPlaneBlocker::BOPlaneBlocker(const math::Plane &p){
	m_plane=p;

}
BOPlaneBlocker::~BOPlaneBlocker(){

}

bool BOPlaneBlocker::test(PhysicalVertex &p,float dt){
	float d=p.getNextPos(dt).dotProduct(m_plane.Normal)-m_plane.D;
	if(d<0){
		p.pos-=m_plane.Normal*d;
		p.vel-=m_plane.Normal*(p.vel.dotProduct(m_plane.Normal))*m_bounce*2;
		return true;
	}
	return false;

}

void BOPlaneBlocker::setPlane(const math::Plane &p){
	m_plane=p;

}
math::Plane BOPlaneBlocker::getPlane(){
	return m_plane;

}

//////////////////////////////////////////////////////////////////////////

BOBoxBlocker::BOBoxBlocker(){

}
BOBoxBlocker::BOBoxBlocker(const math::box3d &p){
	m_box=p;
}

bool BOBoxBlocker::test(PhysicalVertex &p,float dt){
	math::vector3d pos=p.getNextPos(dt);
	if(m_box.isPointTotalyInSide(pos)){
		math::vector3d center=m_box.getCenter();

		math::vector3d ext=m_box.getExtent();
		math::vector3d d=pos-center;
		float max=abs(d.x);
		float dp=ext.x-max;
		math::vector3d norm(math::sign(d.x),0,0);
		if(abs(d.y)>max){
			max=abs(d.y);
			dp=ext.y-max;
			norm.set(0,math::sign(d.y),0);
		}
		if(abs(d.z)>max){
			max=abs(d.z);
			dp=ext.z-max;
			norm.set(0,0,math::sign(d.z));
		}
		p.pos+=norm*dp;
		p.vel-=norm*(p.vel.dotProduct(norm))*m_bounce*2;
		return true;
	}
	return false;
}

void BOBoxBlocker::setBox(const math::box3d &p){
	m_box=p;

}
math::box3d BOBoxBlocker::getBox(){
	return m_box;

}


////////////////////////////
BOSphereBlocker::BOSphereBlocker(){
	setSphere(0,1);

}
BOSphereBlocker::BOSphereBlocker(const math::vector3d& c,float r){
	setSphere(c,r);

}
BOSphereBlocker::~BOSphereBlocker(){

}

bool BOSphereBlocker::test(PhysicalVertex &p,float dt){

	math::vector3d dir=(m_center-p.getNextPos(dt));
	float len=dir.LengthSQ();

	if(len<m_sqrRadius){
		len=sqrt(len);
		math::vector3d normal=dir/len;
		float d=m_radius-len;
		p.pos-=normal*d;
		p.vel-=normal*2*(p.vel.dotProduct(normal))*m_bounce;
		return true;
	}
	return false;

}

void BOSphereBlocker::setSphere(const math::vector3d& c,float r){
	m_center=c;
	m_radius=r;
	m_sqrRadius=r*r;
}
float BOSphereBlocker::getRadius(){
	return m_radius;
}
math::vector3d BOSphereBlocker::getCenter(){
	return m_center;
}

}
}