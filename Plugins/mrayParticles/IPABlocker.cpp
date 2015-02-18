#include "stdafx.h"

#include "IPABlocker.h"
#include "ParticleEmitter.h"
#include "IParticle.h"


namespace mray{
namespace scene{


IPABlocker::IPABlocker(){
	m_blockType=BT_Kill;
	m_bounce=0.5;
}
IPABlocker::~IPABlocker(){
}


void IPABlocker::setBlockType(EPABlockType type){
	m_blockType=type;
}
EPABlockType IPABlocker::getBlockType(){
	return m_blockType;
}

void IPABlocker::setBounce(float b){
	m_bounce=b;
}
float IPABlocker::getBounce(){
	return m_bounce;
}

////////////////////////////

PAPlaneBlocker::PAPlaneBlocker(){
	
}
PAPlaneBlocker::PAPlaneBlocker(const math::Plane &p){
	m_plane=p;
}
PAPlaneBlocker::~PAPlaneBlocker(){
}

void PAPlaneBlocker::applyOnParticle(IParticle*p,float dt){
	float d=p->position.dotProduct(m_plane.Normal)-m_plane.D;
	if(d<0){
		switch(getBlockType()){
			case BT_Kill:
				p->lifeSpawn=0;
				break;
			case BT_Bounce:
			{
				p->position-=m_plane.Normal*d;
				p->velocity-=m_plane.Normal*(p->velocity.dotProduct(m_plane.Normal))*m_bounce*2;
			}break;
		}
	}
}
void PAPlaneBlocker::setPlane(const math::Plane &p){
	m_plane=p;
}
math::Plane PAPlaneBlocker::getPlane(){
	return m_plane;
}

core::string PlaneBlockerFactory::getType(){
	return mT("PlaneBlocker");
}
bool PlaneBlockerFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

IParticleAffector*PlaneBlockerFactory::createAffector(){
	return new PAPlaneBlocker();
}


////////////////////////////

	
void PABoxBlocker::applyOnParticle(IParticle*p,float dt){

	if(m_box.isPointInSide(p->position)){
		switch(getBlockType()){
			case BT_Kill:
				p->lifeSpawn=0;
				break;
			case BT_Bounce:
			{
				p->lifeSpawn=0;
				return;
				//TODO:
				math::vector3d dir=p->position-p->lastPosition;


/*				p->position-=m_plane.Normal*d;
				p->velocity-=2*(p->velocity.dotProduct(m_plane.Normal)*m_plane.Normal*m_bounce;*/
			}break;
		}
	}
}

PABoxBlocker::PABoxBlocker(){
	setBox(math::box3d(-1,-1,-1,1,1,1));
}
PABoxBlocker::PABoxBlocker(const math::box3d &box){
	setBox(box);
}
PABoxBlocker::~PABoxBlocker(){
}

void PABoxBlocker::setBox(const math::box3d &box){

	m_box=box;
	m_center=box.getCenter();
}
math::box3d PABoxBlocker::getBox(){
	return m_box;
}


core::string BoxBlockerFactory::getType(){
	return mT("BoxBlocker");
}
bool BoxBlockerFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

IParticleAffector*BoxBlockerFactory::createAffector(){
	return new PABoxBlocker();
}

////////////////////////////
void PASphereBlocker::applyOnParticle(IParticle*p,float dt){

	math::vector3d dir=(m_center-p->position);
	float len=dir.LengthSQ();

	if(len<m_sqrRadius){
		switch(getBlockType()){
			case BT_Kill:
				p->lifeSpawn=0;
				break;
			case BT_Bounce:
			{
				len=sqrt(len);
				math::vector3d normal=dir/len;
				float d=m_radius-len;
				p->position-=normal*d;
				p->velocity-=normal*2*(p->velocity.dotProduct(normal))*m_bounce;
			}break;
		}
	}
}


PASphereBlocker::PASphereBlocker(){
	setSphere(0,1);
}
PASphereBlocker::PASphereBlocker(const math::vector3d &c,float r){
	setSphere(c,r);
}
PASphereBlocker::~PASphereBlocker(){
}

void PASphereBlocker::setSphere(const math::vector3d &c,float r){
	m_center=c;
	m_radius=r;
	m_sqrRadius=r*r;
}
float PASphereBlocker::getRadius(){
	return m_radius;
}
math::vector3d PASphereBlocker::getCenter(){
	return m_center;
}


core::string SphereBlockerFactory::getType(){
	return mT("SphereBlocker");
}

bool SphereBlockerFactory::canCreate(const core::string&type){
	return getType().equals_ignore_case(type);
}

IParticleAffector*SphereBlockerFactory::createAffector(){
	return new PASphereBlocker();
}



}
}