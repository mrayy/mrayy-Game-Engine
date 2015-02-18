
#include "PlaneCamera.h"


namespace mray{
namespace gameMod{

PlaneCamera::PlaneCamera(scene::SceneManager*smngr)
:scene::SCameraNode(smngr)
{
	m_radius=10;
	m_angles.set(-45,0,0);
}
PlaneCamera::~PlaneCamera(){
}


void PlaneCamera::attachToPlane(IMovable*node){

	math::vector3d p;
	p.x=0;
	p.y=m_radius*math::sind(45);
	p.z=m_radius*math::cosd(45);

	m_position=p+node->getPosition();

	useTarget=true;

	setLookAtTarget(node);
}

void PlaneCamera::update(float dt){

	if(m_lookAtTarget){
		/*
		math::vector3d t=m_lookAtTarget->getAbsolutePosition()-getAbsolutePosition();
		float l=t.Length();
		if(l==0){
			l=1;
			t.z=-1;
		}
		t/=l;
		m_position+=t*(l-m_radius)*dt;*/

		math::vector3d p(0,m_radius*math::cosd(45),-m_radius);
		math::vector3d up(math::vector3d::YAxis);
// 		p.x=m_radius*math::cosd(180)*math::cosd(45);
// 		p.y=m_radius*math::sind(45);
// 		p.z=m_radius*math::sind(180)*math::cosd(45);
		m_lookAtTarget->getRelativeTransformation().transformNormalPre(p);
		m_lookAtTarget->getRelativeTransformation().transformNormalPre(up);
		m_vUp+=(up-m_vUp)*dt;
		m_position+=(p- (getAbsolutePosition()-m_lookAtTarget->getAbsolutePosition()))*dt;

		math::vector3d t=m_position-m_lookAtTarget->getAbsolutePosition();
		float l=t.Length();
		if(l==0){
			l=1;
			t.z=-1;
		}
		t/=l;
		float dp=(m_radius-l)*10;
		m_position+=t*dp*dt;

	}

	scene::SCameraNode::update(dt);
}

}
}

