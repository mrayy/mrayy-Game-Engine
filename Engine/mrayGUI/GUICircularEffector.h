

#ifndef ___GUICircularEff___
#define ___GUICircularEff___


#include "GUIEffector.h"
#include "SColor.h"
#include "Point2d.h"


namespace mray{
namespace GUI{

class MRAY_DLL GUICircularEffector:public GUIEffector{

	math::vector2d m_pos;
	math::vector2d m_Center;
	float m_Radius;
	float m_Speed;


protected:
	virtual void fillProperties();

public:
	DECLARE_RTTI;

	rwProperty<GUICircularEffector,math::vector2d> Center;
	rwProperty<GUICircularEffector,float> Radius;
	rwProperty<GUICircularEffector,float> Speed;

	GUICircularEffector(math::vector2d center,float radius,float speed);
	virtual~GUICircularEffector();
	virtual void effect(GUIElement* elem,float dt);

	void setCenter(const math::vector2d &v){m_Center=v;}
	const math::vector2d& getCenter(){return m_Center;}

	void setRadius(const float &v){m_Radius=v;}
	const float& getRadius(){return m_Radius;}

	void setSpeed(const float &v){m_Speed=v;}
	const float& getSpeed(){return m_Speed;}

};

}
}

#endif




