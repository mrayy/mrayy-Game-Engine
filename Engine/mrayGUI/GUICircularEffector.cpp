

#include "GUICircularEffector.h"
#include "GUIElement.h"
#include "mMath.h"


namespace mray{
namespace GUI{



GUICircularEffector::GUICircularEffector(math::vector2d center,float radius,float speed):
	Center(this,&GUICircularEffector::setCenter,&GUICircularEffector::getCenter,EPT_VECTOR2Df,mT("Center")),
	Radius(this,&GUICircularEffector::setRadius,&GUICircularEffector::getRadius,EPT_FLOAT,mT("Radius")),
	Speed(this,&GUICircularEffector::setSpeed,&GUICircularEffector::getSpeed,EPT_FLOAT,mT("Speed"))
{
	m_pos=math::vector2d(sinf(0),cosf(0));
	this->m_Center=center;
	this->m_Radius=radius;
	this->m_Speed=speed;

	fillProperties();
}

GUICircularEffector::~GUICircularEffector(){
}
void GUICircularEffector::fillProperties(){
	addPropertie(&Center);
	addPropertie(&Radius);
	addPropertie(&Speed);
}

void GUICircularEffector::effect(GUIElement* elem,float dt){
	if(elem){
		math::vector2d P2=m_pos;
		float st=math::sind(dt*m_Speed);
		float ct=math::cosd(dt*m_Speed);

		m_pos.x=P2.x*ct-P2.y*st;
		m_pos.y=P2.x*st+P2.y*ct;
		
		P2*=m_Radius;
		P2+=m_Center;	
		if(elem->getParent())
			P2+=elem->getParent()->getAbsPos();
		
		elem->MainPos=P2;
	}
}

}
}
