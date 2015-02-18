
/********************************************************************
	created:	2009/05/19
	created:	19:5:2009   1:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIPercentSizeEffector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIPercentSizeEffector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIPercentSizeEffector___
#define ___GUIPercentSizeEffector___

#include "GUIEffector.h"
#include "Point2d.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIPercentSizeEffector:public GUIEffector
{
private:
protected:
	math::vector2d m_sizePercent;
	math::vector2di m_constrain;
	virtual void fillProperties();
public:
	DECLARE_RTTI;

	rwProperty<GUIPercentSizeEffector,math::vector2d> SizePercent;
	rwProperty<GUIPercentSizeEffector,math::vector2di> Constrain;

	GUIPercentSizeEffector(const math::vector2d& percent,const math::vector2di&constrain);

	virtual void effect(GUIElement* elem,float dt);

	
	void setSizePercent(const math::vector2d& v){m_sizePercent=v;}
	const math::vector2d& getSizePercent(){return m_sizePercent;}

	void setConstrain(const math::vector2di& v){m_constrain=v;}
	const math::vector2di& getConstrain(){return m_constrain;}
};

}
}


#endif //___GUIPercentSizeEffector___
